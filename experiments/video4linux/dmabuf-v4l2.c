/*
 * Simple example application to show how to allocate dmabufs from user space 
 * (from a dmabuf heap) and use them for v4l2 capture.
 * 
 * 2022, Matthias Fend <matthias.fend@emfend.at>
 */
#define _GNU_SOURCE  
#include <errno.h>
#include <fcntl.h>
#include <poll.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdarg.h>
#include <linux/videodev2.h>
#include <stddef.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <linux/dma-buf.h>
#include <linux/dma-heap.h>
#include <stddef.h>

#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;

/*
 * Depending on the configuration method, the name of the device node name
 * of the dmabuf-heap changes. If the CMA area is configured from a device
 * tree node, the heap node is '/dev/dma_heap/linux,cma', otherwise the
 * node is '/dev/dma_heap/reserved'.
 * So let's just try both.
 */
int dmabuf_heap_open()
{
  int i;
  static const char *heap_names[] = { "/dev/dma_heap/system", };

  for(i = 0; i < 2; i++)
  {
    int fd = open(heap_names[i], O_RDWR, 0);

    if(fd >= 0)
      return fd;
  }

  return -1;
}

void dmabuf_heap_close(int heap_fd)
{
  close(heap_fd);
}

int dmabuf_heap_alloc(int heap_fd, const char *name, size_t size)
{
  struct dma_heap_allocation_data alloc = { 0 };

  alloc.len = size;
  alloc.fd_flags = O_CLOEXEC | O_RDWR;

  if(ioctl(heap_fd, DMA_HEAP_IOCTL_ALLOC, &alloc) < 0)
    return -1;

  if(name)
    ioctl(alloc.fd, DMA_BUF_SET_NAME, name);

  return alloc.fd;
}

static int dmabuf_sync(int buf_fd, bool start)
{
  struct dma_buf_sync sync = { 0 };

  sync.flags = (start ? DMA_BUF_SYNC_START : DMA_BUF_SYNC_END) | DMA_BUF_SYNC_RW;

  do
  {
    if(ioctl(buf_fd, DMA_BUF_IOCTL_SYNC, &sync) == 0)
      return 0;
  } while((errno == EINTR) || (errno == EAGAIN));

  return -1;
}

int dmabuf_sync_start(int buf_fd)
{
  return dmabuf_sync(buf_fd, true);
}

int dmabuf_sync_stop(int buf_fd)
{
  return dmabuf_sync(buf_fd, false);
}

struct arguments_t
{
  char *vdev_name;
  uint32_t timeout_ms;
  unsigned int loop_count;
  char *output_dir;
  uint32_t width;
  uint32_t height;
  uint32_t fourcc;
};

static void print_usage(const char *progname)
{
  printf("usage: %s [-dwhflo]\n", progname);
  printf("\t-d <video-device\t/dev/videoX\n");
  printf("\t-w <width>\tdesired image width\n");
  printf("\t-h <height>\tdesired image height\n");
  printf("\t-f <fourcc>\tdesired image fourcc\n");
  printf("\t-l <loop-count>\tnumber of program loops\n");
  printf("\t-t <timeout>\ttimeout [ms]\n");
  printf("\t-o <out-dir>\tdirectory for file outputs\n");
}

static int parse_arguments(int argc, char *argv[], struct arguments_t *args)
{
  int c;

  while((c = getopt(argc, argv, "d:l:w:h:f:t:o:")) != -1)
  {
    switch(c)
    {
      case 'd':
        args->vdev_name = optarg;
        break;

      case 'l':
        if(sscanf(optarg, "%u", &args->loop_count) != 1)
        {
          printf("Invalid loop count\n");
          return -1;
        }
        break;

      case 'w':
        if(sscanf(optarg, "%u", &args->width) != 1)
        {
          printf("Invalid image width\n");
          return -1;
        }
        break;

      case 'h':
        if(sscanf(optarg, "%u", &args->height) != 1)
        {
          printf("Invalid image width\n");
          return -1;
        }
        break;

      case 'f':
        if(strlen(optarg) != 4)
        {
          printf("Invalid image fourcc\n");
          return -1;
        }
        args->fourcc = ((uint32_t) optarg[0] << 0) | ((uint32_t) optarg[1] << 8) | ((uint32_t) optarg[2] << 16) | ((uint32_t) optarg[3] << 24);
        break;

      case 't':
        if(sscanf(optarg, "%u", &args->timeout_ms) != 1)
        {
          printf("Invalid timeout\n");
          return -1;
        }
        break;

      case 'o':
        args->output_dir = optarg;
        break;

      case '?':
        return -1;
    }
  }

  return 0;
}

