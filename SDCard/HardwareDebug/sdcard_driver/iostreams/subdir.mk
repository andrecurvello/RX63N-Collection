################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
..\sdcard_driver/iostreams/disk_iostreams.c \
..\sdcard_driver/iostreams/mmc_disk.c \
..\sdcard_driver/iostreams/r_diskio.c 

C_DEPS += \
./sdcard_driver/iostreams/disk_iostreams.d \
./sdcard_driver/iostreams/mmc_disk.d \
./sdcard_driver/iostreams/r_diskio.d 

OBJS += \
./sdcard_driver/iostreams/disk_iostreams.obj \
./sdcard_driver/iostreams/mmc_disk.obj \
./sdcard_driver/iostreams/r_diskio.obj 


# Each subdirectory must supply rules for building sources it contributes
sdcard_driver/iostreams/%.obj: ../sdcard_driver/iostreams/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	scandep1 -MM -MP -MF"$(@:%.obj=%.d)" -MT"$(@:%.obj=%.obj)" -MT"$(@:%.obj=%.d)"   -I"C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\cmt" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\fat" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\iostreams" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\mmc" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\r_rspi_rx" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\r_rspi_rx\src" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\r_bsp" -I"C:\Users\Miguel\e2_studio\workspace\SDCard\r_config" -D__RX=1   -D__RX600=1 -D__DBL8=1 -D__SCHAR=1  -D__LIT=1 -D__RON=1 -D__UBIT=1 -D__BITRIGHT=1 -D__DOFF=1 -D__INTRINSIC_LIB=1 -D__STDC_VERSION__=199901L  -D__RENESAS__=1 -D__RENESAS_VERSION__=0x02000000 -D__RX=1 -U_WIN32 -UWIN32 -U__WIN32__ -U__GNUC__ -U__GNUC_MINOR__ -U__GNUC_PATCHLEVEL__   -E -quiet -I. -C "$<"
	ccrx -lang=c99 -output=obj="$(@:%.d=%.obj)"  -include="C:\Renesas\E2_STU~1\Tools\Renesas\RX\2_0_0/include","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\cmt","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\fat","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\iostreams","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\mmc","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\r_rspi_rx","C:\Users\Miguel\e2_studio\workspace\SDCard\sdcard_driver\r_rspi_rx\src","C:\Users\Miguel\e2_studio\workspace\SDCard\r_bsp","C:\Users\Miguel\e2_studio\workspace\SDCard\r_config"  -debug -nomessage -cpu=rx600 -dbl_size=8 -signed_char -nologo  -define=__RX=1   "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

