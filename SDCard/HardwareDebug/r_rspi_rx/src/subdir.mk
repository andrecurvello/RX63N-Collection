################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\r_rspi_rx/src/r_rspi_rx.c 

C_DEPS += \
./r_rspi_rx/src/r_rspi_rx.d 

OBJS += \
./r_rspi_rx/src/r_rspi_rx.obj 


# Each subdirectory must supply rules for building sources it contributes
r_rspi_rx/src/%.obj: ../r_rspi_rx/src/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	scandep1 -MM -MP -MF"$(@:%.obj=%.d)" -MT"$(@:%.obj=%.obj)" -MT"$(@:%.obj=%.d)"   -I"C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\r_bsp" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\r_config" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\r_rspi_rx" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\r_rspi_rx\src" -D__RX=1   -D__RX600=1 -D__DBL8=1 -D__SCHAR=1  -D__LIT=1 -D__RON=1 -D__UBIT=1 -D__BITRIGHT=1 -D__DOFF=1 -D__INTRINSIC_LIB=1 -D__STDC_VERSION__=199901L  -D__RENESAS__=1 -D__RENESAS_VERSION__=0x02000000 -D__RX=1 -U_WIN32 -UWIN32 -U__WIN32__ -U__GNUC__ -U__GNUC_MINOR__ -U__GNUC_PATCHLEVEL__   -E -quiet -I. -C "$<"
	ccrx -lang=c99 -output=obj="$(@:%.d=%.obj)"  -include="C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include","C:\Users\Miguel\e2_studio\workspace\SDCard\r_bsp","C:\Users\Miguel\e2_studio\workspace\SDCard\r_config","C:\Users\Miguel\e2_studio\workspace\SDCard\r_rspi_rx","C:\Users\Miguel\e2_studio\workspace\SDCard\r_rspi_rx\src"  -debug -nomessage -cpu=rx600 -dbl_size=8 -signed_char -nologo  -define=__RX=1   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

