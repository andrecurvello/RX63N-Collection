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
* Copyright (C) 2012-2013 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name     : r_diskio.c
* Version       : 1.0
* Device(s)     : RX
* Tool-Chain    : C/C++ compiler package for RX family V.1.00+
* H/W Platform  :
* Description   : This is the FAT interface driver for sample application 
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*           27.06.2013
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include "../fat/diskio.h"
#include "r_diskio.h"
#include "diskio_drivers.h"

#define RTC_ENABLE 0
#if RTC_ENABLE
#include "RTC.h"
#endif

/******************************************************************************
global variables and functions
******************************************************************************/
disk_io_driver_t * g_disk_driver_list[MAX_NUM_LUNS] = {0};

/***********************************************************************************
Private Function Prototypes
*******************************************************************************/
static uint32_t r_get_fattime (void);


/*******************************************************************************
* Function Name    : r_disk_initialize
* Description    : This function initializes the memory medium for file operations
* Arguments        : lun - 
*                       logical unit (drive) number (0..)
* Return value    : stat -
*                       Status of the memory medium
*******************************************************************************/
R_DSTATUS r_disk_initialize(uint8_t drive) 
{
    R_DSTATUS stat;
    
    stat = g_disk_driver_list[drive]->fp_disk_init(drive);
    
    return     stat;
} /* End of function r_disk_initialize(). */


/*******************************************************************************
* Function Name    : r_disk_read
* Description    : This function reads data from the specified location of the memory medium
* Arguments        : drive -
*                       Physical drive number
*               : buffer -
*                       Pointer to the read data buffer
*               : sector_number -
*                       Sector number.
*               : sector_count -
*                       Number of sectors to read
* Return value    : Result code
*******************************************************************************/
R_DRESULT r_disk_read (
                       uint8_t drive,          /* Physical drive number             */
                       uint8_t* buffer,        /* Pointer to the read data buffer     */
                       uint32_t sector_number, /* Start sector number                 */
                       uint8_t sector_count)   /* Number of sectors to read         */
{
    R_DRESULT res;

    res = g_disk_driver_list[drive]->fp_disk_read(drive, buffer, sector_number, sector_count);
    return res;
} /* End of function r_disk_read(). */


/*******************************************************************************
* Function Name    : r_disk_write
* Description    : This function writes data to a specified location of the memory medium
* Arguments        : drive -
*                       Physical drive number.
*               : buffer -
*                       Pointer to the write data.
*               : sector_number -
*                       Sector number.
*               : sector_count -
*                       Number of sectors to write.
* Return value    : Result code.
*******************************************************************************/
R_DRESULT r_disk_write (
                        uint8_t drive,           /* Physical drive number             */
                        uint8_t* buffer,         /* Pointer to the write data         */
                        uint32_t sector_number,  /* Sector number to write             */
                        uint8_t sector_count)     /* Number of sectors to write         */
{
    R_DRESULT res;

    res = g_disk_driver_list[drive]->fp_disk_write(drive, buffer, sector_number, sector_count);
    return res;
} /* End of function r_disk_write(). */


/*******************************************************************************
* Function Name    : r_disk_ioctl
* Description    : This function is used to execute memory operations other than read\write
* Arguments        : drive -
*                       Physical drive number.
*               : cmd -
*                       Control command code.
*               : buffer -
*                       Data transfer buffer
* Return value    : Result code.
*******************************************************************************/
R_DRESULT r_disk_ioctl (
                        uint8_t drive,      /* Drive number             */
                        uint8_t cmd,        /* Control command code     */
                        uint32_t* buffer )  /* Data transfer buffer     */
{

    R_DRESULT res;

    res = g_disk_driver_list[drive]->fp_disk_ctrl(drive, cmd, buffer);
    return res;
} /* End of function r_disk_ioctl(). */


/*******************************************************************************
* Function Name    : r_disk_status
* Description    : This function is used to retrieve the current status of the disk
* Arguments        : drive -
*                       Physical drive number.
* Return value    : stat -
*                       Status of the disk
*******************************************************************************/
R_DSTATUS r_disk_status (uint8_t drive )            /* Physical drive number     */
{
    R_DSTATUS stat;

    stat = g_disk_driver_list[drive]->fp_disk_stat(drive);

    return     stat;
} /* End of function r_disk_status(). */


/*******************************************************************************
* Function Name    : r_get_fattime
* Description    : This function returns the current date and time
* Arguments        : none
* Return value    : time_date
*                       time and date in 32-bit packed bitfield.
*******************************************************************************/
static uint32_t r_get_fattime (void)
{
    uint32_t time_date;
#if RTC_ENABLE
    time_date_t now;

    rtc_read (&now);        /* get the current time and date from the RTC */

    /* Convert the time to store in FAT entry     */
    /* RX63 series only has 2 digits for the year so we assume year 2000+. */
    time_date = (((uint32_t)((now.year & 0x00FF) + 2000) - 1980) << 25);    /* years since 1980 */
    time_date |= ((uint32_t)now.month << 21);
    time_date |= ((uint32_t)now.day << 16);
    time_date |= ((uint32_t)now.hours << 11);
    time_date |= ((uint32_t)now.minutes << 5);
    time_date |= ((uint32_t)now.seconds >> 1);        /* seconds loses LSB? */
#endif
    return time_date;
} /* End of function r_get_fattime(). */


