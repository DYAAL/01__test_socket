#include "main.h"
#include "client.h"


int main(int argc, const char** argv)
{
    Socket_Client client;
    int ret;
    ret = client.socket_ip_init(SERVRT_IP, SERVRT_PORT);
    client.cout_error(ret);
    ret = client.socket_sockaddr_init(AF_INET, SOCK_STREAM, 0);
    client.cout_error(ret);
    ret = client.socket_connect();
    client.cout_error(ret);
    ret = client.client_timeout_set(5, 0);
    client.cout_error(ret);
    while (1) {
        ret = client.socket_select_set(nullptr);
        client.cout_error(ret);
        ret = client.socket_judge_set();
        client.cout_error(ret);
    }

    return 0;
}