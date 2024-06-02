#include "client.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

int Socket_Client::client_timeout_set(int sec, int usec)
{
    this->timeout.tv_usec = usec;
    this->timeout.tv_sec = sec;
    return 0;
}

int Socket_Client::socket_select_set(timeval* timeout_t)
{
    FD_ZERO(&this->read_set);
    FD_SET(this->fd, &this->read_set);
    FD_SET(STDIN_FILENO, &this->read_set);
    socket_judge_maxfd();
    if (select(this->fd + 1, &this->read_set, NULL, NULL, timeout_t) < 0) {
        std::cerr << "selext failed!!" << std::endl;
        return -1;
    }
    return 0;
}
int Socket_Client::client_handler_01()
{
    this->recv_buf.clear();
    this->recv_buf.resize(this->read_buffer_size);
    ssize_t readRet;
    do {
        readRet = read(STDIN_FILENO, this->recv_buf.data(), this->read_buffer_size);
    } while (readRet == -1 && errno == EINTR);

    if (readRet == 0) {
        if (socket_close_fd(this->fd) < 0) {
            return -1;
        }
        return -2;
    }

    if (readRet < 0) {
        std::cerr << "read failed!!" << std::endl;
        if (socket_close_fd(this->fd) < 0) {
            return -1;
        }
        return -1;
    }

    if (readRet > 0) {
        ssize_t sendRet = send(this->fd, this->recv_buf.data(), readRet, 0);
        if (sendRet < 0) {
            std::cerr << "send failed!!" << std::endl;
            if (socket_close_fd(this->fd) < 0) {
                return -1;
            }
            return -1;
        }
    }

    // Check for quit message
    std::string recv_msg(this->recv_buf.data(), readRet);
    if (recv_msg.find(this->quit_msg) != std::string::npos) {
        if (socket_close_fd(this->fd) < 0) {
            return -1;
        }
        return -2;
    }
    return 0;
}

int Socket_Client::client_handler_02()
{
    int readret;
    this->recv_buf.clear();
    do {
        readret = recv(this->fd, this->recv_buf.data(), this->read_buffer_size, 0);
    } while (readret == -1 && errno == EINTR);
    if (readret == 0) {
        close(this->fd);
        return -2;
    }
    if (readret > 0) {
        std::cout.write(this->recv_buf.data(), readret);
    }
    if (readret < 0) {
        std::cerr << "recv failed!!" << std::endl;
        if (socket_close_fd(this->fd) < 0) {
            return -1;
        }
    }
    return 0;
}

Socket_Client::Socket_Client()
    : Socket_Base()
    , timeout{}
{
    FD_ZERO(&this->read_set);
  
    recv_buf.reserve(read_buffer_size);
    recv_buf.clear();
}

int Socket_Client::socket_judge_set()
{
    if (FD_ISSET(STDIN_FILENO, &this->read_set)) {
        if (client_handler_01() < 0) {
            return -1;
        }
    }
    if (FD_ISSET(this->fd, &this->read_set)) {
        if (client_handler_02() < 0) {
            return -1;
        }
    }
    return 0;
}

Socket_Client::Socket_Client(int buffer_size)
    : Socket_Base()
    , read_buffer_size(buffer_size)
    , timeout{}

{
    FD_ZERO(&this->read_set);
    recv_buf.reserve(read_buffer_size);
    recv_buf.clear();
}

Socket_Client::~Socket_Client()
{
    
}

int Socket_Client::socket_judge_maxfd()
{
    if (this->fd > maxfd) {
        maxfd = this->fd;
    }
    return 0;
}
