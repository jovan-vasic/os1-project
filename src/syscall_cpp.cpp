#include "../h/syscall_cpp.hpp"

// Thread

Thread::Thread(void (*body)(void *), void *arg) {
    this->body = body;
    this->arg = arg;
}

Thread::Thread() {
    myHandle = nullptr;
    body = nullptr;
    arg = nullptr;
}

Thread::~Thread() {}

int Thread::start() {
    int retVal;

    if (body) {
        retVal = thread_create(&myHandle, body, arg);
    } else {
        retVal = thread_create(&myHandle, &Thread::threadWrapper, this);
    }

    return retVal;
}

void Thread::dispatch() {
    thread_dispatch();
}

int Thread::sleep(time_t t) {
    return 0;
}

//void Thread::join() {
//    if (myHandle)
//        thread_join(&myHandle);
//}

// Semaphore

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle, init);
}

Semaphore::~Semaphore() {
    sem_close(myHandle);
}

int Semaphore::wait() {
    return sem_wait(myHandle);
}

int Semaphore::signal() {
    return sem_signal(myHandle);
}

int Semaphore::tryWait() {
    return sem_trywait(myHandle);
}

// Console

char Console::getc() {
    return ::getc();
}

void Console::putc(char c) {
   ::putc(c);
}
