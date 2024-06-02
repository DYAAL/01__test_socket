#ifndef __CLIENT_H__
#define __CLIENT_H__
#include "socket_base.h"
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>

#include <vector>

class Socket_Client : public Socket_Base {
public:
    Socket_Client();
    Socket_Client(int read_buffer_size);
    ~Socket_Client();
    int client_timeout_set(int sec, int usec);
    int socket_select_set(timeval* timeout_t) override;
    int socket_judge_set() override;
    int socket_judge_maxfd() override;
    int client_handler_01();
    int client_handler_02();


private:
    fd_set read_set;
    const int read_buffer_size = 1024;
    timeval timeout;
    std::vector<char> recv_buf;
    const std::string quit_msg = "quit";
};






#endif // __CLIENT_H__