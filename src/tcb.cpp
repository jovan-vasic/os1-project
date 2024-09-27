#include "../h/tcb.hpp"
#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"

TCB *TCB::running = nullptr;

int TCB::nextId = -1;
int TCB::threadCnt = 0;

int TCB::createThread(thread_t *handle, Body body, void *arg, void *stack) {
    *handle = new TCB(body, arg, stack);
    if (body) Scheduler::put(*handle);
    return 0;
}

void TCB::start(TCB *thread) {
    if (thread->body) Scheduler::put(thread);
}

void TCB::dispatch() {
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked()) {
        Scheduler::put(old);
    }

    running = Scheduler::get();

    if (old != running) {
        TCB::contextSwitch(&old->context, &running->context);
    }
}

void TCB::exit() {
    running->setFinished(true);
    dispatch();
}

void TCB::wrapper() {
    Riscv::popSppSpie();
    running->body(running->arg);

    thread_exit();
}

int TCB::getMyId() {
    int id = running->myId;

//    dispatch();

    return id;
}

//void TCB::join(thread_t *handle) {
//    while (!(*handle)->isFinished()) {
//        TCB::dispatch();
//    }
//}