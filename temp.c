#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
int t1 = 0;
int t2 = 0;
int result = 0;
pthread_mutex_t lock_result;
void add(int a, int b) {pthread_mutex_lock(&lock_result);result+=(a+b);pthread_mutex_unlock(&lock_result);}

void* thread_call_1(void* arg) {intptr_t* args=(intptr_t*)arg;add((int)args[0], (int)args[1]);return NULL;}
void* thread_call_2(void* arg) {intptr_t* args=(intptr_t*)arg;add((int)args[0], (int)args[1]);return NULL;}
int main() {
pthread_mutex_init(&lock_result, NULL);
pthread_t _thread_t1;intptr_t _args_t1[2]={4, 7};pthread_create(&_thread_t1,NULL,thread_call_1,_args_t1);pthread_t _thread_t2;intptr_t _args_t2[2]={20, 13};pthread_create(&_thread_t2,NULL,thread_call_2,_args_t2);pthread_join(_thread_t1, NULL);pthread_join(_thread_t2, NULL);printf("%d",result);
  return 0;
}
