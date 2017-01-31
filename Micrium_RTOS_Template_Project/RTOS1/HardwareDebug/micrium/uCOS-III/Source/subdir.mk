################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\micrium/uCOS-III/Source/os_cfg_app.c \
..\micrium/uCOS-III/Source/os_core.c \
..\micrium/uCOS-III/Source/os_dbg.c \
..\micrium/uCOS-III/Source/os_flag.c \
..\micrium/uCOS-III/Source/os_int.c \
..\micrium/uCOS-III/Source/os_mem.c \
..\micrium/uCOS-III/Source/os_msg.c \
..\micrium/uCOS-III/Source/os_mutex.c \
..\micrium/uCOS-III/Source/os_pend_multi.c \
..\micrium/uCOS-III/Source/os_prio.c \
..\micrium/uCOS-III/Source/os_q.c \
..\micrium/uCOS-III/Source/os_sem.c \
..\micrium/uCOS-III/Source/os_stat.c \
..\micrium/uCOS-III/Source/os_task.c \
..\micrium/uCOS-III/Source/os_tick.c \
..\micrium/uCOS-III/Source/os_time.c \
..\micrium/uCOS-III/Source/os_tmr.c \
..\micrium/uCOS-III/Source/os_var.c 

C_DEPS += \
./micrium/uCOS-III/Source/os_cfg_app.d \
./micrium/uCOS-III/Source/os_core.d \
./micrium/uCOS-III/Source/os_dbg.d \
./micrium/uCOS-III/Source/os_flag.d \
./micrium/uCOS-III/Source/os_int.d \
./micrium/uCOS-III/Source/os_mem.d \
./micrium/uCOS-III/Source/os_msg.d \
./micrium/uCOS-III/Source/os_mutex.d \
./micrium/uCOS-III/Source/os_pend_multi.d \
./micrium/uCOS-III/Source/os_prio.d \
./micrium/uCOS-III/Source/os_q.d \
./micrium/uCOS-III/Source/os_sem.d \
./micrium/uCOS-III/Source/os_stat.d \
./micrium/uCOS-III/Source/os_task.d \
./micrium/uCOS-III/Source/os_tick.d \
./micrium/uCOS-III/Source/os_time.d \
./micrium/uCOS-III/Source/os_tmr.d \
./micrium/uCOS-III/Source/os_var.d 

OBJS += \
./micrium/uCOS-III/Source/os_cfg_app.obj \
./micrium/uCOS-III/Source/os_core.obj \
./micrium/uCOS-III/Source/os_dbg.obj \
./micrium/uCOS-III/Source/os_flag.obj \
./micrium/uCOS-III/Source/os_int.obj \
./micrium/uCOS-III/Source/os_mem.obj \
./micrium/uCOS-III/Source/os_msg.obj \
./micrium/uCOS-III/Source/os_mutex.obj \
./micrium/uCOS-III/Source/os_pend_multi.obj \
./micrium/uCOS-III/Source/os_prio.obj \
./micrium/uCOS-III/Source/os_q.obj \
./micrium/uCOS-III/Source/os_sem.obj \
./micrium/uCOS-III/Source/os_stat.obj \
./micrium/uCOS-III/Source/os_task.obj \
./micrium/uCOS-III/Source/os_tick.obj \
./micrium/uCOS-III/Source/os_time.obj \
./micrium/uCOS-III/Source/os_tmr.obj \
./micrium/uCOS-III/Source/os_var.obj 


# Each subdirectory must supply rules for building sources it contributes
micrium/uCOS-III/Source/%.obj: ../micrium/uCOS-III/Source/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	scandep1 -MM -MP -MF"$(@:%.obj=%.d)" -MT"$(@:%.obj=%.obj)" -MT"$(@:%.obj=%.d)"   -I"C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\r_bsp\mcu\rx63n\register_access" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\BSP" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Source\config" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-CPU" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-CPU\RX\RXC" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-LIB" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Ports\Renesas\RX\RXC" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Source" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\r_bsp" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\r_cmt_rx" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\r_cmt_rx\src" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\r_gpio_rx" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\r_gpio_rx\src" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\r_config" -D__RX=1   -D__RX600=1 -D__DBL8=1 -D__SCHAR=1  -D__LIT=1 -D__RON=1 -D__UBIT=1 -D__BITRIGHT=1 -D__DOFF=1 -D__INTRINSIC_LIB=1 -D__STDC_VERSION__=199901L  -D__RENESAS__=1 -D__RENESAS_VERSION__=0x02000000 -D__RX=1 -U_WIN32 -UWIN32 -U__WIN32__ -U__GNUC__ -U__GNUC_MINOR__ -U__GNUC_PATCHLEVEL__   -E -quiet -I. -C "$<"
	ccrx -lang=c99 -output=obj="$(@:%.d=%.obj)"  -include="C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include","C:\Users\Miguel\e2_studio\workspace\RTOS1\r_bsp\mcu\rx63n\register_access","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\BSP","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Source\config","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-CPU","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-CPU\RX\RXC","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-LIB","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Ports\Renesas\RX\RXC","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Source","C:\Users\Miguel\e2_studio\workspace\RTOS1\r_bsp","C:\Users\Miguel\e2_studio\workspace\RTOS1\r_cmt_rx","C:\Users\Miguel\e2_studio\workspace\RTOS1\r_cmt_rx\src","C:\Users\Miguel\e2_studio\workspace\RTOS1\r_gpio_rx","C:\Users\Miguel\e2_studio\workspace\RTOS1\r_gpio_rx\src","C:\Users\Miguel\e2_studio\workspace\RTOS1\r_config"  -debug -nomessage -cpu=rx600 -dbl_size=8 -signed_char -nologo  -define=__RX=1   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

