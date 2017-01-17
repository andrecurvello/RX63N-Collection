/*
 * suart.c
 *
 *  Created on: 11/01/2017
 *      Author: Miguel
 */
#include "platform.h"
#include "r_cmt_rx_if.h"
#include "suart.h"

#define TIMER_FREQ 2000000 /* Frequency of the CMT timer */
#define TIMER_DIVIDER 4    /* We'll divide the timer trigger by this amount */

#define BAUD_TO_US(baudrate) (((uint32_t)((TIMER_FREQ / (baudrate)) / TIMER_DIVIDER)))

volatile uint32_t baudrates[MAX_SUART_CHANNEL_COUNT];
volatile uint32_t preload_polls[MAX_SUART_CHANNEL_COUNT];
volatile uint32_t delta_polls[MAX_SUART_CHANNEL_COUNT];

volatile rx_cback_t rx_cbacks[MAX_SUART_CHANNEL_COUNT];

volatile gpio_port_pin_t rx_pins[MAX_SUART_CHANNEL_COUNT];
volatile gpio_port_pin_t tx_pins[MAX_SUART_CHANNEL_COUNT];

typedef enum soft_uart_fsm {
	SUART_IDLE,
	SUART_RXING,
	SUART_TXING
} soft_uart_fsm_t;

volatile soft_uart_fsm_t suart_states[MAX_SUART_CHANNEL_COUNT];

volatile bool txing = false;
volatile bool rxing = false;

bool suart_initialized = false;

/**************/
/** TX LOGIC **/
/**************/

volatile uint8_t tx_bytes[MAX_SUART_CHANNEL_COUNT];

void suart_tx_poll(uint8_t polling_channel) {
	if(suart_states[polling_channel] == SUART_RXING) return; /* We're receiving. Do not transmit */

	if(suart_states[polling_channel] == SUART_TXING) {
		if(preload_polls[polling_channel]++ < BAUD_TO_US(baudrates[polling_channel])) return; /* Respect the Baud timings */
		preload_polls[polling_channel] = 0;

		switch(delta_polls[polling_channel]) {
		case 0: /* Send start bit */
			R_GPIO_PinWrite(tx_pins[polling_channel], GPIO_LEVEL_LOW);
			delta_polls[polling_channel]++;
			break;
		case 9: /* Send stop bit */
			R_GPIO_PinWrite(tx_pins[polling_channel], GPIO_LEVEL_HIGH);
			delta_polls[polling_channel]++;
			break;
		case 10:
			suart_states[polling_channel] = SUART_IDLE; /* Done transmitting */
			delta_polls[polling_channel]  = 0;
			break;
		default:
			/* Send 'nth' bit from byte 'tx_byte' */
			R_GPIO_PinWrite(tx_pins[polling_channel], (tx_bytes[polling_channel] & (1 << (delta_polls[polling_channel] - 1))) ? GPIO_LEVEL_HIGH : GPIO_LEVEL_LOW);
			delta_polls[polling_channel]++;
			break;
		}
	}
}

/**************/
/** RX LOGIC **/
/**************/

volatile uint8_t rx_bytes[MAX_SUART_CHANNEL_COUNT];

void suart_rx_poll(uint8_t polling_channel) {
	if(suart_states[polling_channel] == SUART_TXING) return; /* We're transmitting. Do not receive */

	if(suart_states[polling_channel] == SUART_RXING) {
		/* Respect the Baud timings: */
		if(preload_polls[polling_channel]++ < BAUD_TO_US(baudrates[polling_channel])) return;
		preload_polls[polling_channel] = 0;

		switch(delta_polls[polling_channel]) {
		case 8: /* Receive stop bit */
			suart_states[polling_channel] = SUART_IDLE;
			delta_polls[polling_channel]  = 0;

			if(R_GPIO_PinRead(rx_pins[polling_channel])) {
				/* We found a stop bit! */
				if(rx_cbacks[polling_channel])
					rx_cbacks[polling_channel](rx_bytes[polling_channel], polling_channel);
			} else {
				/* We didn't find a stop bit... Reception byte must be dropped */
			}
			rx_bytes[polling_channel] = 0;
			break;
		default: {
			/* Receive 'nth' bit into byte 'rx_byte' */
			uint8_t bit = R_GPIO_PinRead(rx_pins[polling_channel]);
			rx_bytes[polling_channel] |= (bit << delta_polls[polling_channel]);
			delta_polls[polling_channel]++;
			break;
		}
		}
	} else {
		/* Check for start bit: */
		if(!R_GPIO_PinRead(rx_pins[polling_channel])) {
			/* We've received a start bit! */
			suart_states[polling_channel] = SUART_RXING;
		}
	}
}

