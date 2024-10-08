#pragma once

#ifndef INCLUDE_HEADER
#define INCLUDE_HEADER
#endif

#include <stdlib.h>
#include <iostream>
#include <string>

timespec get_timestamp();

// borrow_fd_from_pid
#include <sys/syscall.h>
#include <unistd.h>
#include <fcntl.h>

int borrow_fd_from_pid(int pid, int fd);
int pidfd_open(int fd);
int pidfd_getfd(int pid, int fd);

// shared memory
#include <signal.h>
#include <pthread.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/file.h>

// dma_buf
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
#include <drm/drm.h>

int dmabuf_heap_open();
void dmabuf_heap_close(int heap_fd);
int dmabuf_heap_alloc(int heap_fd, const char *name, size_t size);
int dmabuf_sync_start(int buf_fd);
int dmabuf_sync_stop(int buf_fd);
