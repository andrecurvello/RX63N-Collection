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
* File Name    : mmc_protocol.c
* Version      : 1.2  
* Device(s)    : Renesas RX6xx
* Tool-Chain   : Renesas RX Standard Toolchain  
* OS           : None 
* H/W Platform : YRDKRX63N
* Description  : Basic MMC driver implementation
*              : Some algorithms and data structures derived from sample code  
*              : in book "USB Mass Storage" by Jan Axleson:
*              : Axelson, Jan. " USB Mass Storage, Designing and Programming 
*              :     Devices and Embedded Hosts". Lakeview Research. 1st Ed. 2006
*******************************************************************************
* History      : DD.MM.YYYY Version Description
*              :            1.0     First Release
*              :            1.1     added support for SDcard v2.0  initialization
*              :            1.11    updated to new coding standard
*              : 08.05.2012 1.2     FIT updates
*              : 17.09.2012 1.21    Changed SPI clock to slower rate for standard
*                                   capacity SDcard due to occasional read 
*                                   failures on old card types. High capacity 
*                                   still gets faster clock.
*                18.09.2012         modified to support changes to FIT r_rspi
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include <stdint.h>
#include <stdbool.h>

#include "sdcard.h"
#include "mmc_protocol.h"
#include "platform.h"
#include "r_rspi_rx_if.h"   /* RSPI package. */

#include "cmt_oneshot.h"

/******************************************************************************
Typedef definitions
******************************************************************************/
typedef union
{
    uint8_t uByte;
    struct
    {
        unsigned char IN_IDLE_STATE:1;
        unsigned char ERASE_RESET:1;
        unsigned char ILLEGAL_CMD:1;
        unsigned char CRC_ERR:1;
        unsigned char ERASE_SEQ_ERR:1;
        unsigned char ADDRESS_ERR:1;
        unsigned char PARAM_ERR:1;
        unsigned char B7:1;        
    }BIT;
}response1_t;

typedef union
{
    uint16_t _word;
    struct
    {
        uint8_t byte_0;
        uint8_t byte_1;    
    }BYTES;
    struct
    {
        unsigned char IN_IDLE_STATE:1;
        unsigned char ERASE_RESET:1;
        unsigned char ILLEGAL_CMD:1;
        unsigned char CRC_ERR:1;
        unsigned char ERASE_SEQ_ERR:1;
        unsigned char ADDRESS_ERR:1;
        unsigned char PARAM_ERR:1;
        unsigned char B7:1;
        unsigned char CARD_IS_LOCKED:1;
        unsigned char WP_ERASE_SKIP_LK_FAIL:1;
        unsigned char ERROR:1;
        unsigned char CC_ERROR:1;
        unsigned char CARD_ECC_FAIL:1;
        unsigned char WP_VIOLATION:1;
        unsigned char ERASE_PARAM:1;
        unsigned char OUTRANGE_CSD_OVERWRITE:1;
    } BIT;
}response2_t;

typedef struct
{
    response1_t r1;        
    uint8_t OCR3;           // OP Con register.
    uint8_t OCR2;
    uint8_t OCR1;
    uint8_t OCR0;            
}response3_t;

typedef union
{
    uint8_t uByte[5];
    struct
    {
        response1_t r1;        
        uint8_t RESERVED_H;
        uint8_t RESERVED_L;
        uint8_t V4;            // Only low 4 bits of this is voltage value.
        uint8_t CHK_PATTERN;
    }BYTES;
}response7_t;

typedef union
{
    struct
    {
        uint32_t uLONG_0;
        uint32_t uLONG_1;
        uint32_t uLONG_2;
        uint32_t uLONG_3;                        
    }DWORDS;
    struct
    {
        uint8_t uByte[16];
    }ARRAY;    
} csd_t;

typedef enum 
{
    GO_IDLE_STATE = 0,
    SEND_OP_COND,
    SEND_IF_COND,         // (CMD8) in V2.0 spec.    
    SEND_CSD,
    SEND_CID,
    STOP_TRANSMISSION,
    SEND_STATUS,
    SET_BLOCKLEN,
    READ_SINGLE_BLOCK,
    READ_MULTI_BLOCK,
    SET_BLOCK_COUNT,
    WRITE_SINGLE_BLOCK,
    WRITE_MULTI_BLOCK,
    TAG_SECTOR_START,
    TAG_SECTOR_END,
    UNTAG_SECTOR,
    TAG_ERASE_GRP_START,
    TAG_ERASE_GRP_END,
    UNTAG_ERASE_GRP,
    ERASE,    
    SD_APP_OP_COND,
    LOCK_UNLOCK,
    APP_CMD,
    READ_OCR,
    CRC_ON_OFF        
}sdmmc_cmd_t;

typedef enum
{
    R1 = 1,    //Basic MMC response format.
    R1b,       //R1 followed by a number of bytes indicating busy status.    
    R2,        //R2 response, used only by SEND_STATUS (CMD13).
    R3,        //R3 response, used by READ_OCR    (CMD58).
    R7
}resp_t;

typedef struct 
{
    uint8_t cmd_code;    
    uint8_t crc;
    resp_t response_type;
    uint8_t more_data_expected; 
}sdc_cmd_t;



/* Command structure:                                      */
/*  an MMC command is 48 bits or can be 6 bytes,           */
/*  or command byte + 4 address bytes + CRC,               */
/*  or command byte + 4 address bytes + 7bitCRC + end bit  */
/*  or command byte + 32 argument bits + 7bitCRC + end bit */
typedef union
{
    struct
    {
        uint8_t field[6];
    }FIELD;
    struct
    {
        uint8_t crc;
        uint8_t addr0; //LSB
        uint8_t addr1;
        uint8_t addr2;
        uint8_t addr3; //MSB
        uint8_t    cmd;
    }BYTES;
    struct
    {
        unsigned char END_BIT:1;
        unsigned char CRC7:7;
        uint8_t addr0; //LSB
        uint8_t addr1;
        uint8_t addr2;
        uint8_t addr3; //MSB
        uint8_t cmd;
    }CMD_ADD_CRC_EB;        
}cmd_packet_t;

