/*
 * suart.h
 *
 *  Created on: 11/01/2017
 *      Author: Miguel
 */

#ifndef SUART_H_
#define SUART_H_

#include "r_gpio_rx_if.h"
#include <stdint.h>

#define ENABLE_TX (1)
#define ENABLE_RX (1)

#define MAX_SUART_CHANNEL_COUNT 3

typedef void (*rx_cback_t)(uint8_t byte, uint8_t channel);

enum SUART_ERRCODE {
	SUART_OK,
	SUART_ERR_BADINIT,
	SUART_ERR_COUNT
};

void suart_init_all_channels(void);
enum SUART_ERRCODE suart_init(rx_cback_t rx_cback, uint32_t baudrate, uint8_t channel, gpio_port_pin_t txpin, gpio_port_pin_t rxpin);

/* TX API Functions: */
enum SUART_ERRCODE suart_tx_buff(uint8_t * buff, uint32_t length, uint8_t channel);
enum SUART_ERRCODE suart_tx_buff_sync(uint8_t * buff, uint32_t length, uint8_t channel);
enum SUART_ERRCODE suart_tx(uint8_t byte, uint8_t channel);
enum SUART_ERRCODE suart_tx_sync(uint8_t byte, uint8_t channel);

/* RX API Functions: */
/* TODO */

#endif /* SUART_H_ */
