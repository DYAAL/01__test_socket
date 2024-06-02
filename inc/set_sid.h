#ifndef __SET_GUARD_H__
#define __SET_GUARD_H__

#include <unistd.h>

class SET_GUARD {
public:
    SET_GUARD();
    SET_GUARD(SET_GUARD&&) = default;
    SET_GUARD(const SET_GUARD&) = default;
    SET_GUARD& operator=(SET_GUARD&&) = default;
    SET_GUARD& operator=(const SET_GUARD&) = default;
    ~SET_GUARD();
    // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ //
    void set_fork();
    void set_sid();
    void set_error(int errocode);
    void close_father_fds();
    void signal_exit();
    void set_guard();

protected:
    pid_t pid;
    pid_t sid;
};

#endif // __SET_GUARD_H__