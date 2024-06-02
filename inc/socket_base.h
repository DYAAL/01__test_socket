#ifndef __SOCKET_BASE_H__
#define __SOCKET_BASE_H__
#include <bits/types/struct_timeval.h>
#include <netinet/in.h>
#include <string>

class Socket_Base {
public:
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    virtual int socket_select_set(timeval* timeout_t) = 0;
    virtual int socket_judge_set() = 0;
    Socket_Base();
    virtual ~Socket_Base();
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
 
    int socket_ip_init(std::string server_ip, uint16_t server_port);
    int socket_sockaddr_init(int __domain, int __type, int __protocol);
    virtual int socket_connect();
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    virtual int socket_judge_maxfd();
    int socket_close_fd(int which_fd);
    struct Server_Info {
        std::string server_ip;
        uint16_t server_port;
    };
    virtual void cout_error(int error_code);

protected:
    sockaddr_in sin;
    int fd;
    int maxfd;
    in_port_t server_port_rdy;
    in_addr_t server_ip_rdy;
    Server_Info server_info; // 将结构体名称改为 server_infor
};

#endif // __SOCKET_BASE_H__