typedef union _SDCstate
{
    struct
    {
        uint8_t isSDMMC    :1;    // Set for an SD card or multimedia card.
        uint8_t isWP    :1;    // Set if write protected.
    };
    uint8_t uByte;
}sdc_state_t;

/* sdc_response_t can contain either response1_t or response2_t structure */
typedef union
{
    uint8_t uByte[5];
    response1_t r1;
    response2_t r2;
    response3_t r3;
    response7_t r7;
}sdc_response_t;


/******************************************************************************
Macro definitions
******************************************************************************/
#define CSD_SIZE 16
#define DATA_START_TOKEN    0xFE /* mmc start block token */

#define MOREDATA !0
#define NODATA 0

/* To convert command to decimal CMD index as listed in spec,                */
/* subtract the command base value of 0x40 from hex command value, then      */
/* convert to decimal.                                                       */
/* So, for example: ACMD41 is SD_APP_OP_COND(0x69 - 0x40) = 0x29 = 41d       */
/*        command name            first byte    index */    
#define cmdGO_IDLE_STATE        0x40        //1
#define cmdSEND_OP_COND         0x41        //2
#define cmdSEND_IF_COND         0x48        //3
#define cmdSEND_CSD             0x49        //4
#define cmdSEND_CID             0x4a        //5
#define cmdSTOP_TRANSMISSION    0x4c        //6
#define cmdSEND_STATUS          0x4d        //7
#define cmdSET_BLOCKLEN         0x50        //8
#define cmdREAD_SINGLE_BLOCK    0x51        //9
#define cmdREAD_MULTI_BLOCK     0x52        //10
#define cmdSET_BLOCK_COUNT      0x57        //11
#define cmdWRITE_SINGLE_BLOCK   0x58        //12
#define cmdWRITE_MULTI_BLOCK    0x59        //13
#define cmdTAG_SECTOR_START     0x60        //14
#define cmdTAG_SECTOR_END       0x61        //15
#define cmdUNTAG_SECTOR         0x62        //16
#define cmdTAG_ERASE_GRP_START  0x63        //17
#define cmdTAG_ERASE_GRP_END    0x63        //18
#define cmdUNTAG_ERASE_GRP      0x65        //19
#define cmdERASE                0x66        //20
#define cmdSD_APP_OP_COND       0x69        //21
#define cmdLOCK_UNLOCK          0x71        //22
#define cmdAPP_CMD              0x77        //23
#define cmdREAD_OCR             0x7a        //24
#define cmdCRC_ON_OFF           0x7b        //25

#define SDC_FLOATING_BUS    0xFF
#define SDC_BAD_RESPONSE    SDC_FLOATING_BUS
#define SDC_SECTOR_SIZE        512 

#define HCS_YES 0x40000000 // HCS high capacity SD card supported.
#define STD_CAPACITY 0
#define HIGH_CAPACITY 1

#define DATA_ACCEPTED    0x05
#define DATA_REJECTED_WR_ERR    0x0D
#define DATA_REJECTED_CRC_ERR    0x0B

#define MICRO_SD                    // Using micro SD card type memory.

/* clock divisor used during mmc init */
#define TRANS_CLOCK_IN_INIT 127 // PCLK=48MHz SPBR=127 BRDV=0: 187.5kbps(PCLK/256).

#define HIGHSPEED_SDCLOCK 0
#define SDCLOCK 1
//#define SDCLOCK HIGHSPEED_SDCLOCK

#define SIO_SPBR    RSPI0.SPBR.BYTE    // RSPI bit rate register. 

#define NUMBYTES_FOR_1MS_CLOCKS 17  // 24 * 8 bits = 192 clocks at 187.5kbps = 1mS. 

/* Choose which RSPI channel should be used to communicate with the SD Card.
   On the RDKRX63N it is channel 0. */
#define SDCARD_RSPI_CHANNEL RSPI_CHANNEL_0

/*******************************************************************************
* Private global variables and functions 
*******************************************************************************/
static CSD_REG g_CSDregister;   // Union containing raw CDS register bitfields.

CSD_DECODED g_CSDvalues;    // CSD values decoded from the raw register bitfields.

static volatile bool g_mmc_delay; // Used for delay timer status.

static uint8_t g_SDCard_type;

