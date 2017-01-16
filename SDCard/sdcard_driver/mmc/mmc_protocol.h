/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2012 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name    : mmc_protocol.h
* Version      : 1.11  
* Device(s)    : Renesas RX
* Tool-Chain   : Renesas RX Standard Toolchain  
* OS           : None 
* H/W Platform : any
* Description  : Basic MMC driver definitions and declarations
*******************************************************************************/
/*******************************************************************************
* History      : DD.MM.YYYY Version Description
*              :            1.0    First Release
*              :            1.1 added support for SDcard v2.0  initialization
*              :            1.11 updated to new coding standard
******************************************************************************/

#ifndef MMC_PROTOCOL_H          // multiple inclusion guard 
#define MMC_PROTOCOL_H

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
typedef enum
{
    SDC_VALID = 0,        // No error
    SDC_COMM_INIT_FAIL,   // Could not establish communications with card
    SDC_INIT_FAIL,        // Card failed to initialize
    SDC_INIT_TIMEOUT,     // Card initialization timeout
    SDC_TYPE_INVALID,     // Card type could not be defined
    SDC_BAD_CMD,          // Card did not recognize command
    SDC_CARD_TIMEOUT,     // Card timeout during read, write, or erase
    SDC_READ_CRC_ERR,     // CRC error during read
    SDC_DATA_REJECTED,    // The CRC did not match 
    SDC_ERASE_TIMEOUT     // Erase sequence timed out
}sdc_error_t;

/******************************************************************************
Configuration Options
******************************************************************************/
/* Choose which RSPI channel should be used to communicate with the SD Card.
   On the YRDKRX63N board it is channel 0. */
#define SDCARD_RSPI_CHANNEL RSPI_CHANNEL_0

/* The Process ID to use when locking the RSPI peripheral. */
#define SDCARD_RSPI_PID       (0x00000000) 

/******************************************************************************
Macros
******************************************************************************/
#define SDC_SECTOR_SIZE        512 

/*******************************************************************************
* Exported global variables and functions (to be accessed by other files)
*******************************************************************************/
uint8_t R_SdCardSlotInit(void);
sdc_error_t R_SdcSectorRead(uint32_t sector_addr, uint8_t* buffer);
sdc_error_t R_SdcBlockWrite(uint8_t* buffer, uint32_t sector_addr, uint8_t sector_count);

#endif //multiple inclusion
