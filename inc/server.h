#ifndef __SERVER_H__
#define __SERVER_H__
#include "socket_base.h"
#include <arpa/inet.h>
#include <cerrno>
#include <cstdio>
#include <cstring>
#include <map>
#include <netinet/in.h>
#include <strings.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

class Socket_Server : public Socket_Base {
public:
    Socket_Server();
    Socket_Server(int connection_num, int buffer_size);
    int server_setsockopt(int level, int optname);
    int server_timeout_set(int sec, int usec);
    
    void server_cout_error(int error_code);
    int server_bind();
    int server_listen();
    int server_accept();

    int socket_judge_maxfd() override;
    int socket_select_set(timeval* timeout_t) override;
    ~Socket_Server();
    int socket_judge_set() override;

private:
    int new_fd;
    int optval;
    fd_set read_set;
    const int max_connections = 5;
    std::map<int, sockaddr_in> sockaddr_ts;
    timeval timeout;
    const int read_buffer_size = 1024;
    std::vector<char> recv_buf;
};



#endif // __SERVER_H__