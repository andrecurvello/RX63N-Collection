################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\r_bsp/board/rdkrx63n/dbsct.c \
..\r_bsp/board/rdkrx63n/hwsetup.c \
..\r_bsp/board/rdkrx63n/lowlvl.c \
..\r_bsp/board/rdkrx63n/lowsrc.c \
..\r_bsp/board/rdkrx63n/resetprg.c \
..\r_bsp/board/rdkrx63n/sbrk.c \
..\r_bsp/board/rdkrx63n/vecttbl.c 

C_DEPS += \
./r_bsp/board/rdkrx63n/dbsct.d \
./r_bsp/board/rdkrx63n/hwsetup.d \
./r_bsp/board/rdkrx63n/lowlvl.d \
./r_bsp/board/rdkrx63n/lowsrc.d \
./r_bsp/board/rdkrx63n/resetprg.d \
./r_bsp/board/rdkrx63n/sbrk.d \
./r_bsp/board/rdkrx63n/vecttbl.d 

OBJS += \
./r_bsp/board/rdkrx63n/dbsct.obj \
./r_bsp/board/rdkrx63n/hwsetup.obj \
./r_bsp/board/rdkrx63n/lowlvl.obj \
./r_bsp/board/rdkrx63n/lowsrc.obj \
./r_bsp/board/rdkrx63n/resetprg.obj \
./r_bsp/board/rdkrx63n/sbrk.obj \
./r_bsp/board/rdkrx63n/vecttbl.obj 


# Each subdirectory must supply rules for building sources it contributes
r_bsp/board/rdkrx63n/%.obj: ../r_bsp/board/rdkrx63n/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	scandep1 -MM -MP -MF"$(@:%.obj=%.d)" -MT"$(@:%.obj=%.obj)" -MT"$(@:%.obj=%.d)"   -I"C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\r_bsp\mcu\rx63n\register_access" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\BSP" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Source\config" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-CPU" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-CPU\RX\RXC" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-LIB" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Ports\Renesas\RX\RXC" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Source" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\r_bsp" -I"C:\Users\Miguel\e2_studio\workspace\RTOS1\r_config" -D__RX=1   -D__RX600=1 -D__DBL8=1 -D__SCHAR=1  -D__LIT=1 -D__RON=1 -D__UBIT=1 -D__BITRIGHT=1 -D__DOFF=1 -D__INTRINSIC_LIB=1 -D__STDC_VERSION__=199901L  -D__RENESAS__=1 -D__RENESAS_VERSION__=0x02000000 -D__RX=1 -U_WIN32 -UWIN32 -U__WIN32__ -U__GNUC__ -U__GNUC_MINOR__ -U__GNUC_PATCHLEVEL__   -E -quiet -I. -C "$<"
	ccrx -lang=c99 -output=obj="$(@:%.d=%.obj)"  -include="C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium","C:\Users\Miguel\e2_studio\workspace\RTOS1\r_bsp\mcu\rx63n\register_access","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\BSP","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Source\config","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-CPU","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-CPU\RX\RXC","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uC-LIB","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Ports\Renesas\RX\RXC","C:\Users\Miguel\e2_studio\workspace\RTOS1\micrium\uCOS-III\Source","C:\Users\Miguel\e2_studio\workspace\RTOS1\r_bsp","C:\Users\Miguel\e2_studio\workspace\RTOS1\r_config"  -debug -nomessage -cpu=rx600 -dbl_size=8 -signed_char -nologo  -define=__RX=1   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

