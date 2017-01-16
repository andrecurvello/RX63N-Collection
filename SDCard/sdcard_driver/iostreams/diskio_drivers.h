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
* File Name      : diskio_drivers.h
* Version        : .01
* Device         : Renesas Generic MCU.
* Tool Chain     : HEW
* H/W Platform   : RDK Generic
* Description    : 
******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description            
*******************************************************************************/

/******************************************************************************
Define an array to hold function pointers for various physical storage device 
drivers. The device driver will provide functions called out in FATfs diskio.c API. 
A generic structure provides storage for each function type, i.e.: dis_read,   
disk_write, etc. A device driver is an instance of diskio API functions structure  
(diskio object) containing pointers to functions that implement its unique 
hardware level I/O operations. The driver array will contain a pointer to each  
logical device's driver. Driver array is of a fixed size of maximum number of 
logical units (LUNs). Duplicate driver entries are allowed for multiple LUNs of 
like type, therefore, driver object must be aware of its LUN.
A driver is registered by copying its driver object into a vacant slot in the 
drivers array, and providing an index to its location in the array.
******************************************************************************/

#ifndef DISKIO_DRIVERS_H
#define DISKIO_DRIVERS_H

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include "r_diskio.h"

#define MAX_NUM_LUNS 4 /* Just a small number for now to save memory. */

/* Add more drive units here */
typedef enum {
    SPI_FLASH_LUN = 0, /* drive 0: */
    SDCARD_LUN         /* drive 1: */
                       /* drive 2: */
} FS_LUN; 

/* Define driver function pointer types. */
typedef R_DRESULT(*R_DiskRead)(uint8_t, uint8_t*, uint32_t, uint8_t);     /* LUN, read buffer, sector, sector count */ 
typedef R_DRESULT(*R_DiskWrite)(uint8_t, uint8_t*, uint32_t, uint8_t);    /* LUN, write buffer, sector, sector count */ 
typedef R_DSTATUS(*R_DiskInitialize)(uint8_t);                            /* Drive number (LUN) */ 
typedef R_DSTATUS(*R_DiskStatus)(uint8_t);                                /* Drive number (LUN) */     
typedef R_DRESULT(*R_DiskIoCtl)(uint8_t, uint8_t, uint32_t* );            /* Drive number, command code, data buffer    */


typedef struct {
    uint8_t    LUN;
    R_DiskRead fp_disk_read;
    R_DiskWrite fp_disk_write;
    R_DiskInitialize fp_disk_init;
    R_DiskStatus fp_disk_stat;
    R_DiskIoCtl fp_disk_ctrl;
}disk_io_driver_t;

extern disk_io_driver_t * g_disk_driver_list[MAX_NUM_LUNS]; 

#endif