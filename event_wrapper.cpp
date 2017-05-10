#include <sys/epoll.h>
#include "event_wrapper.h"

event_wrapper::event_wrapper(epoll_wrapper &service, fd_wrapper &fd, uint32_t flags, std::function<void(uint32_t)> handler)
        : service(service), fd(fd), flags(flags | EPOLLERR | EPOLLRDHUP | EPOLLHUP), callback(handler) {
    service.add(fd, *this, flags);

}

event_wrapper::~event_wrapper() {
    std::cerr<<"Removing from epoll "<<fd.get_fd()<<"\n";
    service.remove(fd, *this, 0);
    // std::cerr<<"Event deleted\n";
}


void event_wrapper::add_flag(uint32_t flag) {
    flags |= flag;
    service.modify(fd, *this, flags);
}

void event_wrapper::remove_flag(uint32_t flag) {
    flags &= ~flag;
    service.modify(fd, *this, flags);
}

