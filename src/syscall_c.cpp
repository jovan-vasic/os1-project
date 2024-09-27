#include "../h/syscall_c.hpp"
#include "../h/riscv.hpp"

inline uint64 syscall(uint64 code, uint64 *args) {
    uint64 returnVal;

    __asm__ volatile ("mv a1, %0" : : "r"(args));
    __asm__ volatile ("li a0, %0" : : "i"(code));
    __asm__ volatile ("ecall");
    __asm__ volatile ("mv %0, a0" : "=r"(returnVal));

    return returnVal;
}

void *mem_alloc(size_t size) {
    size_t newSize = (size % MEM_BLOCK_SIZE == 0) ? size : ((size + MEM_BLOCK_SIZE - 1) / MEM_BLOCK_SIZE) *
                                                           MEM_BLOCK_SIZE;
    uint64 args[] = {newSize};
    return (void *) syscall(MEM_ALLOC, args);
}

int mem_free(void *ptr) {
    uint64 args[] = {(uint64) ptr};
    return (int) syscall(MEM_FREE, args);
}

int thread_create(thread_t *handle, Body routine, void *arg) {
    void *stack = (routine != nullptr ? mem_alloc(DEFAULT_STACK_SIZE) : nullptr);
    uint64 args[] = {(uint64) handle, (uint64) routine, (uint64) arg, (uint64) stack};
    return (int) syscall(THREAD_CREATE, args);
}

int thread_exit() {
    return (int) syscall(THREAD_EXIT, nullptr);
}

void thread_dispatch() {
    syscall(THREAD_DISPATCH, nullptr);
}

int thread_get_my_id() {
    return (int) syscall(THREAD_GET_MY_ID, nullptr);
}

//void thread_join(thread_t *handle) {
//    uint64 args[] = {(uint64) handle};
//    syscall(THREAD_JOIN, args);
//}

int sem_open(sem_t *handle, unsigned init) {
    uint64 args[] = {(uint64) handle, (uint64) init};
    return (int) syscall(SEM_OPEN, args);
}

int sem_close(sem_t handle) {
    uint64 args[] = {(uint64) handle};
    return (int) syscall(SEM_CLOSE, args);
}

int sem_wait(sem_t handle) {
    uint64 args[] = {(uint64) handle};
    return (int) syscall(SEM_WAIT, args);
}

int sem_signal(sem_t handle) {
    uint64 args[] = {(uint64) handle};
    return (int) syscall(SEM_SIGNAL, args);
}

int sem_trywait(sem_t handle) {
    uint64 args[] = {(uint64) handle};
    return (int) syscall(SEM_TRYWAIT, args);
}

char getc() {
    return (char) syscall(GETC, nullptr);
}

void putc(char c) {
    uint64 args[] = {(uint64) c};
    syscall(PUTC, args);
}

int time_sleep(time_t time) {
    // Not used
    return 0;
}