int open_video_device(const char *vdevice, uint32_t in_width, uint32_t in_height, uint32_t in_fourcc, struct v4l2_pix_format *pix_fmt, bool *mplane_api)
{
  int fd;
  struct v4l2_capability caps;
  struct v4l2_format fmt;

  fd = open(vdevice, O_RDWR);
  if(fd < 0)
  {
    printf("Failed to open %s\n", vdevice);
    return -1;
  }

  memset(&caps, 0, sizeof(caps));
  if(ioctl(fd, VIDIOC_QUERYCAP, &caps))
  {
    printf("VIDIOC_QUERYCAP: %s\n", strerror(errno));
    exit(0);
  }

  if(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE)
  {
    printf("Using single-planar API\n");
    *mplane_api = false;
  }
  else if(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE_MPLANE)
  {
    printf("Using multi-planar API\n");
    *mplane_api = true;
  }
  else
  {
    printf("Devicce does not support video capture\n");
    exit(0);
  }

  memset(&fmt, 0, sizeof(fmt));
  if(*mplane_api)
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  else
    fmt.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(ioctl(fd, VIDIOC_G_FMT, &fmt))
  {
    printf("VIDIOC_G_FMT: %s\n", strerror(errno));
    exit(0);
  }

  if(in_width > 0)
    fmt.fmt.pix.width = in_width;

  if(in_height > 0)
    fmt.fmt.pix.height = in_height;

  if(in_fourcc)
    fmt.fmt.pix.pixelformat = in_fourcc;

  if(ioctl(fd, VIDIOC_S_FMT, &fmt))
  {
    printf("VIDIOC_S_FMT: %s\n", strerror(errno));
    exit(0);
  }

  if(ioctl(fd, VIDIOC_G_FMT, &fmt))
  {
    printf("VIDIOC_G_FMT: %s\n", strerror(errno));
    exit(0);
  }

  memcpy(pix_fmt, &fmt.fmt.pix, sizeof(*pix_fmt));

  return fd;

err_cleanup:
  close(fd);

  return -1;
}

