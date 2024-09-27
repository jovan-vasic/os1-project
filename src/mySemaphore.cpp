#include "../h/mySemaphore.hpp"

void mySemaphore::block() {
    TCB *old = TCB::running;
    if (!old->isFinished()) {
        blockedThreads.addLast(old);
    }
    TCB::running = Scheduler::get();
    TCB::contextSwitch(&old->context, &TCB::running->context);
}

void mySemaphore::unblock() {
    if (TCB *thread = blockedThreads.removeFirst()) {
        Scheduler::put(thread);
    }
}

int mySemaphore::open(sem_t *handle, unsigned init) {
    *handle = new mySemaphore(init);
    return (*handle != nullptr) ? 0 : -1;
}

int mySemaphore::signal(sem_t handle) {
    if (!handle) return -1;
    if (++handle->value <= 0) {
        handle->unblock();
    }
    return 0;
}

int mySemaphore::wait(sem_t handle) {
    if (!handle) return -1;
    if (--handle->value < 0) {
        handle->block();
    }
    return 0;
}

int mySemaphore::trywait(sem_t handle) {
    if (!handle) return -1;
    if (handle->value > 0) {
        handle->value--;
        return 0;
    }
    return 1;
}

int mySemaphore::close(sem_t handle) {
    if (!handle) return -1;
    while (TCB *thread = handle->blockedThreads.removeFirst()) {
        Scheduler::put(thread);
    }
    delete handle;
    return 0;
}
