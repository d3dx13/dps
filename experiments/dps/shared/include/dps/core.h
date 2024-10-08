#include <stdlib.h>
#include <string>

#define DPS_BASE_PATH /dev/shm/.ips

namespace dps {
    class Node {
        private:
            int pid;
            std::string name;

        public:
            Node();
            Node(std::string name);

            ~Node();

            int get_pid();
    };
}

int borrow_fd_from_pid(int pid, int fd);
int pidfd_open(int fd);
int pidfd_getfd(int pid, int fd);

int dmabuf_heap_open();
void dmabuf_heap_close(int heap_fd);
int dmabuf_heap_alloc(int heap_fd, const char *name, size_t size);
int dmabuf_sync_start(int buf_fd);
int dmabuf_sync_stop(int buf_fd);
