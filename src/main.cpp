#include "../h/memoryAllocator.hpp"
#include "../h/riscv.hpp"
#include "../h/syscall_c.hpp"
#include "../h/tcb.hpp"

extern void userMain();

void wrapperMain(void *arg) {
    userMain();
}

void main() {
    MemoryAllocator::initMem();

    TCB *threads[2];

    Riscv::w_stvec((uint64) &Riscv::supervisorTrap); // Function for trap handler
    Riscv::ms_sstatus(Riscv::SSTATUS_SIE); // Enable software interrupts

    thread_create(&threads[0], nullptr, nullptr);
    TCB::running = threads[0];

    thread_create(&threads[1], &wrapperMain, nullptr);

    while (!threads[1]->isFinished()) {
        thread_dispatch();
    }
}