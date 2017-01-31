#include "platform.h"
#include <stdio.h>
#include <rtos_inc.h>

#define KMAIN_STACKSIZE 128
static  CPU_STK mainStk[KMAIN_STACKSIZE];
static  OS_TCB  mainTCB;

extern void kmain(void * p_arg);

void main(void) {
	OS_ERR err;

	R_BSP_InterruptsDisable();

	OSInit(&err);

	BSP_Init();
	CPU_Init();
	Mem_Init();

	OSTaskCreate((OS_TCB     *)&mainTCB,
				(CPU_CHAR   *)"Main",
				(OS_TASK_PTR ) kmain,
				(void       *) 0,
				(OS_PRIO     ) 10,
				(CPU_STK    *)&mainStk[0],
				(CPU_STK_SIZE) KMAIN_STACKSIZE / 10u,
				(CPU_STK_SIZE) KMAIN_STACKSIZE,
				(OS_MSG_QTY  ) 0u,
				(OS_TICK     ) 0u,
				(void       *) 0,
				(OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
				(OS_ERR     *)&err);

	OSStart(&err);
	while(1);
}
