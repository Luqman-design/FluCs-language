#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
void process_call_1(int*, intptr_t*);
void process_call_2(int*, intptr_t*);
int add(int a, int b) {return (a+b);}

void process_call_1(int* result, intptr_t* args) {*result=add((int)args[0], (int)args[1]);exit(0);}
void process_call_2(int* result, intptr_t* args) {*result=add((int)args[0], (int)args[1]);exit(0);}
int main() {
int total = 0;if (1){intptr_t _args_p1[2]={10, 5};int r1 = 0; int* r1_ptr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0); *r1_ptr = 0; pid_t _process_r1 = fork(); if (_process_r1 == 0) { process_call_1(r1_ptr, _args_p1); }intptr_t _args_p2[2]={20, 3};int r2 = 0; int* r2_ptr = mmap(NULL, sizeof(int), PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0); *r2_ptr = 0; pid_t _process_r2 = fork(); if (_process_r2 == 0) { process_call_2(r2_ptr, _args_p2); }waitpid(_process_r1, NULL, 0); r1 = *r1_ptr;waitpid(_process_r2, NULL, 0); r2 = *r2_ptr;total=(r1+r2);}printf("%d",total);
  return 0;
}
