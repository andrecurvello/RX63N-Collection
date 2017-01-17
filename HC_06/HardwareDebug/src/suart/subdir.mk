################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\src/suart/suart.c 

C_DEPS += \
./src/suart/suart.d 

OBJS += \
./src/suart/suart.obj 


# Each subdirectory must supply rules for building sources it contributes
src/suart/%.obj: ../src/suart/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	scandep1 -MM -MP -MF"$(@:%.obj=%.d)" -MT"$(@:%.obj=%.obj)" -MT"$(@:%.obj=%.d)"   -I"C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_glyph" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_glyph\src" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_glyph\src\glyph" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_glyph\src\glyph\drivers" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_glyph\src\glyph\fonts" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_rspi_rx600" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_rspi_rx600\src" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_bsp" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_cmt_rx" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_cmt_rx\src" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_gpio_rx" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_gpio_rx\src" -I"C:\Users\Miguel\e2_studio\workspace\HC_06\r_config" -D__RX=1   -D__RX600=1 -D__DBL8=1 -D__SCHAR=1  -D__LIT=1 -D__RON=1 -D__UBIT=1 -D__BITRIGHT=1 -D__DOFF=1 -D__INTRINSIC_LIB=1 -D__STDC_VERSION__=199901L  -D__RENESAS__=1 -D__RENESAS_VERSION__=0x02000000 -D__RX=1 -U_WIN32 -UWIN32 -U__WIN32__ -U__GNUC__ -U__GNUC_MINOR__ -U__GNUC_PATCHLEVEL__   -E -quiet -I. -C "$<"
	ccrx -lang=c99 -output=obj="$(@:%.d=%.obj)"  -include="C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include","C:\Users\Miguel\e2_studio\workspace\HC_06\r_glyph","C:\Users\Miguel\e2_studio\workspace\HC_06\r_glyph\src","C:\Users\Miguel\e2_studio\workspace\HC_06\r_glyph\src\glyph","C:\Users\Miguel\e2_studio\workspace\HC_06\r_glyph\src\glyph\drivers","C:\Users\Miguel\e2_studio\workspace\HC_06\r_glyph\src\glyph\fonts","C:\Users\Miguel\e2_studio\workspace\HC_06\r_rspi_rx600","C:\Users\Miguel\e2_studio\workspace\HC_06\r_rspi_rx600\src","C:\Users\Miguel\e2_studio\workspace\HC_06\r_bsp","C:\Users\Miguel\e2_studio\workspace\HC_06\r_cmt_rx","C:\Users\Miguel\e2_studio\workspace\HC_06\r_cmt_rx\src","C:\Users\Miguel\e2_studio\workspace\HC_06\r_gpio_rx","C:\Users\Miguel\e2_studio\workspace\HC_06\r_gpio_rx\src","C:\Users\Miguel\e2_studio\workspace\HC_06\r_config"  -debug -nomessage -cpu=rx600 -dbl_size=8 -signed_char -nologo  -define=__RX=1   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

