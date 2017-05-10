#include <vector>
#include <csignal>
#include <iostream>
#include <cassert>
#include "socket_wrapper.h"


socket_wrapper::socket_wrapper() : fd(-1) {

}


socket_wrapper::socket_wrapper(int fd) : fd(fd) {
    set_flags(get_flags() | SOCK_STREAM | SOCK_NONBLOCK);
}


void socket_wrapper::bind(sa_family_t sa_family, uint16_t port, in_addr_t s_addr) {
    struct sockaddr_in temp;

    temp.sin_family = sa_family;
    temp.sin_port = htons(port);
    temp.sin_addr.s_addr = htons(s_addr);

    if (::bind(fd.get_fd(), (struct sockaddr *) &temp, sizeof(temp)) == -1) {

        throw_server_error("Error in bind posix_socket");
    }
}

void socket_wrapper::listen() {
    if (::listen(fd.get_fd(), SOMAXCONN) == -1) {
        throw_server_error("Error in listen posix_socket");
    }
}

int socket_wrapper::accept() {
    int result = ::accept(fd.get_fd(), (struct sockaddr *) NULL, NULL);
    if (result == -1) {
        throw_server_error("Error in accept posix_socket");
    }

    return result;
}

uint32_t socket_wrapper::get_flags() {
    return fd.get_flags();
}

int socket_wrapper::get_available_bytes() {
    return fd.get_available_bytes();
}


void socket_wrapper::set_flags(uint32_t flags) {
    fd.set_flags(flags);
}

fd_wrapper &socket_wrapper::get_fd() {
    return fd;
}

socket_wrapper::~socket_wrapper() {
    //std::cerr << "Socket destroyed\n";
}


size_t socket_wrapper::write(std::string const &msg) {
    ssize_t len;
    if ((len = send(get_fd().get_fd(), msg.c_str(), msg.size(), 0)) == -1) {
        throw_server_error("Error while writing!");
    }

    return len;
}

std::string socket_wrapper::read(size_t buffer_size) {
    std::vector<char> buffer(buffer_size);
    int recieved = (recv(get_fd().get_fd(), buffer.data(), buffer_size, 0));
    if (recieved == -1) {
        throw_server_error("Error while reading!");
    }
    assert(recieved == buffer_size);
    return std::string(buffer.begin(), buffer.end());
}

