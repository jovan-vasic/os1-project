.global _ZN5Riscv14supervisorTrapEv

.extern _ZN5Riscv20handleSupervisorTrapEv
.type _ZN5Riscv14supervisorTrapEv, @function

.align 4

_ZN5Riscv14supervisorTrapEv:
    # Push all registers to stack
    addi sp, sp, -256
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr

    # Call the trap handler
    call _ZN5Riscv20handleSupervisorTrapEv

    # Pop all registers from stack
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr

    csrr a0, sscratch

    addi sp, sp, 256

    # Return from trap
    sret