/* Index to this table is the command enumeration                            */
/* SPI mode can ignore CRC values except for GO_IDLE_STATE                   */
/* command, so we will just stuff dummy bits into CRC for simplicity         */
/* A more robust implementation should calculate valid CRCs and               */
/* issue the CRC_ON_OFF command to enable CRCs                               */ 
static const sdc_cmd_t sdcmmc_cmdtable[] =
{
    /*command name              CRC   response type data follows? */    
    {cmdGO_IDLE_STATE,          0x95,    R1,            NODATA},    // CRC must be valid for CMD0(0).
    {cmdSEND_OP_COND,           0xF9,    R1,            NODATA},
    {cmdSEND_IF_COND,           0x87,    R7,            NODATA},    // CRC must be valid for CMD8 + set end bit.
    {cmdSEND_CSD,               0xAF,    R1,            MOREDATA},
    {cmdSEND_CID,               0x1B,    R1,            MOREDATA},
    {cmdSTOP_TRANSMISSION,      0xC3,    R1,            NODATA},
    {cmdSEND_STATUS,            0xAF,    R2,            NODATA},
    {cmdSET_BLOCKLEN,           0xFF,    R1,            NODATA},
    {cmdREAD_SINGLE_BLOCK,      0xFF,    R1,            MOREDATA},
    {cmdREAD_MULTI_BLOCK,       0xFF,    R1,            MOREDATA},
    {cmdSET_BLOCK_COUNT,        0xFF,    R1,            NODATA},
    {cmdWRITE_SINGLE_BLOCK,     0xFF,    R1,            MOREDATA},
    {cmdWRITE_MULTI_BLOCK,      0xFF,    R1,            MOREDATA},
    {cmdTAG_SECTOR_START,       0xFF,    R1,            NODATA},
    {cmdTAG_SECTOR_END,         0xFF,    R1,            NODATA},
    {cmdUNTAG_SECTOR,           0xFF,    R1,            NODATA},
    {cmdTAG_ERASE_GRP_START,    0xFF,    R1,            NODATA},
    {cmdTAG_ERASE_GRP_END,      0xFF,    R1,            NODATA},
    {cmdUNTAG_ERASE_GRP,        0xFF,    R1,            NODATA},
    {cmdERASE,                  0xDF,    R1b,           NODATA},
    {cmdSD_APP_OP_COND,         0xE5,    R1,            NODATA},
    {cmdLOCK_UNLOCK,            0x89,    R1b,           NODATA},
    {cmdAPP_CMD,                0x73,    R1,            NODATA},
    {cmdREAD_OCR,               0x25,    R3,            NODATA},
    {cmdCRC_ON_OFF,             0x25,    R1,            NODATA},
};


/*******************************************************************************
* Private function declarations 
*******************************************************************************/
static void mmc_send_8_clocks(void);
static uint8_t mmc_read_byte(void);
static sdc_response_t sdcard_command_send(uint8_t cmd, uint32_t address);
static sdc_error_t sdcard_read_CSD(void);
static sdc_error_t sdcard_media_init(sdc_state_t * state_flag);
static void mmc_delay(uint16_t period);
static void rspi_clock_set_rate(uint8_t rate_divisor);
static void sdcard_decode_CSD(CSD_REG* csd_reg);

#ifndef MICRO_SD
    static uint8_t sdcard_detect(void);
    static uint8_t sdcard_write_protect_get(void);
    static void mmc_crc_read(void);
    static void mmc_crc_send(void);
#endif

/*******************************************************************************
* Function definitions 
*******************************************************************************/
/*******************************************************************************
* Function Name : R_SdCardSlotInit 
* Description     : Invokes the mmc driver and card initialisation. 
* Arguments      : none
* Return value  : uint8_t 1 success, 0 failure
*******************************************************************************/
uint8_t R_SdCardSlotInit(void)
{
    sdc_state_t state_flag;
    sdc_error_t   result;

    R_RSPI_Init(SDCARD_RSPI_CHANNEL);
    
    cmt_init(); // Initialize a timer

    result = sdcard_media_init(&state_flag);
    if (SDC_VALID == result)
    {
        return 1; // success
    }
    else
    {
        return 0; // fail
    }
} /* end R_SdCardSlotInit */


/*******************************************************************************
* Function Name : mmc_crc_read 
* Description     : Dummy CRC 'reader' writes two 0xFF CRC bytes.
*                  SPI chip select must already be asserted
*                  replace this with real CRC calculation if needed
* Arguments      : none
* Return value  : none
*******************************************************************************/
static void mmc_crc_read(void)
{
    uint8_t crc = 0xFF;

    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &crc, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &crc, 1, SDCARD_RSPI_PID);        
} /* end mmc_crc_read */


/*******************************************************************************
* Function Name : mmc_crc_send 
* Description     : dummy CRC generator writes two 0xFF CRC bytes
*                  SPI chip select must already be asserted
*                  replace this with real CRC calculation if needed
* Arguments      : none
* Return value  : none
*******************************************************************************/
static void mmc_crc_send(void)
{
    uint8_t crc = 0xFF;

    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &crc, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &crc, 1, SDCARD_RSPI_PID);        
} /* end mmc_crc_send */


/*******************************************************************************
* Function Name : mmc_send_8_clocks 
* Description     : Writes one 0xFF to SPI to send 8 clocks indicating end of command.
*                  SPI chip select must already be asserted.
* Arguments      : none
* Return value  : none
*******************************************************************************/
static void mmc_send_8_clocks(void)
{
    uint8_t clocks = 0xFF;

    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &clocks, 1, SDCARD_RSPI_PID);
} /* end mmc_send_8_clocks */


/********************************************************************************
* Function Name : mmc_read_byte 
* Description     : Reads a byte from the SPI bus.
* Arguments      : none
* Return value  : uint8_t A byte from the SPI bus.
*******************************************************************************/
static uint8_t mmc_read_byte(void)
{
    uint8_t a_byte;

    R_RSPI_Read(SDCARD_RSPI_CHANNEL, &a_byte, 0x01, SDCARD_RSPI_PID);
    return a_byte;
} /* end mmc_read_byte */            


/*******************************************************************************
* Function Name : sdcard_detect 
* Description     : Return the state of the card-detect pin.
*                : Not present on micro SD.
* Arguments      : none
* Return value  : uint8_t    The card present state.
*******************************************************************************/
#ifndef MICRO_SD
static uint8_t sdcard_detect(void)
{
    if (MEDIA_CD)
    {    
        return 0;    // Card not present. 
    }
    else 
    {
        return 1;    // Card present. 
    }
} /* end sdcard_detect */
#endif


