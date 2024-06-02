#include "server.h"
#include "iostream"
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */

int Socket_Server::server_setsockopt(int level, int optname)
{
    int ret;
    ret = setsockopt(this->fd, level, optname, &this->optval, sizeof(this->optval));
    if (ret < 0) {

        std::cerr << "setsockopt failed!!! " << strerror(errno) << std::endl;
        return -1;
    }
    return 0;
}

int Socket_Server::server_timeout_set(int sec, int usec)
{
    this->timeout.tv_usec = usec;
    this->timeout.tv_sec = sec;
    return 0;
}

void Socket_Server::server_cout_error(int error_code)
{
    if (error_code < 0) {
        socket_close_fd(this->fd);
        socket_close_fd(this->new_fd);
        exit(1);
    }
}

int Socket_Server::server_bind()
{
    if (bind(this->fd, (struct sockaddr*)&this->sin, sizeof(this->sin)) < 0) {
        std::cerr << "bind failed!!! " << strerror(errno) << std::endl;
        return -1;
    }
    return 0;
}

int Socket_Server::server_listen()
{
    if (listen(this->fd, 3) < 0) {
        std::cerr << "listen failed!!! " << strerror(errno) << std::endl;
        return -1;
    }
    return 0;
}

int Socket_Server::server_accept()
{
    sockaddr_in sockaddr_in_temp;
    socklen_t addrlen = sizeof(sockaddr_in_temp);
    if ((this->new_fd = accept(this->fd, (struct sockaddr*)&sockaddr_in_temp, &addrlen)) < 0) {
        std::cerr << "accept failed!!! " << strerror(errno) << std::endl;
        return -1;
    } else {
        std::cout << "Connection accepted" << std::endl;
        std::cout << "New connection, socket fd is " << this->new_fd << ", IP is : " << inet_ntoa(sockaddr_in_temp.sin_addr) << ", port : " << ntohs(sockaddr_in_temp.sin_port) << std::endl;
        sockaddr_ts.insert({ this->new_fd, sockaddr_in_temp });
        return 0;
    }
}

int Socket_Server::socket_judge_maxfd()
{
    if (this->fd > this->new_fd) {
        this->maxfd = this->fd;
    }
    if (this->new_fd > this->maxfd) {
        this->maxfd = this->new_fd;
    }
    return 0;
}

int Socket_Server::socket_select_set(timeval* timeout_t)
{
    FD_ZERO(&this->read_set);
    for (auto it = sockaddr_ts.begin(); it != sockaddr_ts.end(); ++it) {
        int temp_fd = it->first;
        FD_SET(temp_fd, &this->read_set);
    }
    FD_SET(this->fd, &this->read_set);
    socket_judge_maxfd();
    if (select(this->maxfd + 1, &this->read_set, NULL, NULL, timeout_t) < 0) {
        std::cerr << "select failed!!" << strerror(errno) << std::endl;
        return -1;
    }
    return 0;
}

Socket_Server::~Socket_Server()
{
}

int Socket_Server::socket_judge_set()
{
    this->recv_buf.clear();
    int ret;
    if (FD_ISSET(this->fd, &this->read_set)) {
        ret = server_accept();
        return ret;
    } else {
        for (const auto& pair : sockaddr_ts) {
            int client_fd = pair.first;
            const sockaddr_in& addr = pair.second;
            if (FD_ISSET(client_fd, &read_set)) {
                ssize_t readret = read(client_fd, this->recv_buf.data(), this->read_buffer_size);
                if (readret == 0) {
                    std::cout << "Break connection, socket fd is " << this->new_fd << ", IP is : " << inet_ntoa(this->sockaddr_ts[client_fd].sin_addr) << ", port : " << ntohs(this->sockaddr_ts[client_fd].sin_port) << std::endl;
                    socket_close_fd(client_fd);
                    sockaddr_ts.erase(client_fd);
                    FD_CLR(client_fd, &this->read_set);
                    return 0;
                } else if (readret > 0) {
                    std::string received_data = std::string(this->recv_buf.data(), readret);
                    std::cout << received_data << std::endl;
                    return 0;
                } else {
                    std::cerr << "Error: Receive failed." << std::endl;
                    return -1;
                }
            }
        }
    }

    return 0; // 在这里返回，表示处理完所有客户端后返回
}

Socket_Server::Socket_Server(int connection_num, int buffer_size)
    : max_connections(connection_num)
    , timeout {}
    , read_buffer_size(buffer_size)
{
    new_fd = -1;
    optval = 1;
    recv_buf.reserve(read_buffer_size);
    recv_buf.clear();
}

Socket_Server::Socket_Server()
    : timeout {}
{
    new_fd = -1;
    optval = 1;
    recv_buf.reserve(read_buffer_size);
    recv_buf.clear();
}
