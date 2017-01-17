#include <stdio.h>
#include <string.h>
#include "suart/suart.h"
#include "lcd.h"

#define TX GPIO_PORT_2_PIN_4
#define RX GPIO_PORT_E_PIN_3

char msg[10] = {'\0'};
int charidx = 0;

void rx_cback(uint8_t byte, uint8_t channel) {
	if(charidx < sizeof(msg)-1) {
		if(byte == 127 && charidx > 0) {
			msg[--charidx] = '\0';
		} else {
			msg[charidx++] = byte;
			msg[charidx] = '\0';
		}
	} else {
		charidx = 0;
	}

	if(byte == 13) {
		charidx = 0;
		msg[0] = '\0';
	}

	lcd_display(LCD_LINE1, msg);

	suart_tx(byte, channel);
}

void main(void) {
	lcd_initialize();
	lcd_clear();
	suart_init(rx_cback, 9600, 0, TX, RX);
	//for(;;) suart_tx('a',0);
	for(;;);
}
