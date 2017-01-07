#include <stdint.h>
#include <stdio.h>
#include "platform.h"
#include "r_gpio_rx_if.h"
#include "r_cmt_rx_if.h"
#include "lcd.h"

#define TRIG GPIO_PORT_D_PIN_5
#define ECHO GPIO_PORT_D_PIN_3

volatile bool enable_timer = false;
volatile uint32_t delta_time = 0;

void timer_cback(void * arg) {
	if(enable_timer) delta_time++;
	else delta_time = 0;
}

void delay_us(uint32_t timeout) {
	enable_timer = true;
	while(delta_time < timeout);
	enable_timer = false;
}

uint32_t pulseIn(gpio_port_pin_t read_pin, uint8_t trig_level, uint32_t timeout) {
	enable_timer = true;
	if(trig_level) {
		while(!R_GPIO_PinRead(read_pin)) if(delta_time >= timeout && timeout > 0 && (int32_t)timeout != -1) break;
		while(R_GPIO_PinRead(read_pin))  if(delta_time >= timeout && timeout > 0 && (int32_t)timeout != -1) break;
	} else {
		while(R_GPIO_PinRead(read_pin))  if(delta_time >= timeout && timeout > 0 && (int32_t)timeout != -1) break;
		while(!R_GPIO_PinRead(read_pin)) if(delta_time >= timeout && timeout > 0 && (int32_t)timeout != -1) break;
	}
	uint32_t ret = delta_time;
	enable_timer = false;
	return ret;
}

uint32_t hc_sr04_read(gpio_port_pin_t echo_pin) {
	#define MAX_DISTANCE 200

	R_GPIO_PinWrite(TRIG, GPIO_LEVEL_HIGH);
	delay_us(10);
	R_GPIO_PinWrite(TRIG, GPIO_LEVEL_LOW);
	uint32_t distance = (pulseIn(ECHO, 1, 0))/58.2-10;
	return distance >= MAX_DISTANCE ? MAX_DISTANCE : distance;
}

void main(void) {
	lcd_initialize();
	lcd_clear();

	R_GPIO_PinDirectionSet(ECHO, GPIO_DIRECTION_INPUT);
	R_GPIO_PinDirectionSet(TRIG, GPIO_DIRECTION_OUTPUT);
	R_GPIO_PinWrite(TRIG, GPIO_LEVEL_LOW);
	R_CMT_CreatePeriodic(1000000, timer_cback, (uint32_t*)&CMT0);

	for(;;) {
		/* Read HC-SR04 */
		uint32_t distance = hc_sr04_read(ECHO);

		char buff[50];
		sprintf(buff, "-> %d cm   ", distance);
		lcd_display(LCD_LINE1, (const uint8_t*)buff);
		delay_us(50000); /* Wait 50 ms */
	}
}
