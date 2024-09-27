.global pushRegisters
.global popRegisters

pushRegisters:
    # Adjust stack pointer to make space for 32 registers (each 8 bytes)
    addi sp, sp, -256

    # Save all 32 registers to the stack
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    sd x\index, \index * 8(sp)
    .endr

    ret

popRegisters:
    # Restore all 32 registers from the stack
    .irp index, 0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
    ld x\index, \index * 8(sp)
    .endr

    # Adjust stack pointer back
    addi sp, sp, 256

    ret