/*******************************************************************************
* Function Name : sdcard_write_protect_get 
* Description     : Returns the state of the card write protect tab.
*                : Not present on micro SD.
* Arguments      : none
* Return value  : uint8_t   1 if write protected, 0 if not.
*******************************************************************************/
#ifndef MICRO_SD
static uint8_t sdcard_write_protect_get(void)
{
    if (MEDIA_WP) // Macro to be defined. 
    {
        return 1; // Write protected. 
    }
    else
    {
        return 0; // Write enabled.
    }    
} /* end sdcard_write_protect_get */
#endif


/*******************************************************************************
* Function Name    : sdcard_command_send
* Description     : Sends commands to the SD card.
* Arguments      : (uint8_t cmd, uint32_t address/argument)            
* Return value  : sdc_response_t Success or error code.
*******************************************************************************/
static sdc_response_t sdcard_command_send(uint8_t cmd, uint32_t address)
{

    cmd_packet_t   cmd_packet;
    uint8_t        idx;
    sdc_response_t response;
    uint16_t       timeout = 10; // retries 

    R_RSPI_Select(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID);  // Assert SD slot chip select. 

    /* Store command, address (or argument), and CRC in the cmd_packet_t struct. */
    cmd_packet.BYTES.cmd  = (uint8_t)sdcmmc_cmdtable[cmd].cmd_code;
    // Pack the 32bit address value into 4 bytes on odd boundary. 
    cmd_packet.BYTES.addr0 = (uint8_t)(address & 0xff);
    cmd_packet.BYTES.addr1 = (uint8_t)((address >> 8) & 0xff);
    cmd_packet.BYTES.addr2 = (uint8_t)((address >> 16) & 0xff);
    cmd_packet.BYTES.addr3 = (uint8_t)((address >> 24) & 0xff);        
    cmd_packet.BYTES.crc = sdcmmc_cmdtable[cmd].crc;

    /* Send command, address, and CRC over the SPI bus. */
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.cmd, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.addr3, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.addr2, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.addr1, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.addr0, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.crc, 1, SDCARD_RSPI_PID);

    if ((R1 == sdcmmc_cmdtable[cmd].response_type) 
        || (R1b == sdcmmc_cmdtable[cmd].response_type)
        || (R7 == sdcmmc_cmdtable[cmd].response_type)
        || (R3 == sdcmmc_cmdtable[cmd].response_type))
    {
        do 
        {
            /* Read a byte from the sdcard until byte not FFh or until a timeout. */
            response.r1.uByte = mmc_read_byte();
            timeout--;
        } while ((response.r1.uByte > 1) && (timeout != 0)); 
    }
    else if (R2 == sdcmmc_cmdtable[cmd].response_type) // Is the command's response type R2? 
    {
        do 
        {     /* read the first byte */
            response.r2.BYTES.byte_0 = mmc_read_byte();
            timeout--;                        
        } while ((0xFF == response.r2.BYTES.byte_0) && (0 != timeout)); 

        /* read the second byte */
        if (0xFF != response.r2.BYTES.byte_0)
        {
            response.r2.BYTES.byte_1 = mmc_read_byte();
        } 

    }
    else
    {
        //dummy
    }
    
    if (R1b == sdcmmc_cmdtable[cmd].response_type)
    {
        /* Have already read R1b response byte. Now wait for not busy status. */
        response.r1.uByte = 0x00;

        for (idx = 0; ((idx < 0xFF) && (0x00 == response.r1.uByte)); idx++)
        {
            timeout = 0xFFF; // empirically determined number of retries (should be more than enough)
            do 
            {
                response.r1.uByte = mmc_read_byte();
                timeout--;
            } while ((0x00 == response.r1.uByte) && (0 != timeout)); // retry for nonzero response
        }
    }
    
    if ((R3 == sdcmmc_cmdtable[cmd].response_type) 
        ||(R7 == sdcmmc_cmdtable[cmd].response_type)) 
    {
        if (response.r1.uByte <= 0x01) /* Read the rest of the response. */
        {
            response.uByte[1] = mmc_read_byte(); 
            response.uByte[2] = mmc_read_byte();  
            response.uByte[3] = mmc_read_byte();         
            response.uByte[4] = mmc_read_byte(); 
        }        
    }    

    mmc_send_8_clocks();    // 8 clocks required to complete the command.

    if (!(sdcmmc_cmdtable[cmd].more_data_expected))
    {
        R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Done. Deselect the sdcard 
    }

    return response;
} /* end sdcard_command_send */


