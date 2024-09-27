#ifndef _TCB_HPP
#define _TCB_HPP

#include "../lib/hw.h"
#include "scheduler.hpp"

using thread_t = TCB *;

// Thread Control Block
class TCB {
public:
    using Body = void (*)(void *);

    static TCB *running;

    TCB(Body b, void *a, void *s) {
        body = b;
        arg = a;
        stack = body ? new uint64[DEFAULT_STACK_SIZE] : nullptr;
        context.ra = body != nullptr ? (uint64) &TCB::wrapper : 0;
        context.sp = stack != nullptr ? (uint64) &stack[DEFAULT_STACK_SIZE] : 0;
        finished = false;
        blocked = false;

        myId = nextId++;
        if (myId >= 1) {
            threadCnt++;
        }
    }

    ~TCB() { delete[] stack; }

    static int createThread(thread_t *, Body, void *, void *);

    static void start(TCB *);

    static void dispatch();

    static void exit();

    static int getMyId();
//    static void join (thread_t* handle);

    bool isFinished() const { return finished; }

    void setFinished(bool done) { finished = done; }

    void block() { blocked = true; }

    void unblock() { blocked = false; }

    bool isBlocked() const { return blocked; }

private:
    struct Context {
        uint64 ra;
        uint64 sp;
    };

    Body body;
    uint64 *stack;
    Context context;
    void *arg;
    bool finished;
    bool blocked;

    int myId;
    static int nextId;
    static int threadCnt;

    static void contextSwitch(Context *, Context *);

    static void wrapper();

    friend class Thread;

    friend class mySemaphore;
};

#endif
