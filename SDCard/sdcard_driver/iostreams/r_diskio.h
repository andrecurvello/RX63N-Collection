/******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized.
* This software is owned by Renesas Electronics Corporation and is  protected
* under all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE  EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE  LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software. By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/******************************************************************************
* File Name        : r_diskio.h
* Version         : 1.00
* Device         : Renesas Generic MCU.
* Tool Chain     : HEW
* H/W Platform    : 
* Description     : 
******************************************************************************/

/******************************************************************************
* History         
******************************************************************************/
/***************************************************************************************/
#ifndef R_DISKIO_H
#define R_DISKIO_H

#include "../fat/ff.h"        /* FAT filesystem header file */
#include "../fat/diskio.h"

/***********************************************************************************
Definitions
***********************************************************************************/
typedef uint8_t    R_DSTATUS; /* Status of Disk Functions */

/* Results of Disk Functions */
typedef enum 
{
    R_RES_OK = 0,        /* 0: Successful */
    R_RES_ERROR,        /* 1: R/W Error */
    R_RES_WRPRT,        /* 2: Write Protected */
    R_RES_NOTRDY,        /* 3: Not Ready */
    R_RES_PARERR        /* 4: Invalid Parameter */
} R_DRESULT;


#define SPI_FLASH     0 /* LUN of logical drive unit */

#define FDISK    0 /* disk format rule */
#define SFD        1 /* disk format rule */

/***********************************************************************************
Public Function Prototypes
***********************************************************************************/
R_DSTATUS r_disk_initialize(uint8_t Drive);

R_DSTATUS r_disk_status (uint8_t Drive);             /* Physical drive number     */

R_DRESULT r_disk_read (
                       uint8_t Drive,         /* Physical drive number             */
                       uint8_t* Buffer,       /* Pointer to the read data buffer     */
                       uint32_t SectorNumber, /* Start sector number                 */
                       uint8_t SectorCount    /* Number of sectors to read         */
                       );

R_DRESULT r_disk_write (
                        uint8_t Drive,          /* Physical drive number             */
                        uint8_t* Buffer,        /* Pointer to the write data         */
                        uint32_t SectorNumber,  /* Sector number to write             */
                        uint8_t SectorCount     /* Number of sectors to write         */
                        );

R_DRESULT r_disk_ioctl (
                        uint8_t Drive,          /* Drive number             */
                        uint8_t Command,        /* Control command code     */
                        uint32_t* Buffer        /* Data transfer buffer     */
                        );       

#endif