/************************************/
/************ SUART API *************/
/************************************/

enum SUART_ERRCODE suart_tx(uint8_t byte, uint8_t channel) {
	if(channel > MAX_SUART_CHANNEL_COUNT) return SUART_ERR_BADINIT;
	/* Wait for the current byte to be sent / received: */
	while(suart_states[channel] != SUART_IDLE);

	/* Send byte Asynchronously using the timer interrupt: */
	tx_bytes[channel]     = byte;
	suart_states[channel] = SUART_TXING;
	return SUART_OK;
}

enum SUART_ERRCODE suart_tx_sync(uint8_t byte, uint8_t channel) {
	if(channel > MAX_SUART_CHANNEL_COUNT) return SUART_ERR_BADINIT;
	suart_tx(byte, channel);
	while(suart_states[channel] != SUART_IDLE); /* Wait for the current byte to be sent / received */
	return SUART_OK;
}

enum SUART_ERRCODE suart_tx_buff(uint8_t * buff, uint32_t length, uint8_t channel) {
	if(channel > MAX_SUART_CHANNEL_COUNT) return SUART_ERR_BADINIT;
	for(uint32_t i = 0; i < length; i++)
		suart_tx(buff[i], channel);
	return SUART_OK;
}

enum SUART_ERRCODE suart_tx_buff_sync(uint8_t * buff, uint32_t length, uint8_t channel) {
	if(channel > MAX_SUART_CHANNEL_COUNT) return SUART_ERR_BADINIT;
	for(uint32_t i = 0; i < length; i++)
		suart_tx_sync(buff[i], channel);
	return SUART_OK;
}

void suart_timer_cback(void * args) {
#if ENABLE_TX || ENABLE_RX
	/* Handle TX and RX for all UART channels: */
	for(uint8_t i = 0; i < MAX_SUART_CHANNEL_COUNT; i++) {
		if(baudrates[i]) {
#if ENABLE_TX
			suart_tx_poll(i);
#endif
#if ENABLE_RX
			suart_rx_poll(i);
#endif
		}
	}
#endif
}

void suart_init_all_channels(void) {
	for(uint8_t i = 0; i < MAX_SUART_CHANNEL_COUNT; i++) {
		suart_states[i] = SUART_IDLE;
		rx_cbacks[i]    = 0;
		baudrates[i]    = 0;
		tx_bytes[i]     = 0;
		rx_bytes[i]     = 0;
	}

	/* Create timer for servicing UART via GPIOs: */
	R_CMT_CreatePeriodic(TIMER_FREQ / TIMER_DIVIDER, suart_timer_cback, (uint32_t*)&CMT0);
}

enum SUART_ERRCODE suart_init(rx_cback_t rx_cback, uint32_t baudrate, uint8_t channel, gpio_port_pin_t txpin, gpio_port_pin_t rxpin) {
	if(channel > MAX_SUART_CHANNEL_COUNT || baudrate == 0) return SUART_ERR_BADINIT;

	if(!suart_initialized) {
		suart_init_all_channels();
		suart_initialized = true;
	}

	/* Set Baud Rate for this particular channel: */
	baudrates[channel] = baudrate;

	/* Set RX callback: */
	rx_cbacks[channel] = rx_cback;

	/* Initialize RX and TX GPIO pins: */
	tx_pins[channel] = txpin;
	rx_pins[channel] = rxpin;
	R_GPIO_PinDirectionSet(rx_pins[channel], GPIO_DIRECTION_INPUT);
	R_GPIO_PinDirectionSet(tx_pins[channel], GPIO_DIRECTION_OUTPUT);
	R_GPIO_PinWrite(tx_pins[channel], GPIO_LEVEL_HIGH);

	return SUART_OK;
}
