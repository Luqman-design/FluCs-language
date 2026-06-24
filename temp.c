#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
int counter = 0;
pthread_mutex_t lock_counter;
void *for_loop_worker_1(void *arg) { intptr_t* _args = (intptr_t*)arg; int start_index = (int)_args[0] + 0; int* _counter = (int*)_args[1]; pthread_mutex_t* _lock_counter = (pthread_mutex_t*)_args[2]; for (int i = start_index; i < 100; i = i + 3) { pthread_mutex_lock(_lock_counter);printf("%d",i);pthread_mutex_lock(_lock_counter);(*_counter)=((*_counter)+i);pthread_mutex_unlock(_lock_counter);printf("%s","\n");pthread_mutex_unlock(_lock_counter);} return NULL; }int main() {
pthread_mutex_init(&lock_counter, NULL);
pthread_mutex_t _lock_counter;pthread_mutex_init(&_lock_counter, NULL);intptr_t _fargs[3][3]; for (int _fi = 0; _fi < 3; _fi++) { _fargs[_fi][0] = _fi; _fargs[_fi][1] = (intptr_t)&counter; _fargs[_fi][2] = (intptr_t)&_lock_counter; } pthread_t threads[3]; for (int i = 0; i < 3; i++) { pthread_create(&threads[i], NULL, for_loop_worker_1, _fargs[i]); } for (int i = 0; i < 3; i++) { pthread_join(threads[i], NULL); }pthread_mutex_destroy(&_lock_counter);
  return 0;
}
