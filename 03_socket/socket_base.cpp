#include "socket_base.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

Socket_Base::Socket_Base()
    : sin {}
    , server_info{}
{
  
    this->fd = -1;
    this->server_port_rdy = 0;
    this->server_ip_rdy = 0;
    this->maxfd = 5;
}

Socket_Base::~Socket_Base()
{
}

int Socket_Base::socket_ip_init(std::string server_ip, uint16_t server_port)
{
    this->server_info.server_port = server_port;
    this->server_info.server_ip = server_ip;
    this->server_port_rdy = htons(this->server_info.server_port);
    this->server_ip_rdy = inet_addr(this->server_info.server_ip.c_str());
    if (this->server_ip_rdy == INADDR_NONE) {
        std::cerr << "inet_addr failed" << std::endl;
        return -1;
    }
    return 0;
}

int Socket_Base::socket_sockaddr_init(int __domain, int __type, int __protocol)
{
    this->fd = socket(__domain, __type, __protocol);
    if (fd < 0) {
        std::cerr << "socket failed" << std::endl;
        return -1;
    }
    this->sin.sin_port = this->server_port_rdy;
    this->sin.sin_family = __domain;
    this->sin.sin_addr.s_addr = this->server_ip_rdy;
    return 0;
}

int Socket_Base::socket_close_fd(int which_fd)
{
    if (close(which_fd) < 0) {
        std::cerr << "close failed" << std::endl;
        return -1;
    }
    return 0;
}

void Socket_Base::cout_error(int error_code)
{
    if (error_code < 0) {
        socket_close_fd(this->fd);
        exit(1);
    }
}

int Socket_Base::socket_connect()
{
    if (connect(this->fd, (struct sockaddr*)&this->sin, sizeof(sockaddr_in)) < 0) {
        std::cerr << "connect failed" << std::endl;
        return -1;
    }
    return 0;
}

int Socket_Base::socket_judge_maxfd()
{
    if (this->fd > maxfd) {
        maxfd = this->fd;
    }
    return 0;
}