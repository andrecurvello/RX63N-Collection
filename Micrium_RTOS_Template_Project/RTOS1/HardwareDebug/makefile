################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf *.sni *.dbg *.lst *.lis *.lpp *.map *.x *.lbp src/*.src src/*.p src/*.pp src/*.mrc *.bls *.libelf *.lbk *.abs *.fsy

# All of the sources participating in the build are defined here
-include sources.mk
-include micrium/BSP/subdir.mk
-include micrium/subdir.mk
-include micrium/uC-CPU/subdir.mk
-include micrium/uC-LIB/subdir.mk
-include micrium/uCOS-III/Ports/Renesas/RX/RXC/subdir.mk
-include micrium/uCOS-III/Source/subdir.mk
-include r_bsp/board/rdkrx63n/subdir.mk
-include r_bsp/mcu/all/subdir.mk
-include r_bsp/mcu/rx63n/subdir.mk
-include r_cmt_rx/src/subdir.mk
-include r_gpio_rx/src/subdir.mk
-include r_gpio_rx/src/targets/rx63n/subdir.mk
-include src/subdir.mk
-include src/suart/subdir.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(CC_UPPER_DEPS)),)
-include $(CC_UPPER_DEPS)
endif
ifneq ($(strip $(C++_UPPER_DEPS)),)
-include $(C++_UPPER_DEPS)
endif
ifneq ($(strip $(CXX_UPPER_DEPS)),)
-include $(CXX_UPPER_DEPS)
endif
ifneq ($(strip $(CPP_UPPER_DEPS)),)
-include $(CPP_UPPER_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(CP_DEPS)),)
-include $(CP_DEPS)
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
-include $(SRC_UPPER_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(SRC_DEPS)),)
-include $(SRC_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(CP_UPPER_DEPS)),)
-include $(CP_UPPER_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(OBJ_DEPS)),)
-include $(OBJ_DEPS)
endif
ifneq ($(strip $(ABS_DEPS)),)
-include $(ABS_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(O_DEPS)),)
-include $(O_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
endif

ifneq ($(strip $(SRC_UPPER_SRCS)),)
ALL_ASMS += $(SRC_UPPER_SRCS) 
endif
ifneq ($(strip $(SRC_SRCS)),)
ALL_ASMS += $(SRC_SRCS) 
endif
ifneq ($(strip $(S_UPPER_SRCS)),)
ALL_ASMS += $(S_UPPER_SRCS) 
endif
ifneq ($(strip $(S_SRCS)),)
ALL_ASMS += $(S_SRCS) 
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
CONVERTOR_OUTPUT_OUTPUTS += \
RTOS1.mot \

LINKER_OUTPUT_OUTPUTS += \
RTOS1.abs \

LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS += \
RTOS1.lib \
RTOS1.lib \

# All Target
all: RTOS1.mot
	@echo 'Build complete.'

# Tool invocations
RTOS1.mot: $(LINKER_OUTPUT_OUTPUTS)
	@echo 'Invoking: Converter'
	rlink   "C:\Users\Miguel\e2_studio\workspace\RTOS1\HardwareDebug\RTOS1.abs" -form=stype  -output="C:\Users\Miguel\e2_studio\workspace\RTOS1\HardwareDebug\RTOS1.mot" -nomessage
	@echo 'Finished building target:'
	@echo.
	$(MAKE) --no-print-directory post-build

RTOS1.abs: $(OBJS) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(ALL_ASMS) 
	@echo 'Invoking: Linker'
	@echo 'Building target:'
	rlink  -subcommand="LinkerSubCommand.tmp"  -library="C:\Users\Miguel\E2_STU~1\WORKSP~1\RTOS1\HARDWA~2\RTOS1.lib" -noprelink -list="RTOS1.map" -show -nooptimize -debug -output="C:\Users\Miguel\e2_studio\workspace\RTOS1\HardwareDebug\RTOS1.abs" -rom=D=R,D_1=R_1,D_2=R_2 -nocompress -vect=_undefined_interrupt_source_isr -nomessage -nologo
	@echo 'Finished building:'
	@echo.

RTOS1.lib:  
	@echo 'Invoking: Standard Library'
	lbgrx -lang=c99 -head=runtime,stdio,stdlib,string,new -cpu=rx600 -dbl_size=8 -signed_char -output="RTOS1.lib"
	@echo 'Finished building:'
	@echo.

# Other Targets
clean:
ifneq ($(strip $(OBJS)),)
	-$(file > clean_OBJS.tmp , $(OBJS))
	-@ xargs -s 6000 -t -a clean_OBJS.tmp $(RM)
	-@$(RM) clean_OBJS.tmp
endif
ifneq ($(strip $(C_DEPS)),)
	-$(file > clean_C_DEPS.tmp , $(C_DEPS))
	-@ xargs -s 6000 -t -a clean_C_DEPS.tmp $(RM)
	-@$(RM) clean_C_DEPS.tmp
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
	-$(file > clean_C_UPPER_DEPS.tmp , $(C_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_C_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_C_UPPER_DEPS.tmp
endif
ifneq ($(strip $(CPP_DEPS)),)
	-$(file > clean_CPP_DEPS.tmp , $(CPP_DEPS))
	-@ xargs -s 6000 -t -a clean_CPP_DEPS.tmp $(RM)
	-@$(RM) clean_CPP_DEPS.tmp
endif
ifneq ($(strip $(CPP_UPPER_DEPS)),)
	-$(file > clean_CPP_UPPER_DEPS.tmp , $(CPP_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_CPP_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_CPP_UPPER_DEPS.tmp
endif
ifneq ($(strip $(C++_DEPS)),)
	-$(file > clean_C++_DEPS.tmp , $(C++_DEPS))
	-@ xargs -s 6000 -t -a clean_C++_DEPS.tmp $(RM)
	-@$(RM) clean_C++_DEPS.tmp
endif
ifneq ($(strip $(C++_UPPER_DEPS)),)
	-$(file > clean_C++_UPPER_DEPS.tmp , $(C++_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_C++_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_C++_UPPER_DEPS.tmp
endif
ifneq ($(strip $(CP_DEPS)),)
	-$(file > clean_CP_DEPS.tmp , $(CP_DEPS))
	-@ xargs -s 6000 -t -a clean_CP_DEPS.tmp $(RM)
	-@$(RM) clean_CP_DEPS.tmp
endif
ifneq ($(strip $(CP_UPPER_DEPS)),)
	-$(file > clean_CP_UPPER_DEPS.tmp , $(CP_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_CP_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_CP_UPPER_DEPS.tmp
endif
ifneq ($(strip $(CXX_DEPS)),)
	-$(file > clean_CXX_DEPS.tmp , $(CXX_DEPS))
	-@ xargs -s 6000 -t -a clean_CXX_DEPS.tmp $(RM)
	-@$(RM) clean_CXX_DEPS.tmp
endif
ifneq ($(strip $(CXX_UPPER_DEPS)),)
	-$(file > clean_CXX_UPPER_DEPS.tmp , $(CXX_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_CXX_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_CXX_UPPER_DEPS.tmp
endif
ifneq ($(strip $(CC_DEPS)),)
	-$(file > clean_CC_DEPS.tmp , $(CC_DEPS))
	-@ xargs -s 6000 -t -a clean_CC_DEPS.tmp $(RM)
	-@$(RM) clean_CC_DEPS.tmp
endif
ifneq ($(strip $(CC_UPPER_DEPS)),)
	-$(file > clean_CC_UPPER_DEPS.tmp , $(CC_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_CC_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_CC_UPPER_DEPS.tmp
endif
ifneq ($(strip $(ASM_PRNS)),)
	-$(file > clean_ASM_PRNS.tmp , $(ASM_PRNS))
	-@ xargs -s 6000 -t -a clean_ASM_PRNS.tmp $(RM)
	-@$(RM) clean_ASM_PRNS.tmp
endif
ifneq ($(strip $(OBJ_DEPS)),)
	-$(file > clean_OBJ_DEPS.tmp , $(OBJ_DEPS))
	-@ xargs -s 6000 -t -a clean_OBJ_DEPS.tmp $(RM)
	-@$(RM) clean_OBJ_DEPS.tmp
endif
ifneq ($(strip $(ABS_DEPS)),)
	-$(file > clean_ABS_DEPS.tmp , $(ABS_DEPS))
	-@ xargs -s 6000 -t -a clean_ABS_DEPS.tmp $(RM)
	-@$(RM) clean_ABS_DEPS.tmp
endif
ifneq ($(strip $(O_DEPS)),)
	-$(file > clean_O_DEPS.tmp , $(O_DEPS))
	-@ xargs -s 6000 -t -a clean_O_DEPS.tmp $(RM)
	-@$(RM) clean_O_DEPS.tmp
endif
ifneq ($(strip $(S_DEPS)),)
	-$(file > clean_S_DEPS.tmp , $(S_DEPS))
	-@ xargs -s 6000 -t -a clean_S_DEPS.tmp $(RM)
	-@$(RM) clean_S_DEPS.tmp
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
	-$(file > clean_S_UPPER_DEPS.tmp , $(S_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_S_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_S_UPPER_DEPS.tmp
endif
ifneq ($(strip $(SRC_DEPS)),)
	-$(file > clean_SRC_DEPS.tmp , $(SRC_DEPS))
	-@ xargs -s 6000 -t -a clean_SRC_DEPS.tmp $(RM)
	-@$(RM) clean_SRC_DEPS.tmp
endif
ifneq ($(strip $(SRC_UPPER_DEPS)),)
	-$(file > clean_SRC_UPPER_DEPS.tmp , $(SRC_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_SRC_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_SRC_UPPER_DEPS.tmp
endif
ifneq ($(strip $(ASM_DEPS)),)
	-$(file > clean_ASM_DEPS.tmp , $(ASM_DEPS))
	-@ xargs -s 6000 -t -a clean_ASM_DEPS.tmp $(RM)
	-@$(RM) clean_ASM_DEPS.tmp
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
	-$(file > clean_ASM_UPPER_DEPS.tmp , $(ASM_UPPER_DEPS))
	-@ xargs -s 6000 -t -a clean_ASM_UPPER_DEPS.tmp $(RM)
	-@$(RM) clean_ASM_UPPER_DEPS.tmp
endif
	-$(RM) $(LIBRARY_GENERATOR_OUTPUTTYPE_OUTPUTS) $(CONVERTOR_OUTPUT_OUTPUTS) 
	-rm -rf "micrium/BSP"/*.obj "micrium/BSP"/*.d "micrium/BSP"/*.src "micrium/BSP"/*.p "micrium/BSP"/*.pp "micrium/BSP"/*.mrc "micrium/BSP"/*.prn "micrium/BSP"/*.asm "micrium/BSP"/*.lst
	-rm -rf "micrium"/*.obj "micrium"/*.d "micrium"/*.src "micrium"/*.p "micrium"/*.pp "micrium"/*.mrc "micrium"/*.prn "micrium"/*.asm "micrium"/*.lst
	-rm -rf "micrium/uC-CPU"/*.obj "micrium/uC-CPU"/*.d "micrium/uC-CPU"/*.src "micrium/uC-CPU"/*.p "micrium/uC-CPU"/*.pp "micrium/uC-CPU"/*.mrc "micrium/uC-CPU"/*.prn "micrium/uC-CPU"/*.asm "micrium/uC-CPU"/*.lst
	-rm -rf "micrium/uC-LIB"/*.obj "micrium/uC-LIB"/*.d "micrium/uC-LIB"/*.src "micrium/uC-LIB"/*.p "micrium/uC-LIB"/*.pp "micrium/uC-LIB"/*.mrc "micrium/uC-LIB"/*.prn "micrium/uC-LIB"/*.asm "micrium/uC-LIB"/*.lst
	-rm -rf "micrium/uCOS-III/Ports/Renesas/RX/RXC"/*.obj "micrium/uCOS-III/Ports/Renesas/RX/RXC"/*.d "micrium/uCOS-III/Ports/Renesas/RX/RXC"/*.src "micrium/uCOS-III/Ports/Renesas/RX/RXC"/*.p "micrium/uCOS-III/Ports/Renesas/RX/RXC"/*.pp "micrium/uCOS-III/Ports/Renesas/RX/RXC"/*.mrc "micrium/uCOS-III/Ports/Renesas/RX/RXC"/*.prn "micrium/uCOS-III/Ports/Renesas/RX/RXC"/*.asm "micrium/uCOS-III/Ports/Renesas/RX/RXC"/*.lst
	-rm -rf "micrium/uCOS-III/Source"/*.obj "micrium/uCOS-III/Source"/*.d "micrium/uCOS-III/Source"/*.src "micrium/uCOS-III/Source"/*.p "micrium/uCOS-III/Source"/*.pp "micrium/uCOS-III/Source"/*.mrc "micrium/uCOS-III/Source"/*.prn "micrium/uCOS-III/Source"/*.asm "micrium/uCOS-III/Source"/*.lst
	-rm -rf "r_bsp/board/rdkrx63n"/*.obj "r_bsp/board/rdkrx63n"/*.d "r_bsp/board/rdkrx63n"/*.src "r_bsp/board/rdkrx63n"/*.p "r_bsp/board/rdkrx63n"/*.pp "r_bsp/board/rdkrx63n"/*.mrc "r_bsp/board/rdkrx63n"/*.prn "r_bsp/board/rdkrx63n"/*.asm "r_bsp/board/rdkrx63n"/*.lst
	-rm -rf "r_bsp/mcu/all"/*.obj "r_bsp/mcu/all"/*.d "r_bsp/mcu/all"/*.src "r_bsp/mcu/all"/*.p "r_bsp/mcu/all"/*.pp "r_bsp/mcu/all"/*.mrc "r_bsp/mcu/all"/*.prn "r_bsp/mcu/all"/*.asm "r_bsp/mcu/all"/*.lst
	-rm -rf "r_bsp/mcu/rx63n"/*.obj "r_bsp/mcu/rx63n"/*.d "r_bsp/mcu/rx63n"/*.src "r_bsp/mcu/rx63n"/*.p "r_bsp/mcu/rx63n"/*.pp "r_bsp/mcu/rx63n"/*.mrc "r_bsp/mcu/rx63n"/*.prn "r_bsp/mcu/rx63n"/*.asm "r_bsp/mcu/rx63n"/*.lst
	-rm -rf "r_cmt_rx/src"/*.obj "r_cmt_rx/src"/*.d "r_cmt_rx/src"/*.src "r_cmt_rx/src"/*.p "r_cmt_rx/src"/*.pp "r_cmt_rx/src"/*.mrc "r_cmt_rx/src"/*.prn "r_cmt_rx/src"/*.asm "r_cmt_rx/src"/*.lst
	-rm -rf "r_gpio_rx/src"/*.obj "r_gpio_rx/src"/*.d "r_gpio_rx/src"/*.src "r_gpio_rx/src"/*.p "r_gpio_rx/src"/*.pp "r_gpio_rx/src"/*.mrc "r_gpio_rx/src"/*.prn "r_gpio_rx/src"/*.asm "r_gpio_rx/src"/*.lst
	-rm -rf "r_gpio_rx/src/targets/rx63n"/*.obj "r_gpio_rx/src/targets/rx63n"/*.d "r_gpio_rx/src/targets/rx63n"/*.src "r_gpio_rx/src/targets/rx63n"/*.p "r_gpio_rx/src/targets/rx63n"/*.pp "r_gpio_rx/src/targets/rx63n"/*.mrc "r_gpio_rx/src/targets/rx63n"/*.prn "r_gpio_rx/src/targets/rx63n"/*.asm "r_gpio_rx/src/targets/rx63n"/*.lst
	-rm -rf "src"/*.obj "src"/*.d "src"/*.src "src"/*.p "src"/*.pp "src"/*.mrc "src"/*.prn "src"/*.asm "src"/*.lst
	-rm -rf "src/suart"/*.obj "src/suart"/*.d "src/suart"/*.src "src/suart"/*.p "src/suart"/*.pp "src/suart"/*.mrc "src/suart"/*.prn "src/suart"/*.asm "src/suart"/*.lst
	@echo.
	@echo 'Clean complete.'

post-build:
	@echo.
	-C:\Renesas\E2_STU~1\DEBUGC~1\RENESA~1.EXE RTOS1.abs RTOS1.x 
	@echo.

.PHONY: all clean dependents
.SECONDARY: post-build

-include ../makefile.targets
