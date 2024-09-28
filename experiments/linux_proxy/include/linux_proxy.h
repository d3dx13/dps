#pragma once

#ifndef INCLUDE_HEADER
#define INCLUDE_HEADER
#endif

#include <sys/syscall.h>
#include <unistd.h>
#include <sys/eventfd.h>
#include <fcntl.h>
#include <sys/mman.h>

int borrow_fd_from_pid(int pid, int fd);
