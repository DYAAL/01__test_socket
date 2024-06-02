#include <arpa/inet.h>
#include <cerrno>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h> /* See NOTES */
#include <unistd.h>
// ---------------------------------------------------
#include "main.h"
#include "server.h"
#include "set_sid.h"

int main(int argc, const char** argv)
{
    Socket_Server server;
    int ret;
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    ret = server.socket_ip_init(SERVRT_IP, SERVRT_PORT);
    server.Socket_Base::cout_error(ret);
    ret = server.socket_sockaddr_init(AF_INET, SOCK_STREAM, 0);
    server.Socket_Base::cout_error(ret);
    server.server_setsockopt(SOL_SOCKET, SO_REUSEADDR);
    server.Socket_Base::cout_error(ret);
    ret = server.server_bind();
    server.Socket_Base::cout_error(ret);
    ret = server.server_listen();
    server.Socket_Base::cout_error(ret);
    ret = server.server_timeout_set(5, 0);
    server.Socket_Base::cout_error(ret);
    while (1) {
        ret = server.socket_select_set(nullptr);
        server.server_cout_error(ret);
        ret = server.socket_judge_set();

        server.server_cout_error(ret);
    }
    return 0;
}