/*******************************************************************************
* Function Name    : sdcard_read_CSD
* Description     : Reads the SD card's CSD register.
* Arguments      : none            
* Return value  : sdc_error_t Success or error code.
*******************************************************************************/
static sdc_error_t sdcard_read_CSD(void)
{
    uint32_t        address = 0x0000;
    uint8_t            cmd = SEND_CSD;
    cmd_packet_t    cmd_packet;
    uint8_t            data_token;
    uint16_t        idx;
    uint16_t        timeout;    
    sdc_response_t  response;
    sdc_error_t        status = SDC_VALID;

    R_RSPI_Select(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID);  // Assert SD slot chip select. 

    /* Store command, address, and CRC in the cmd_packet_t struct */
    cmd_packet.BYTES.cmd = sdcmmc_cmdtable[cmd].cmd_code;
    /* Pack the 32bit address value into 4 bytes on odd boundary.  */
    cmd_packet.BYTES.addr0 = (uint8_t)(address & 0xff);
    cmd_packet.BYTES.addr1 = (uint8_t)((address >> 8) & 0xff);
    cmd_packet.BYTES.addr2 = (uint8_t)((address >> 16) & 0xff);
    cmd_packet.BYTES.addr3 = (uint8_t)((address >> 24) & 0xff);    
    cmd_packet.BYTES.crc = sdcmmc_cmdtable[cmd].crc;

    /* Send command, address, and CRC over the SPI bus. */
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.cmd, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.addr3, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.addr2, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.addr1, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.addr0, 1, SDCARD_RSPI_PID);
    R_RSPI_Write(SDCARD_RSPI_CHANNEL, &cmd_packet.BYTES.crc, 1, SDCARD_RSPI_PID);

    timeout = 0x8ff;

    do      // Read a byte from the sdcard until byte not FFh or until a timeout.  
    {
        response.r1.uByte = mmc_read_byte();
        timeout--;
    } while ((0xFF == response.r1.uByte) && (timeout != 0)); 

    if (0x00 == response.r1.uByte) // Command was accepted. 
    {
        timeout = 0x2ff;

        do                  // Wait for the data start token or timeout. 
        {
            data_token = mmc_read_byte();
            timeout--;
        } while ((SDC_FLOATING_BUS == data_token) && (timeout != 0));

        if((0 == timeout) || (DATA_START_TOKEN != data_token))
        {           // Reached end of retries before getting a start token. 
            status = SDC_CARD_TIMEOUT; 
        }
        else
        {         
            for (idx = 0; idx < CSD_SIZE; idx++ ) // Read 16 bytes from the CSD register
            {
                g_CSDregister.CSD_ARRAY[idx] = mmc_read_byte();
            }        
        }

        mmc_send_8_clocks();        // Send the 8 clocks to complete the command.    
    } 

    else      // command was rejected                                     
    {
        status = SDC_BAD_CMD;
    }

    R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Done. Deselect the sdcard. 

    return status;    
} /* end sdcard_read_CSD */


/*******************************************************************************
* Function Name    : R_SdcSectorRead
* Description     : Reads 512 byte block from the SD card.
* Arguments      : uint32_t sector_addr, uint8_t* buffer            
* Return value  : sdc_error_t Success or error code.
*******************************************************************************/
sdc_error_t R_SdcSectorRead(uint32_t sector_addr, uint8_t* buffer)
{
    uint8_t         data_token;
    uint16_t        timeout;
    uint32_t        address;    
    sdc_response_t  response;
    sdc_error_t     status = SDC_VALID;

    if(HIGH_CAPACITY == g_SDCard_type) 
    {
        rspi_clock_set_rate(HIGHSPEED_SDCLOCK);        
        address = sector_addr;                     // High capacity uses sector address. 
    }
    else
    {
//        rspi_clock_set_rate(HIGHSPEED_SDCLOCK);         
        rspi_clock_set_rate(SDCLOCK);          
        address = sector_addr * SDC_SECTOR_SIZE; // Standard capacity uses byte address.
    }

    response = sdcard_command_send((uint8_t)READ_SINGLE_BLOCK, address);
    if (0x00 == response.r1.uByte) // Command was accepted.
    {
        timeout = 0x8ff;
        do                         // Wait for the data start token or timeout. 
        {
            data_token = mmc_read_byte();
            timeout--;
        } while ((SDC_FLOATING_BUS == data_token) && (timeout != 0));

        if((0 == timeout) || (data_token != DATA_START_TOKEN))
        {                       // Reached end of retries before getting a start token. 
            status = SDC_CARD_TIMEOUT; 
        }        
        else
        {
            R_RSPI_Read(SDCARD_RSPI_CHANNEL, buffer, 512, SDCARD_RSPI_PID);

            mmc_crc_read();         // Fake CRC, so we don't do anything with it. 
        }

        mmc_send_8_clocks();            // Send the 8 clocks to complete the command.    

        response = sdcard_command_send((uint8_t)SEND_STATUS, 0x00);
        if (response.r2._word)
        {
            status = SDC_READ_CRC_ERR;
        }            
    }
    else                                       // command was rejected 
    {
        status = SDC_BAD_CMD;
    }

    R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Done. Deselect the sdcard.     
    return status;    
} /* end R_SdcSectorRead */


/*******************************************************************************
* Function Name    : R_SdcBlockWrite
* Description     : Writes SDC_SECTOR_SIZE byte block to the SD card.
* Arguments      : uint8_t* buffer, uint32_t sector_addr, uint8_t sector_count          
* Return value  : sdc_error_t Success or error code.
*******************************************************************************/
sdc_error_t R_SdcBlockWrite(uint8_t* buffer, uint32_t sector_addr, uint8_t sector_count)
{
    uint8_t            data_response;
    uint8_t         data_start_token = DATA_START_TOKEN;
    uint16_t        idx;
    uint32_t        timeout;
    uint32_t        address;    
    sdc_response_t  response;
    sdc_error_t        status = SDC_VALID;

    if(HIGH_CAPACITY == g_SDCard_type) 
    {
        rspi_clock_set_rate(HIGHSPEED_SDCLOCK);
        address = sector_addr;                      // High capacity uses sector address.
    }
    else
    {
        rspi_clock_set_rate(SDCLOCK);
        address = sector_addr * SDC_SECTOR_SIZE; // Standard capacity uses byte address. 
    }

    response = sdcard_command_send((uint8_t)WRITE_SINGLE_BLOCK, address);    
    if (0x00 == response.r1.uByte)                 // Command was accepted.    
    {
        R_RSPI_Write(SDCARD_RSPI_CHANNEL, &data_start_token, 1, SDCARD_RSPI_PID);

        /* write SDC_SECTOR_SIZE bytes to the sdcard */
        for (idx = 0; idx < SDC_SECTOR_SIZE; idx++)
        {
            R_RSPI_Write(SDCARD_RSPI_CHANNEL, &buffer[idx], 1, SDCARD_RSPI_PID);
        }

        mmc_crc_send();

        data_response = mmc_read_byte();            // Read the response token.             
        if ((data_response & 0x0F) != DATA_ACCEPTED )
        {   
            status = SDC_DATA_REJECTED; 
        }        
        else                                    // Wait, card is busy.
        {
            timeout = 0x2ffff;                    // Could take up to 250mS. 
            do
            {
                data_response = mmc_read_byte();
                timeout--;
            } while ((0 == data_response) && (timeout != 0));

            if (0 == timeout)
            {
                status = SDC_CARD_TIMEOUT;
            }        
        }

        mmc_send_8_clocks();                // Send 8 clocks to complete the command         

    }
    else                                          // Command was rejected. 
    {
        status = SDC_BAD_CMD;
    }

    R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Done. Deselect the sdcard.     
    return status;    
} /* end R_SdcBlockWrite */


