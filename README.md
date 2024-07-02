# Description
Operating system project done at a course called Operating Systems 1 at School of Electrical Engineering, Belgrade.

It was built completely from scratch in C/C++ without any libraries, put together to work on a RISC-V 64-bit processor using a host XV6 system, with minimal additional support.

## OS Functionalities:
* C user interface
* C++ user interface
* Preemptive functionality
* FIFO memory allocation for heap
* multiple system and user threads
* FIFO scheduling algorithm
* periodic thread support

## Linux packages for starting the project:
* build-essential
* qemu-system-misc
* gcc-riscv64-linux-gnu
* binutils-riscv64-linux-gnu
* gdb-multiarch
* g++-riscv64-linux-gnu

## Make commands:
* make clean
* make all
* make qemu
* make qemu-gdb (for debugging)