int main(int argc, char *argv[])
{
  int i;
  struct arguments_t args;
  int dmabuf_heap_fd;
  unsigned int loop_count;
  const int num_buffers = 3;
  int v4l2_fd;
  struct v4l2_pix_format pix_fmt;
  bool mplane_api;
  int dmabuf_fds[num_buffers];
  void *dmabuf_maps[num_buffers];
  struct v4l2_requestbuffers rqbufs;
  struct pollfd pfds[1];

  memset(&args, 0, sizeof(args));
  args.vdev_name = "/dev/video0";
  args.timeout_ms = 5000;
  args.loop_count = 10;
  args.output_dir = "/tmp";
  args.width = 640;
  args.height = 480;
  args.fourcc = v4l2_fourcc('Y', 'U', 'Y', 'V');
  // #define V4L2_PIX_FMT_YUYV    v4l2_fourcc('Y', 'U', 'Y', 'V') /* 16  YUV 4:2:2     */

  if(parse_arguments(argc, argv, &args) < 0)
  {
    printf("Invalid arguments\n");
    print_usage(argv[0]);
    return -1;
  }

  /* open v4l2 device */
  v4l2_fd = open_video_device(args.vdev_name, args.width, args.height, args.fourcc, &pix_fmt, &mplane_api);
  if(v4l2_fd < 0)
    return -1;

  printf("Actual v4l2 device:  %s\n", args.vdev_name);
  printf("Actual timeout:      %ums\n", args.timeout_ms);
  printf("Actual image width:  %u\n", pix_fmt.width);
  printf("Actual image height: %u\n", pix_fmt.height);
  printf("Actual image format: %.4s\n", (char*) &pix_fmt.pixelformat);
  printf("Actual image size:   %u\n", pix_fmt.sizeimage);

  /* request buffers from v4l2 device */
  memset(&rqbufs, 0, sizeof(rqbufs));
  rqbufs.count = num_buffers;
  if(mplane_api)
    rqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
  else
    rqbufs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
  rqbufs.memory = V4L2_MEMORY_DMABUF;
  if(ioctl(v4l2_fd, VIDIOC_REQBUFS, &rqbufs))
  {
    printf("VIDIOC_REQBUFS: %s\n", strerror(errno));
    exit(0);
  }
  if(rqbufs.count < num_buffers)
  {
    printf("VIDIOC_REQBUFS: too few buffers\n");
    exit(0);
  }

  dmabuf_heap_fd = dmabuf_heap_open();
  if(dmabuf_heap_fd < 0)
  {
    printf("Could not open dmabuf-heap\n");
    exit(0);
  }

  /* allocate and map dmabufs */
  for(i = 0; i < num_buffers; i++)
  {
    dmabuf_fds[i] = dmabuf_heap_alloc(dmabuf_heap_fd, NULL, pix_fmt.sizeimage);
    if(dmabuf_fds[i] < 0)
    {
      printf("Failed to alloc dmabuf %d\n", i);
      exit(0);
    }

    dmabuf_maps[i] = mmap(0, pix_fmt.sizeimage, PROT_WRITE | PROT_READ, MAP_SHARED, dmabuf_fds[i], 0);
    if(dmabuf_maps[i] == MAP_FAILED)
    {
      printf("Failed to map dmabuf %d\n", i);
      exit(0);
    }
  }

  /* enque dmabufs into v4l2 device */
  for(i = 0; i < num_buffers; ++i)
  {
    struct v4l2_buffer buf;
    struct v4l2_plane planes[VIDEO_MAX_PLANES];

    memset(&buf, 0, sizeof(buf));
    buf.index = i;
    buf.memory = V4L2_MEMORY_DMABUF;
    if(mplane_api)
    {
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
      memset(&planes, 0, sizeof(planes));
      buf.m.planes = planes;
      buf.length = 1;
      buf.m.planes[0].m.fd = dmabuf_fds[i];
    }
    else
    {
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.m.fd = dmabuf_fds[i];
    }
    if(ioctl(v4l2_fd, VIDIOC_QBUF, &buf))
    {
      printf("VIDIOC_QBUF: %s\n", strerror(errno));
      exit(0);
    }
  }

  /* start v4l2 device */
  int type = mplane_api ? V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE : V4L2_BUF_TYPE_VIDEO_CAPTURE;
  if(ioctl(v4l2_fd, VIDIOC_STREAMON, &type))
  {
    printf("VIDIOC_STREAMON: %s\n", strerror(errno));
    exit(0);
  }

  pfds[0].fd = v4l2_fd;
  pfds[0].events = POLLIN;

  loop_count = 0;
  while((poll(pfds, 1, args.timeout_ms) > 0)) //  && (loop_count < args.loop_count)
  {
    struct v4l2_buffer buf;
    struct v4l2_plane planes[VIDEO_MAX_PLANES];
    int buf_index;

    /* dequeue a buffer */
    memset(&buf, 0, sizeof(buf));
    buf.memory = V4L2_MEMORY_DMABUF;
    if(mplane_api)
    {
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
      memset(&planes, 0, sizeof(planes));
      buf.m.planes = planes;
      buf.length = 1;
    }
    else
    {
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    }
    if(ioctl(v4l2_fd, VIDIOC_DQBUF, &buf))
    {
      printf("VIDIOC_DQBUF: %s\n", strerror(errno));
      exit(0);
    }

    buf_index = buf.index;

    /* prepare buffer for CPU access */
    // dmabuf_sync_start(dmabuf_fds[buf_index]);

    std::cout << "buf_index = " << buf_index << "\n";
    
    int rc;
    unsigned int offset, length;

    if(mplane_api)
    {
      offset = planes[0].data_offset;
      length = planes[0].bytesused;
    }
    else
    {
      offset = 0;
      length = buf.bytesused;
    }
    
    unsigned int width = args.width;
    unsigned int height = args.height;
    unsigned char* dataBuffer = (unsigned char*) dmabuf_maps[buf_index];
    Mat image(Size(width, height), CV_8UC2, dataBuffer, Mat::AUTO_STEP);
    std::vector<Mat> channels(2);
    // split img:
    split(image, channels);
    // get the channels (dont forget they follow BGR order in OpenCV)
    Mat ch1, ch2;
    ch1 = channels[0];
    ch2 = channels[1];
    imshow("ch1", ch1);
    imshow("ch2", ch2);
    Mat yuv_bluat(Size(width, height * 3 / 2), CV_8UC1);
    for (int y = 0; y < height; y++){
      for (int x = 0; x < width; x++){
        yuv_bluat.at<unsigned char>(y, x) = (unsigned char)ch1.at<unsigned char>(y, x);
      }
    }
    for (int y = 0; y < height / 2; y++){
      for (int x = 0; x < width; x++){
        yuv_bluat.at<unsigned char>(height + y, x) = (unsigned char)ch2.at<unsigned char>(y * 2, x);
      }
    }
    Mat yuv_bluat_rgb;
    cvtColor(yuv_bluat, yuv_bluat_rgb, COLOR_YUV2BGR_NV12);
    imshow("yuv_bluat", yuv_bluat);
    imshow("yuv_bluat_rgb" + std::to_string(buf_index), yuv_bluat_rgb);
    int k = waitKey(1); // Wait for a keystroke in the window

    /* release buffer for CPU access */
    // dmabuf_sync_stop(dmabuf_fds[buf_index]);

    /* enqueue a buffer */
    memset(&buf, 0, sizeof(buf));
    buf.index = buf_index;
    buf.memory = V4L2_MEMORY_DMABUF;
    if(mplane_api)
    {
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
      memset(&planes, 0, sizeof(planes));
      buf.m.planes = planes;
      buf.length = 1;
      buf.m.planes[0].m.fd = dmabuf_fds[buf_index];
    }
    else
    {
      buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
      buf.m.fd = dmabuf_fds[buf_index];
    }
    if(ioctl(v4l2_fd, VIDIOC_QBUF, &buf))
    {
      printf("VIDIOC_QBUF: %s\n", strerror(errno));
      exit(0);
    }

    loop_count++;
  }

  exit_cleanup:

  /* 
   * FIXME: add cleanup 
   */

  return 0;
}
