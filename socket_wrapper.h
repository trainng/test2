#ifndef PROXY_LINUX_SOCKET_H
#define PROXY_LINUX_SOCKET_H


#include <bits/sockaddr.h>
#include <cstdint>
#include <netinet/in.h>
#include <string>
#include "fd_wrapper.h"
#include "my_error.h"

class socket_wrapper {
public:
    socket_wrapper();

    socket_wrapper(int fd);


    socket_wrapper(socket_wrapper &&) = default;

    socket_wrapper &operator=(socket_wrapper &&) = default;

    void bind(sa_family_t sa_family, uint16_t port, in_addr_t s_addr);

    void listen();

    int accept();


    uint32_t get_flags();

    int get_available_bytes();

    void set_flags(uint32_t flags);

    fd_wrapper &get_fd();

    ~socket_wrapper();

    size_t write(const std::string &msg);

    std::string read(size_t buffer_size);

private:
    fd_wrapper fd;

};


#endif //PROXY_LINUX_SOCKET_H
