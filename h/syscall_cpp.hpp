#ifndef _SYSCALL_CPP_HPP
#define _SYSCALL_CPP_HPP

#include "syscall_c.hpp"

class Thread {
public:
    Thread(void (*body)(void *), void *arg);

    virtual ~Thread();

    int start();

    static void dispatch();

    static int sleep(time_t);

    static void threadWrapper(void *thread) {
        if (thread != nullptr) {
            ((Thread *) thread)->run();
        }
    }

//    void join();

protected:
    Thread();

    virtual void run() {}

private:
    thread_t myHandle;

    void (*body)(void *);

    void *arg;
};

class Semaphore {
public:
    Semaphore(unsigned init = 1);

    virtual ~Semaphore();

    int wait();

    int signal();

    int tryWait();

private:
    sem_t myHandle;
};

class Console {
public:
    static char getc();

    static void putc(char);
};

#endif
