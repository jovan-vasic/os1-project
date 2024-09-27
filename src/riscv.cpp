#include "../h/riscv.hpp"
#include "../h/memoryAllocator.hpp"
#include "../h/tcb.hpp"
#include "../h/mySemaphore.hpp"

#include "../lib/console.h"
#include "../test/printing.hpp"

void Riscv::popSppSpie() {
    __asm__ volatile("csrw sepc, ra");
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap() {
    uint64 scause = r_scause();

    if (scause == 0x0000000000000008UL || scause == 0x0000000000000009UL) {
        // Interrupt: no
        // Environment call from U-mode(8) or S-mode(9)
        uint64 volatile sepc, sstatus;
        uint64 code, returnVal;
        uint64 *args;

        sepc = r_sepc() + 4;
        sstatus = r_sstatus();

        __asm__ volatile ("mv %0, a0" : "=r"(code));
        __asm__ volatile ("mv %0, a1" : "=r"(args));

        switch (code) {
            case MEM_ALLOC: {
                size_t bytes = args[0] * MEM_BLOCK_SIZE;

                void *p = MemoryAllocator::mem_alloc(bytes);
                w_sscratch((uint64) p);

                break;
            }
            case MEM_FREE: {
                returnVal = MemoryAllocator::mem_free((void *) args[0]);
                w_sscratch(returnVal);

                break;
            }
            case THREAD_CREATE: {
                returnVal = TCB::createThread((thread_t *) args[0], (Body) args[1], (void *) args[2], (void *) args[3]);
                w_sscratch(returnVal);

                break;
            }
            case THREAD_EXIT: {
                returnVal = 0;

                TCB::exit();
                w_sscratch(returnVal);

                break;
            }
            case THREAD_DISPATCH: {
                TCB::dispatch();

                break;
            }
//            case THREAD_JOIN: {
//                TCB::join((thread_t *) args[0]);
//
//                break;
//            }
            case THREAD_GET_MY_ID: {
                returnVal = TCB::getMyId();
                w_sscratch(returnVal);

                break;
            }
            case SEM_OPEN: {
                returnVal = mySemaphore::open((sem_t *) args[0], (unsigned) args[1]);
                w_sscratch(returnVal);

                break;
            }
            case SEM_CLOSE: {
                returnVal = mySemaphore::close((sem_t) args[0]);
                w_sscratch(returnVal);

                break;
            }
            case SEM_WAIT: {
                returnVal = mySemaphore::wait((sem_t) args[0]);
                w_sscratch(returnVal);

                break;
            }
            case SEM_SIGNAL: {
                returnVal = mySemaphore::signal((sem_t) args[0]);
                w_sscratch(returnVal);

                break;
            }
            case SEM_TRYWAIT: {
                returnVal = mySemaphore::trywait((sem_t) args[0]);
                w_sscratch(returnVal);

                break;
            }
            case GETC: {
                returnVal = __getc();
                w_sscratch(returnVal);

                break;
            }
            case PUTC: {
                __putc((char) args[0]);

                break;
            }
            default: {
                break;
            }
        }

        w_sstatus(sstatus);
        w_sepc(sepc);
    } else if (scause == 0x8000000000000001UL) {
        // Interrupt: yes
        // Software interrupt from the third, most privileged mode of processor operation
        mc_sip(SSTATUS_SIE);
    } else if (scause == 0x8000000000000009UL) {
        // Interrupt: yes
        // Supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
    } else {
        // Unexpected trap cause
        printString("ERROR: scause ");
        printInt(scause);
        printString("\n");
        while (true) {}
    }
}