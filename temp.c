#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
void process_call_1(int*, intptr_t*);
int mul(int a, int b) {return (a*b);}

void process_call_1(int* result, intptr_t* args) {*result=mul((int)args[0], (int)args[1]);exit(0);}
int main() {
int total = 0;if (1){int a=7;int b=6;intptr_t _args_p1[2]={a, b};int r = 0; int* r_ptr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0); *r_ptr = 0; pid_t _process_r = fork(); if (_process_r == 0) { process_call_1(r_ptr, _args_p1); } pthread_t _thread_r;waitpid(_process_r, NULL, 0); r = *r_ptr;total=r;}printf("%d",total);
  return 0;
}
