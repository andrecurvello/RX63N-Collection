/*
 * main.c
 *
 *  Created on: 27/01/2017
 *      Author: Miguel
 */
#include "platform.h"
#include <rtos_inc.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
	OS_TCB tcb;
	CPU_STK_SIZE stacksize;
	CPU_STK * stack;
} process_t;

#define PROCESS_STACKSIZE 128
#define PROCESS_DEFAULTPRIORITY 5

void spawn_process(char * proc_name, OS_TASK_PTR task_ptr) {
	OS_ERR err;

	process_t * proc = (process_t*)malloc(sizeof(process_t));
	proc->stack = (CPU_STK*)malloc(sizeof(CPU_STK) * PROCESS_STACKSIZE);
	proc->stacksize = PROCESS_STACKSIZE;

	OSTaskCreate((OS_TCB     *) &proc->tcb,
                 (CPU_CHAR   *) proc_name,
                 (OS_TASK_PTR ) task_ptr,
                 (void       *) 0,
                 (OS_PRIO     ) PROCESS_DEFAULTPRIORITY,
                 (CPU_STK    *) &proc->stack[0],
                 (CPU_STK_SIZE) proc->stacksize / 10u,
                 (CPU_STK_SIZE) proc->stacksize,
                 (OS_MSG_QTY  ) 0u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      ) (OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
}

void thread1(void * args);

OS_TMR tmr;

int i = 0;
void cback(void * args) {
	printf("\nCBACK %d", i++);
}

void kmain(void * args) {
	OS_ERR err;
	OSTmrCreate(&tmr, (CPU_CHAR*)"T1", 1, 1000, OS_OPT_TMR_PERIODIC, (OS_TMR_CALLBACK_PTR)cback, 0, &err);
	OSTmrStart(&tmr, &err);

	spawn_process("Thread 1", thread1);
	spawn_process("Thread 1 copy", thread1);

	int i = 0;
    while (1) {
    	printf("\nRTOS Working! %d", i++);
        OSTimeDlyHMSM(0, 0, 0, 1000, OS_OPT_TIME_HMSM_NON_STRICT, &err);
    }
}

void thread1(void * args) {
	OS_ERR err_os;
	int i = 0;
	while(1) {
		printf("\nThread 1: %d", i++);
		OSTimeDlyHMSM(0, 0, 0, 500, OS_OPT_TIME_HMSM_NON_STRICT, &err_os);
	}
}