/*******************************************************************************
* Function Name    : sdcard_media_init
* Description     : Initializes the SD card.
* Arguments      : sdc_state_t * state_flag            
* Return value  : sdc_error_t Success or error code.
*******************************************************************************/
static sdc_error_t sdcard_media_init(sdc_state_t * state_flag)
{
    sdc_error_t        CSDstatus = SDC_VALID;
    sdc_response_t  response;
    sdc_error_t        status = SDC_VALID;
    uint16_t        timeout;


    state_flag->uByte = 0x00;

    R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Deselect the sdcard 

    // Set SPI clock to <= 400 KHz until card is initialized, as per MMC spec 
    rspi_clock_set_rate(TRANS_CLOCK_IN_INIT);


    mmc_delay(100); // delay 100mS for card to initialize (spec.)

    /* output clocks for 1 mS, as per MMC spec */
    for (timeout = 0; timeout < NUMBYTES_FOR_1MS_CLOCKS; timeout++)
    {
        mmc_send_8_clocks();
    }

    R_RSPI_Select(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID);  // assert SD slot chip select 


    mmc_delay(1); // 1mS delay required by spec.

    // send GO_IDLE_STATE command to invoke SPI mode on card 
    response = sdcard_command_send((uint8_t)GO_IDLE_STATE, 0x00);

    if (SDC_BAD_RESPONSE == response.r1.uByte)
    {
        status = SDC_COMM_INIT_FAIL;
        R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Deselect the sdcard 
        rspi_clock_set_rate(1);  // restore clock speed 
        return status;    
    }

    if (response.r1.uByte != 0x01) // if 01, card is in idle state and still initializing 
    {
        status = SDC_INIT_FAIL;
        R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Deselect the sdcard 
        rspi_clock_set_rate(1);  // restore clock speed 
        return status;
    } 

    /* Now send SEND_IF_COND command to identify version and supported voltage of the card */
    /* CMD08 supported only in SdCard spec version 1.1 or higher.                          */
    /* CMD08 argument = check pattern 0xAA, | voltage check 0x01 for 2.7-3.6V range.       */
    response = sdcard_command_send((uint8_t)SEND_IF_COND, 0x01AA);
    if (0x01 == response.r1.uByte) // command was accepted and card is still in idle state  
    {        
        if ((0x01 == response.r7.BYTES.V4) && (0xAA == response.r7.BYTES.CHK_PATTERN)) // voltage check and check pattern OK 
        {                
            timeout = 0xfff; // adjust timeout retries if necessary for cards requiring longer initialization time */
            do
            {
                response =sdcard_command_send((uint8_t)APP_CMD, 0x00);    // send CMD55 to indicate next command is 'App' type 
                response = sdcard_command_send((uint8_t)SD_APP_OP_COND, HCS_YES); //send ACMD41 with high capacity support 
                timeout--;    
            } while ((response.r1.uByte != 0x00) && (timeout!= 0)); // loop here until the card returns ready 

            if (timeout != 0)
            {
                response = sdcard_command_send((uint8_t)READ_OCR, 0x00); // send CMD58 to get CCS
                 
                if (response.r3.r1.uByte > 1)
                {
                    status = SDC_INIT_FAIL;
                    R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Deselect the sdcard 
                    rspi_clock_set_rate(1);  // restore clock speed 
                    return status;
                }    
                                                                
                if (response.r3.OCR3 & 0x40)         // high capacity bit in OCR was set 
                {
                    g_SDCard_type = HIGH_CAPACITY;
                }
                else
                {
                    g_SDCard_type = STD_CAPACITY;                    
                }
            }
            else 
            {
                status = SDC_INIT_TIMEOUT;
                R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Deselect the sdcard 
                rspi_clock_set_rate(1);  // restore clock speed 
                return status;
            }            
        } 
        else                // bad voltage check or check pattern. Card not supported 
        {
            status = SDC_TYPE_INVALID;
            R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Deselect the sdcard 
            rspi_clock_set_rate(1);  // restore clock speed 
            return status;    
        }        
    }

    else if (response.r1.BIT.ILLEGAL_CMD)                 // command was rejected 
    {
        status = SDC_BAD_CMD;
            R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Deselect the sdcard 
            rspi_clock_set_rate(1);  // restore clock speed 
            return status;    
    }    
    else 
    {     /* Not SD 1.1 or later card. MMC or legacy ? */ 
        /* send SEND_OP_COND command until response received from card or timeout */
        timeout = 0xfff; // adjust timeout retries if necessary for cards requiring longer initialization time 
        do
        {
            response = sdcard_command_send((uint8_t)SEND_OP_COND, 0x00); // supporting only standard capacity card 
            timeout--;
        } while((response.r1.uByte != 0x00) && (timeout!= 0));

        if (0 == timeout)
        {
            status = SDC_INIT_TIMEOUT;
            R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Deselect the sdcard 
            rspi_clock_set_rate(1);  // restore clock speed 
            return status;        
        }
    }    

    /* Command was successful, so read the CSD register */
    CSDstatus = sdcard_read_CSD();
    if (0 == CSDstatus)                     // Status OK. Ready to increase SPI clock speed 
    {        
        sdcard_decode_CSD(&g_CSDregister);    // pull the sdcard data from the CSD bitfields 
        
        if (STD_CAPACITY == g_SDCard_type)
        {  
            rspi_clock_set_rate(SDCLOCK);  // raise the SPI clock speed
        } 
        else
        {                  // High capacity card can clock faster.
            rspi_clock_set_rate(HIGHSPEED_SDCLOCK);  // raise the SPI clock speed
        }           
    }
    else // CSD read failed 
    {
        status = SDC_TYPE_INVALID;
        R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Deselect the sdcard 
        rspi_clock_set_rate(1);  // restore clock speed 
        return status;        
    }        

    // send SET_BLOCKLEN command to set the block length to SDC_SECTOR_SIZE 
    response = sdcard_command_send((uint8_t)SET_BLOCKLEN, SDC_SECTOR_SIZE);

    // Set write protect bit in sdc_state_t if the card is write protected
#ifndef MICRO_SD     
    if (1 == sdcard_write_protect_get())
    {
        state_flag->isWP = 1;
    }
#endif

    R_RSPI_Deselect(SDCARD_RSPI_CHANNEL, SDMICRO_SELECTED, SDCARD_RSPI_PID); // Deselect the sdcard 
    rspi_clock_set_rate(1);  // restore clock speed 
    return status;    
    
} /* end sdcard_media_init */


