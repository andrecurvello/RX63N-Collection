################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\r_bsp/mcu/rx63n/cpu.c \
..\r_bsp/mcu/rx63n/locking.c \
..\r_bsp/mcu/rx63n/mcu_clocks.c \
..\r_bsp/mcu/rx63n/mcu_init.c \
..\r_bsp/mcu/rx63n/mcu_interrupts.c \
..\r_bsp/mcu/rx63n/mcu_locks.c 

C_DEPS += \
./r_bsp/mcu/rx63n/cpu.d \
./r_bsp/mcu/rx63n/locking.d \
./r_bsp/mcu/rx63n/mcu_clocks.d \
./r_bsp/mcu/rx63n/mcu_init.d \
./r_bsp/mcu/rx63n/mcu_interrupts.d \
./r_bsp/mcu/rx63n/mcu_locks.d 

OBJS += \
./r_bsp/mcu/rx63n/cpu.obj \
./r_bsp/mcu/rx63n/locking.obj \
./r_bsp/mcu/rx63n/mcu_clocks.obj \
./r_bsp/mcu/rx63n/mcu_init.obj \
./r_bsp/mcu/rx63n/mcu_interrupts.obj \
./r_bsp/mcu/rx63n/mcu_locks.obj 


# Each subdirectory must supply rules for building sources it contributes
r_bsp/mcu/rx63n/%.obj: ../r_bsp/mcu/rx63n/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	scandep1 -MM -MP -MF"$(@:%.obj=%.d)" -MT"$(@:%.obj=%.obj)" -MT"$(@:%.obj=%.d)"   -I"C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\cmt" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\fat" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\iostreams" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\mmc" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\r_rspi_rx" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\r_rspi_rx\src" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\r_bsp" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\r_config" -D__RX=1   -D__RX600=1 -D__DBL8=1 -D__SCHAR=1  -D__LIT=1 -D__RON=1 -D__UBIT=1 -D__BITRIGHT=1 -D__DOFF=1 -D__INTRINSIC_LIB=1 -D__STDC_VERSION__=199901L  -D__RENESAS__=1 -D__RENESAS_VERSION__=0x02000000 -D__RX=1 -U_WIN32 -UWIN32 -U__WIN32__ -U__GNUC__ -U__GNUC_MINOR__ -U__GNUC_PATCHLEVEL__   -E -quiet -I. -C "$<"
	ccrx -lang=c99 -output=obj="$(@:%.d=%.obj)"  -include="C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\cmt","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\fat","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\iostreams","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\mmc","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\r_rspi_rx","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\r_rspi_rx\src","C:\Users\Miguel\e2_studio\workspace\SDCard\r_bsp","C:\Users\Miguel\e2_studio\workspace\SDCard\r_config"  -debug -nomessage -cpu=rx600 -dbl_size=8 -signed_char -nologo  -define=__RX=1   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

