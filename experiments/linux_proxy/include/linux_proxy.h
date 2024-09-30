#pragma once

#ifndef INCLUDE_HEADER
#define INCLUDE_HEADER
#endif

#include <sys/syscall.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <sys/inotify.h>
#include <fcntl.h>
#include <sys/mman.h>

#include <stdio.h>
#include <sys/inotify.h>
#include <sys/time.h>

#include <sys/epoll.h>
#include <errno.h>

#include <linux/memfd.h>
#include <sys/mman.h>
#include <sys/syscall.h>
#include <unistd.h>

int borrow_fd_from_pid(int pid, int fd);