/*******************************************************************************
* Function Name    : sdcard_decode_CSD
* Description     : Extracts the sdcard parameters from the CSD bitfields
*                :  and stores the results in a global structure.
* Arguments      : CSD_REG* csd_reg
* Return value  : none
*******************************************************************************/
static void sdcard_decode_CSD(CSD_REG* csd_reg)
{
    g_CSDvalues.CSD_STRUCTURE = (uint8_t)(csd_reg->CSD_REG_V1.CSD_VERSION & 0xC0);
    g_CSDvalues.TAAC          = (uint8_t)csd_reg->CSD_REG_V1.TAAC;
    g_CSDvalues.NSAC          = (uint8_t)csd_reg->CSD_REG_V1.NSAC;
    g_CSDvalues.TRAN_SPEED    = (uint8_t)csd_reg->CSD_REG_V1.TRAN_SPEED;

    g_CSDvalues.CCC =  (((uint16_t)(csd_reg->CSD_REG_V1.CCC8) << 4) & 0x0FF0)
        | ((csd_reg->CSD_REG_V1.CCC4_READ_BL_LEN >> 4) & 0x0F);

    g_CSDvalues.READ_BL_LEN = (uint8_t)(csd_reg->CSD_REG_V1.CCC4_READ_BL_LEN & 0x0F);

    g_CSDvalues.READ_BL_PARTIAL    = (uint8_t)((csd_reg->CSD_REG_V1.RBP_WBM_RBM_DSR_CSZ2 >> 7)& 0x01);
    g_CSDvalues.WRITE_BLK_MISALIGN = (uint8_t)((csd_reg->CSD_REG_V1.RBP_WBM_RBM_DSR_CSZ2 >> 6)& 0x01);
    g_CSDvalues.READ_BLK_MISALIGN  = (uint8_t)((csd_reg->CSD_REG_V1.RBP_WBM_RBM_DSR_CSZ2 >> 5)& 0x01);
    g_CSDvalues.DSR_IMP            = (uint8_t)((csd_reg->CSD_REG_V1.RBP_WBM_RBM_DSR_CSZ2 >> 4)& 0x01);

    if (0 == g_CSDvalues.CSD_STRUCTURE) /* version 1.x 12 bits*/
    {
        g_CSDvalues.C_SIZE = (((uint32_t)csd_reg->CSD_REG_V1.RBP_WBM_RBM_DSR_CSZ2 << 10)& 0xC00)
            | (((uint32_t)csd_reg->CSD_REG_V1.CSZ8                 <<  2)& 0x3FC)
            | (((uint32_t)csd_reg->CSD_REG_V1.CSZ2_VRMIN_VRMAX     >>  6)& 0x003);

        //CSZ2_VRMIN_VRMAX  :8;    // 2+3+3
        g_CSDvalues.VDD_R_CURR_MIN = (uint8_t)((csd_reg->CSD_REG_V1.CSZ2_VRMIN_VRMAX >> 3)& 0x07);
        g_CSDvalues.VDD_R_CURR_MAX = (uint8_t)(csd_reg->CSD_REG_V1.CSZ2_VRMIN_VRMAX & 0x07);
        //VWMIN_VWMAX_CSM2    :8; // 3+3+2
        g_CSDvalues.VDD_W_CURR_MIN = (uint8_t)((csd_reg->CSD_REG_V1.VWMIN_VWMAX_CSM2 >> 5)& 0x07);
        g_CSDvalues.VDD_W_CURR_MAX = (uint8_t)((csd_reg->CSD_REG_V1.VWMIN_VWMAX_CSM2 >> 2)& 0x07);

        //VWMIN_VWMAX_CSM2    :8; // 3+3+2
        //CSM1_EBE_SSZ6     :8; // 1+1+6
        g_CSDvalues.C_SIZE_MULT = (uint8_t)((csd_reg->CSD_REG_V1.VWMIN_VWMAX_CSM2 << 1)& 0x06)
            |((csd_reg->CSD_REG_V1.CSM1_EBE_SSZ6    >> 7)& 0x01);

        g_CSDvalues.ERASE_BLK_EN = (uint8_t)((csd_reg->CSD_REG_V1.CSM1_EBE_SSZ6 >> 6) & 0x01);

        //CSM1_EBE_SSZ6     :8; // 1+1+6
        //SSZ1_WGRPSZ       :8;    // 1+7
        g_CSDvalues.SECTOR_SIZE = (uint8_t)((csd_reg->CSD_REG_V1.CSM1_EBE_SSZ6 << 1) & 0x7E)
            |((csd_reg->CSD_REG_V1.SSZ1_WGRPSZ   >> 7) & 0x01);        
    }
    else /* version 2.x  22 bits */ 
    {
        g_CSDvalues.C_SIZE = (((uint32_t)csd_reg->CSD_REG_V2._2_C_SIZE_6_   << 16)& 0x3F0000)
            |(((uint32_t)csd_reg->CSD_REG_V2._C_SIZE_MID_8_ <<  8)& 0x00FF00)
            | ((uint32_t)csd_reg->CSD_REG_V2._C_SIZE_LAST_8       & 0x0000FF);

        // EBE_SSZ6          :8; // 1+1+6
        g_CSDvalues.ERASE_BLK_EN = (uint8_t)((csd_reg->CSD_REG_V2.EBE_SSZ6 >> 7)& 0x01);
        // SSZ1_WGRPSZ       :8;    // 1+7
        g_CSDvalues.SECTOR_SIZE  = (uint8_t)((csd_reg->CSD_REG_V2.EBE_SSZ6    << 1)& 0x7E)
            |((csd_reg->CSD_REG_V2.SSZ1_WGRPSZ >> 7)& 0x01);
    }

    // SSZ1_WGRPSZ       :8;    // 1+7
    g_CSDvalues.WP_GRP_SIZE = (uint8_t)(csd_reg->CSD_REG_V1.SSZ1_WGRPSZ & 0x7F);

    // WGE_R2W_WBL2      :8; // 1+2+3+2
    g_CSDvalues.WP_GRP_ENABLE      = (uint8_t)((csd_reg->CSD_REG_V1.WGE_R2W_WBL2 >> 7)& 0x01);
    g_CSDvalues.R2W_FACTOR         = (uint8_t)((csd_reg->CSD_REG_V1.WGE_R2W_WBL2 >> 5)& 0x03);
    g_CSDvalues.WRITE_BL_LEN       = (uint8_t)((csd_reg->CSD_REG_V1.WGE_R2W_WBL2 >> 2)& 0x07);
    g_CSDvalues.WRITE_BL_PARTIAL   =  (uint8_t)(csd_reg->CSD_REG_V1.WGE_R2W_WBL2      & 0x03);

    // FFG_CP_PWP_TWP_FF :8; // 1+1+1+1+2+2
    g_CSDvalues.FILE_FORMAT_GRP    = (uint8_t)((csd_reg->CSD_REG_V1.FFG_CP_PWP_TWP_FF >> 7)& 0x01);
    g_CSDvalues.COPY               = (uint8_t)((csd_reg->CSD_REG_V1.FFG_CP_PWP_TWP_FF >> 6)& 0x01);
    g_CSDvalues.PERM_WRITE_PROTECT = (uint8_t)((csd_reg->CSD_REG_V1.FFG_CP_PWP_TWP_FF >> 5)& 0x01);
    g_CSDvalues.TMP_WRITE_PROTECT  = (uint8_t)((csd_reg->CSD_REG_V1.FFG_CP_PWP_TWP_FF >> 4)& 0x01);
    g_CSDvalues.FILE_FORMAT        = (uint8_t)((csd_reg->CSD_REG_V1.FFG_CP_PWP_TWP_FF >> 2)& 0x03);

    //CSD_CRC           :8; // 7+1    
    g_CSDvalues.CSD_CRC = (uint8_t)((csd_reg->CSD_REG_V1.CSD_CRC >> 1)& 0x7F);
} /* end sdcard_decode_CSD */


