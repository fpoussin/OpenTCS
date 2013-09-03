CC=arm-none-eabi-gcc
AR=arm-none-eabi-ar

###########################################

FW_PATH = STM32F0xx_StdPeriph_Driver/src
BUILDDIR = build

CFLAGS  = -ggdb3 -Os -Wall -ffunction-sections -fdata-sections
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m0 -march=armv6s-m
CFLAGS += -ffreestanding -nostdlib
CFLAGS += -I. -ICMSIS/Include -ICMSIS/Device/ST/STM32F0xx/Include -ISTM32F0xx_StdPeriph_Driver/inc
CFLAGS += -Wl,--gc-sections

FW_SRCS =  $(FW_PATH)/stm32f0xx_adc.c $(FW_PATH)/stm32f0xx_cec.c $(FW_PATH)/stm32f0xx_comp.c $(FW_PATH)/stm32f0xx_crc.c \
	$(FW_PATH)/stm32f0xx_dac.c $(FW_PATH)/stm32f0xx_dbgmcu.c $(FW_PATH)/stm32f0xx_dma.c $(FW_PATH)/stm32f0xx_exti.c \
	$(FW_PATH)/stm32f0xx_flash.c $(FW_PATH)/stm32f0xx_gpio.c $(FW_PATH)/stm32f0xx_i2c.c $(FW_PATH)/stm32f0xx_iwdg.c \
	$(FW_PATH)/stm32f0xx_misc.c $(FW_PATH)/stm32f0xx_pwr.c $(FW_PATH)/stm32f0xx_rcc.c $(FW_PATH)/stm32f0xx_rtc.c \
	$(FW_PATH)/stm32f0xx_spi.c $(FW_PATH)/stm32f0xx_syscfg.c $(FW_PATH)/stm32f0xx_tim.c \
	$(FW_PATH)/stm32f0xx_usart.c $(FW_PATH)/stm32f0xx_wwdg.c

FW_OBJS = $(FW_SRCS:%.c=$(BUILDDIR)/%.o)

.PHONY: all

all: libstm32f0.a

$(FW_OBJS): $(BUILDDIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -o $@ $^

libstm32f0.a: $(FW_OBJS)
	@mkdir -p $(@D)
	$(AR) -r $@ $(FW_OBJS)

clean:
	rm -rf $(BUILDDIR)
	
mrproper:
	rm -rf $(BUILDDIR) libstm32f0.a
