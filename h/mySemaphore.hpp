#ifndef _MYSEMAPHORE_HPP
#define _MYSEMAPHORE_HPP

#include "../h/tcb.hpp"
#include "../h/list.hpp"

class mySemaphore {
public:
    using sem_t = mySemaphore *;

    mySemaphore(unsigned init = 1) {
        value = init;
    }

    static int signal(sem_t);

    static int wait(sem_t);

    static int trywait(sem_t);

    static int open(sem_t *, unsigned);

    static int close(sem_t);

    int getValue() const {
        return value;
    }

protected:
    void block();

    void unblock();

private:
    int value;
    List<TCB> blockedThreads;
};

#endif