/*******************************************************************************
* Function Name    : rspi_clock_set_rate
* Description     :
* Arguments      : uint8_t rate_divisor        
* Return value  : none
*******************************************************************************/
static void rspi_clock_set_rate(uint8_t rate_divisor)
{
    R_RSPI_BaudRateSet(SDCARD_RSPI_CHANNEL, rate_divisor, SDCARD_RSPI_PID);
} /* end rspi_clock_set_rate */


/*******************************************************************************
* Function Name    : mmc_delayCB 
* Description     : Clears the delay status, ending the delay.
* Arguments      : uint8_t rate_divisor        
* Return value  : none
*******************************************************************************/
static void mmc_delayCB(void)
{
    g_mmc_delay = false;
} /* end mmc_delayCB */


/*******************************************************************************
* Function Name    : mmc_delay
* Description     : Convenient time delay in seconds
* Arguments      : uint16 period  in miliseconds       
* Return value  : none
*******************************************************************************/
static void mmc_delay(uint16_t period)
{    
    uint32_t ms_to_count; // For converting the milisecond value to counter value.

    g_mmc_delay = true; // Use as a flag.

    cmt_callback_set(&mmc_delayCB);

    ms_to_count = period * 94; // Using 48MHz PCLK/512 = ~10.66 uS per count.
    if (0xFFFF < ms_to_count)
    {
        ms_to_count = 0xFFFF;  //Limit it to maximum of 16-bit counter value.
    } 
    
    cmt_start((uint16_t)ms_to_count);
    
    while (g_mmc_delay)
    {
      ;
    }

}/* end mmc_delay */

/*******************************************************************************
* end  mmc_protocol.c
*******************************************************************************/
