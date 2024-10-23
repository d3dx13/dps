#include "dps/core/event.h"

namespace dps
{
    Event::Event()
    {
        unsigned int initval = 0;
        int flags = EFD_SEMAPHORE;
        this->event_fd = eventfd(initval, flags);
    }

    Event::~Event()
    {
        if (this->event_fd > 0)
        {
            close(this->event_fd);
        }
    }

    void Event::notify()
    {
        eventfd_t value = 1;
        eventfd_write(this->event_fd, value);
        eventfd_read(this->event_fd, &value);
    }

    int Event::fd() { return this->event_fd; }
} // namespace dps