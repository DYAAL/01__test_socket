#include "set_sid.h"
#include <cstdio>
#include <fcntl.h>
#include <iostream>
#include <signal.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

SET_GUARD::SET_GUARD()
{
    this->pid = 0;
    this->sid = 0;
}

SET_GUARD::~SET_GUARD()
{
}

void SET_GUARD::set_fork()
{
    this->pid = fork();
    set_error(this->pid);
}

void SET_GUARD::set_sid()
{
    int ret;
    sid = setsid();
    set_error(this->sid);
    ret = umask(0);
    set_error(ret);
    ret = (chdir("/"));
    set_error(ret);
}

void SET_GUARD::set_error(int errocode)
{
    if (errocode < 0) {
        exit(EXIT_FAILURE);
    }
    if (errocode > 0) {
        exit(EXIT_SUCCESS);
    }
}

void SET_GUARD::close_father_fds()
{
    for (int i = 0; i < getdtablesize(); i++) {
        close(i);
    }
}

void SET_GUARD::signal_exit()
{
    signal(SIGCHLD, SIG_IGN);
}

void SET_GUARD::set_guard()
{
    this->set_fork();
    std::cout << "pid: " << this->pid << std::endl;
    this->set_sid();
    std::cout << "pid: " << this->pid << std::endl;
    this->close_father_fds();
    this->signal_exit();
}
