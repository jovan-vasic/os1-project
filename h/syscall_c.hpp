#ifndef _SYSCALL_C_HPP
#define _SYSCALL_C_HPP

#include "../lib/hw.h"

using Body = void (*)(void *);

class TCB;
typedef TCB *thread_t;

class mySemaphore;
typedef mySemaphore *sem_t;

void *mem_alloc(size_t);

int mem_free(void *);

int thread_create(thread_t *, Body, void *arg);

int thread_exit();

void thread_dispatch();

int thread_get_my_id();

//void thread_join(thread_t* handle);

int sem_open(sem_t *, unsigned int);

int sem_close(sem_t);

int sem_wait(sem_t);

int sem_signal(sem_t);

int sem_trywait(sem_t);

int time_sleep(time_t);

const int EOF = -1;

char getc();

void putc(char);

#endif
