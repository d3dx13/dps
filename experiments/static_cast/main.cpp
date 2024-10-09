#include <iostream>
#include <unistd.h>
#include <signal.h>
#include <vector>

// dma_buf
#include <sys/mman.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <linux/dma-buf.h>
#include <linux/dma-heap.h>

using namespace std;

int dmabuf_heap_open()
{
  int i;
  static const char *heap_names[] = { "/dev/dma_heap/linux,cma", "/dev/dma_heap/reserved", "/dev/dma_heap/system" };

  for(i = 0; i < 3; i++)
  {
    printf("Try open fd %s\n", heap_names[i]);
    int fd = open(heap_names[i], O_RDWR, 0);

    if(fd >= 0)
      return fd;
  }

  return -1;
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

struct test_msg {
    int a;
    int b;
    int c;
    int d[];
};


class BufferMemoryAllocator {
    public:
        template <typename T>
        static auto allocate(char buffer_[], std::size_t offset_)
        {
            cout << "sizeof(T) " << sizeof(T) << "\n";
            cout << "sizeof(buffer_) " << sizeof(&buffer_) << "\n";
            const auto new_offset = offset_ + sizeof(T);
            const auto place = buffer_ + new_offset - sizeof(T);
            offset_ = new_offset;
            return new (place) T{};
        }
};

void signalHandler(int signum) {
    if (signum > 0) {
        std::cout << "Interrupt signal (" << signum << ") received, pid " << getpid() << "\n";
    } else {
        std::cout << "Calling Node Destructor, pid " << getpid() << "\n";
    }
    
    exit(signum);
};

int main(int argc, char *argv[])
{
    
    for (int i = SIGHUP; i <= SIGTERM; i++){
        signal(i, signalHandler);
    }
    
    int heap_fd = dmabuf_heap_open();
    cout << "heap_fd " << heap_fd << "\n";

    const size_t size = 12;
    int fd = dmabuf_heap_alloc(heap_fd, "wft", size);
    cout << "fd " << fd << "\n";

    void *map = NULL;
    map = mmap(0, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED){
        printf("mmap failed!\n");
        return -errno;
    }
    char* buf = (char*) map;
    test_msg *msg = BufferMemoryAllocator::allocate<test_msg>(buf, 0);

    cout << sizeof(buf) << ":" << "\n\n";
    for (int i = 0; i < 24; i++){
        cout << i << ":" << int(buf[i]) << "\n";
    }

    buf[2] = 123;

    cout << sizeof(buf) << ":" << "\n\n";
    for (int i = 0; i < 24; i++){
        cout << i << ":" << int(buf[i]) << "\n";
    }

    cout << msg->a << ", " << msg->b << ", " << msg->c << "\n";

    msg->a = 1231;
    msg->b = 23111;
    msg->c = 11111231;

    cout << msg->a << ", " << msg->b << ", " << msg->c << "\n";
    
    cout << sizeof(buf) << ":" << "\n\n";
    for (int i = 0; i < 24; i++){
        cout << i << ":" << int(buf[i]) << "\n";
    }

    /*
    test_msg *msg = BufferMemoryAllocator::allocate<test_msg>(map, 0);
    cout << "sizeof(*msg): " << sizeof(*msg) << "\n";

    cout << sizeof(map) << ":" << "\n\n";
    for (int i = 0; i < 24; i++){
        cout << i << ":" << *(&map + i) << "\n";
    }
    cout << msg->a << ", " << msg->b << ", " << msg->c << "\n";
    cout << *msg->d << "\n";

    msg->a = 1231;
    msg->b = 23111;
    msg->c = 11111231;
    *(msg->d + 1000) = 12312;
    
    cout << sizeof(map) << ":" << "\n\n";
    for (int i = 0; i < 24; i++){
        cout << i << ":" << *(&map + i) << "\n";
    }
    cout << msg->a << ", " << msg->b << ", " << msg->c << "\n";
    cout << *(msg->d + 1000) << "\n";
    */

    return 0;
}