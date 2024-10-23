#ifndef EVENT_H
#define EVENT_H

#include "dps/core/common.h"

namespace dps
{
    class Event
    {
    public:
        Event();
        ~Event();

        void notify();
        int fd();

    protected:
        int event_fd = -1;
    };
}

#endif