/******************************************************************************
FAT FS interface functions
These adapt the FatFs module to the I/O routines specific to the hardware 
envirronment.
******************************************************************************/

/******************************************************************************
* Function Name    : disk_initialize
* Description    : Interface to drive initialization function.
* Arguments        : drv -
*                       Physical drive number (0..) 
* Return value    : stat -
*                       File system result code
*******************************************************************************/
DSTATUS disk_initialize (BYTE drv)    
{
    DSTATUS stat;
    int result;

    result = r_disk_initialize(drv);
    if (result == R_RES_OK)
    {
        // translate the result code here
        stat = RES_OK;             
        return stat;
    }
    else
    {    
        return STA_NOINIT;
    }
} /* End of function disk_initialize(). */


/******************************************************************************
* Function Name    : disk_status
* Description    : Interface to r_disk_status
* Arguments        : drv -
*                       Physical drive number (0..) 
* Return value    : File system result code
*******************************************************************************/
DSTATUS disk_status (BYTE drv)        
{
    DSTATUS stat;
    int result;

    result = r_disk_status(drv);    
    if (result == R_RES_OK)
    {
        // translate the result code here
        stat = RES_OK;             
        return stat;
    }
    else
    {    
        return STA_NOINIT;
    }
} /* End of function disk_status(). */


/******************************************************************************
* Function Name    : disk_read
* Description    : Interface to read sector(s) function
* Arguments        : drv -
*                       Physical drive number (0..)
*                 *buff -
*                       Buffer to store read data in. 
*                 sector -
*                       Sector address (LBA)
*                 count -
*                       Number of sectors to write (1..255)
* Return value    : File system result code
*******************************************************************************/
DRESULT disk_read (
                   BYTE drv,        /* Physical drive number (0..) */
                   BYTE *buff,        /* Data buffer to store read data */
                   DWORD sector,    /* Sector address (LBA) */
                   BYTE count        /* Number of sectors to read (1..255) */
                   )
{
    DRESULT res;
    int result;

    result = r_disk_read(drv, buff, sector, count);    
    if (result == R_RES_OK)
    {
        // translate the result code here
        res = RES_OK;             
        return res;
    }
    else
    {    
        return RES_PARERR;
    }
} /* End of function disk_read(). */


/******************************************************************************
* Function Name    : disk_write
* Description    : Interface to write sector(s) function
* Arguments        : drv -
*                       Physical drive number (0..)
*                 *buff -
*                       Buffer containing Data to be written 
*                 sector -
*                       Sector address (LBA)
*                 count -
*                       Number of sectors to write (1..255)
* Return value    : File system result code
*******************************************************************************/
#if _FS_READONLY == 0
DRESULT disk_write (
                    BYTE drv,            /* Physical drive number (0..) */
                    const BYTE *buff,    /* Data to be written */
                    DWORD sector,        /* Sector address (LBA) */
                    BYTE count            /* Number of sectors to write (1..255) */
                    )
{
    DRESULT res;
    int result;

    result = r_disk_write(drv, (uint8_t*)buff, sector, count);    
    if (result == R_RES_OK)
    {
        // translate the result code here
        res = RES_OK;             
        return res;
    }
    else
    {    
        return RES_PARERR;
    }
} /* End of function disk_write(). */
#endif /* _FS_READONLY */


/******************************************************************************
* Function Name    : disk_ioctl
* Description    : Interface to IOCTL functions
* Arguments        : drv -
*                       Physical drive number (0..)
*                 ctrl -
*                       Control code.
*                 *buff -
*                       Buffer to send/receive control data
* Return value    : File system result code
*******************************************************************************/
DRESULT disk_ioctl (BYTE drv, BYTE ctrl, void *buff)
{
    DRESULT res;
    int result;

    result = r_disk_ioctl(drv, ctrl, buff);    
    if (result == R_RES_OK)
    {
        // translate the result code here
        res = RES_OK;             
        return res;
    }
    else
    {    
        return RES_PARERR;
    }
} /* End of function disk_ioctl(). */


/******************************************************************************
* Function Name    : get_fattime
* Description    : get_fattime interface Function
* Arguments        : none
* Return value    : tim -
*                       File system time
*******************************************************************************/
DWORD get_fattime(void)
{
    DWORD tim; 

    tim = r_get_fattime();

    return tim;
} /* End of function get_fattime(). */


