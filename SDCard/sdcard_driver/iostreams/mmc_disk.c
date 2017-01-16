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
* File Name        : mmc_disk.c
* Version         : 1.01
* Device         : Renesas Generic MCU.
* Tool Chain     : HEW
* H/W Platform    : RDK 
* Description     : Simple flash disk using mmc protocol. Implementation
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*           16.02.2011     1.00        First Release
*           15.07.2011     1.01        revision to coding standards
*           08.05.2012                 Updated for MP3 demo
*           07.06.2012                 changed data type in sdcard_compute_size()
*                                      to 64-bit for sdcards > 4GB capacity.
*******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include "mmc_disk.h"
#include "../mmc/mmc_protocol.h"
#include "../mmc/sdcard.h"
#include "diskio_drivers.h"
#include "../fat/diskio.h"
#include "r_diskio.h"

/*******************************************************************************
Imported global variables
*******************************************************************************/
extern CSD_DECODED g_CSDvalues; 

/*******************************************************************************
* Private function declarations
*******************************************************************************/
static uint16_t sdcard_get_sector_size(void);
#if 0
static uint32_t sdcard_get_block_size(void);
static uint32_t sdcard_get_num_blocks();
static bool mmc_disk_format_filesystem();
#endif
static uint64_t sdcard_compute_size();
static void sdcard_register_my_driver(uint8_t lun);

/***** driver functions *******************************************************/
                                    /* LUN, read buffer, sector, sector count */ 
static R_DRESULT sdcard_disk_read(uint8_t, uint8_t*, uint32_t, uint8_t);
                                    /* LUN, write buffer, sector, sector count */
static R_DRESULT sdcard_disk_write(uint8_t, uint8_t*, uint32_t, uint8_t); 
                                    /* Drive number (LUN) */
static R_DSTATUS sdcard_disk_initialize(uint8_t);  
                                    /* Drive number (LUN) */
static R_DSTATUS sdcard_disk_status(uint8_t);
                                    /* Drive number, command code, data buffer */
static R_DRESULT sdcard_disk_ioctl(uint8_t, uint8_t, uint32_t*); 

/*******************************************************************************
* Private global variables
*******************************************************************************/
/* to store the filesystem driver function pointers */
static disk_io_driver_t g_mmc_fs_driver = 
{
    0,
    0,
    0,
    0,
    0,
    0
};

static FATFS g_mmc_fatfs; /* local instance of FAT file system structure */

/*******************************************************************************
* Function Name: R_MmcDiskInit
* Description  :
* Argument     : none
* Return value : none
*******************************************************************************/
void R_MmcDiskInit(void)
{
    sdcard_register_my_driver(SDCARD_LUN);
} /* end R_MmcDiskInit */
 
 
/*******************************************************************************
* Function Name: sdcard_get_sector_size
* Description  : Get sector size
* Argument     : none
* Return value : sector size in bytes.
*******************************************************************************/
static uint16_t sdcard_get_sector_size(void)
{
    uint16_t block_len;
    
    if(!g_CSDvalues.CSD_STRUCTURE) /* SDC ver 1.XX standard capacity */
    {
        block_len = (uint16_t)(2 << (g_CSDvalues.READ_BL_LEN - 1));
    } 
    else /* high capacity */
    {
        block_len = SDC_SECTOR_SIZE; //this parameter not fully supported by this code for HC.
    }
    
    return block_len;    
} /* end sdcard_get_sector_size */ 
 
 
/*******************************************************************************
* Function Name: sdcard_get_block_size. Stub for future use
* Description  : Get erase block size in unit of sector
* Argument     : none
* Return value : Block size in bytes.
*******************************************************************************/
#if 0
static uint32_t sdcard_get_block_size(void)
{
    if(!g_CSDvalues.CSD_STRUCTURE) /* SDC ver 1.XX standard capacity */
    {
    } 
    else /* high capacity */
    {
    }
    return SDC_SECTOR_SIZE;
} /* end sdcard_get_block_size */
#endif

/*******************************************************************************
* Function Name: sdcard_get_num_blocks
* Description  : gets the number of logical blocks in the SdCard
* Argument     : none
* Return value : Number of blocks.
*******************************************************************************/
static uint32_t sdcard_get_num_blocks()
{
    uint32_t blocknr;
    uint16_t mult;
    
    
    if(!g_CSDvalues.CSD_STRUCTURE) /* SDC ver 1.XX standard capacity */
    {
        mult = (uint16_t)(2 << (g_CSDvalues.C_SIZE_MULT + 1));
        blocknr = (g_CSDvalues.C_SIZE + 1) * mult;
    } 
    else /* high capacity--not fully supported. */
    {
        /* Yields approximate value. */
        blocknr = (uint32_t)(sdcard_compute_size()/SDC_SECTOR_SIZE); 
    }    

    return blocknr;
} /* end sdcard_get_num_blocks */


/*******************************************************************************
* Function Name: sdcard_compute_size
* Description  : Calculates the user memory capacity of the sdcard
*              For version 1.x standard capacity cards, the memory capacity is
*              computed from the C_SIZE, C_SIZE_MULT and READ_BL_LEN as follows:
*                  memory capacity = BLOCKNR * BLOCK_LEN
*                  Where
*                      BLOCKNR = (C_SIZE+1) * MULT
*                      MULT = 2^(C_SIZE_MULT+2) (C_SIZE_MULT < 8)
*                      BLOCK_LEN = 2^READ_BL_LEN (READ_BL_LEN < 12)
*
*              For version 2.x high capacity cards, the memory capacity is
*              computed as follows:
*                  memory capacity = (C_SIZE+1) * 512K byte
*
* Argument     : none
* Return value : uint64_t; the memory capacity
*******************************************************************************/
static uint64_t sdcard_compute_size(void)
{
    uint32_t blocknr;
    uint16_t mult;
    uint16_t block_len;
    uint64_t capacity;
    
    
    if(!g_CSDvalues.CSD_STRUCTURE) /* SDC ver 1.XX standard capacity */
    {
        mult = (uint16_t)(2 << (g_CSDvalues.C_SIZE_MULT + 1));
        blocknr = (g_CSDvalues.C_SIZE + 1) * mult;
        block_len = (uint16_t)(2 << (g_CSDvalues.READ_BL_LEN - 1));
        
        capacity = (uint64_t)(blocknr * block_len);
    } 
    else /* high capacity */
    {
        capacity = (uint64_t)(g_CSDvalues.C_SIZE+1) * (uint64_t)0x80000;
    }    
    return capacity;
} /* end sdcard_compute_size */


/*******************************************************************************
* FAT Filesystem disk IO Driver Functions
*******************************************************************************/
/*******************************************************************************
* Function Name: sdcard_disk_read
* Description  :
* Argument     :     (uint8_t lun, the logical unit (device number) to read from
*                    uint8_t* read_buffer,     destination buffer for read data
*                    uint32_t sector,         the starting sector number
*                    uint8_t sector_count)    number of sectors to be read
*
* Return value :     R_RES_OK  = 0     Successful
*                    R_RES_ERROR = 1  R/W Error
* Note:    This function does not check for destination buffer overrun.
*          Make absolutely sure that enough memory has been allocated to accept 
*          the data that will be read. This is especially important when the 
*          sector count is greater than 1, because typically only 1 sector is 
*          read at a time.
*******************************************************************************/
static R_DRESULT sdcard_disk_read(uint8_t lun, 
                                  uint8_t* read_buffer, 
                                  uint32_t sector, 
                                  uint8_t sector_count)
{
    uint8_t count;
    
    for (count = 0; count < sector_count; count++)
    {
        if (R_SdcSectorRead((sector + count), read_buffer) != SDC_VALID)
        {
            return R_RES_ERROR;
        }
        if (sector_count > 1)
        {
            read_buffer += SDC_SECTOR_SIZE; // Advance read_buffer pointer.
        }
    }

    return R_RES_OK;
} /* end sdcard_disk_read */


/*******************************************************************************
* Function Name: sdcard_disk_write
* Description  :
* Argument     :    uint8_t lun,  the logical unit (device number) to write to
*                   const uint8_t* write_buffer,  source for data to be written from
*                   uint32_t sector,         the starting sector number
*                   uint8_t sector_count     number of sectors to be written
*                     
* Return value :    R_RES_OK  = 0     Successful
*                   R_RES_ERROR = 1  R/W Error
*******************************************************************************/
static R_DRESULT sdcard_disk_write(uint8_t lun, 
                                   uint8_t* write_buffer, 
                                   uint32_t sector, 
                                   uint8_t sector_count)
{
    uint8_t count;
    
    for ( count = 0; count < sector_count; count++)
    {
    
        if (R_SdcBlockWrite(write_buffer, sector+count, sector_count) != SDC_VALID)
        {
            return R_RES_ERROR;
        }

        if (sector_count > 1)
        {
            write_buffer += SDC_SECTOR_SIZE; // Advance write_buffer pointer.
        }
    }

    return R_RES_OK;
} /* end sdcard_disk_write */


/*******************************************************************************
* Function Name: sdcard_disk_initialize
* Description  :
* Argument     : (uint8_t lun)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/ 
static R_DSTATUS sdcard_disk_initialize(uint8_t lun)
{
    if (R_SdCardSlotInit())
    {
        return     R_RES_OK;
    } 
    else return STA_NOINIT;
} /* end sdcard_disk_initialize */


/*******************************************************************************
* Function Name: sdcard_disk_status
* Description  :
* Argument     : (uint8_t lun)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/ 
static R_DSTATUS sdcard_disk_status(uint8_t lun)
{
    return R_RES_OK;
} /* end  sdcard_disk_status*/


/*******************************************************************************
* Function Name: sdcard_disk_ioctl
* Description  :
* Argument     : (uint8_t lun, uint8_t command_code, uint32_t* data_buffer)
* Return value :     R_RES_OK  = 0     Successful
*
*******************************************************************************/      
static R_DRESULT sdcard_disk_ioctl(uint8_t lun, 
                                   uint8_t command_code, 
                                   uint32_t* data_buffer)
{
	R_DRESULT res = R_RES_OK;

    switch (command_code) {
        
        case GET_SECTOR_COUNT:
            *data_buffer = sdcard_get_num_blocks();
        break;
        
        case GET_SECTOR_SIZE:
        case GET_BLOCK_SIZE:        
            *data_buffer = (uint32_t)sdcard_get_sector_size();
        break;
        
        case CTRL_SYNC:
            /* todo */
        break;
        default:
        	res = R_RES_PARERR;
        break;
    }    
    return res;
} /* end sdcard_disk_ioctl */


/*******************************************************************************
* Function Name: sdcard_register_my_driver
* Description  : Register Filesystem disk IO Driver Functions
* Argument     : lun -
*                Logical unit number. Serves as index to drivers entry.
* Return value : none
*
*******************************************************************************/
static void sdcard_register_my_driver(uint8_t lun)
{
    /* Fill out the device driver functions structure with the pointers to the 
       coresponding functions. */
    g_mmc_fs_driver.LUN = lun;    /* Index. */    
    g_mmc_fs_driver.fp_disk_read  = &sdcard_disk_read;
    g_mmc_fs_driver.fp_disk_write = &sdcard_disk_write;
    g_mmc_fs_driver.fp_disk_init  = &sdcard_disk_initialize;
    g_mmc_fs_driver.fp_disk_stat  = &sdcard_disk_status;
    g_mmc_fs_driver.fp_disk_ctrl  = &sdcard_disk_ioctl;
    
    /* Insert the pointer to the driver structure into list */
    g_disk_driver_list[lun] = &g_mmc_fs_driver;
      
} /* end sdcard_register_my_driver */


/*******************************************************************************
* Function Name : R_SdCardDiskMount
* Description   : Mount/Unmount a logical drive
* Arguments     : lun -
*                    Logical unit number. Serves as index to drivers entry.
* Return value  : none
*
*******************************************************************************/
bool R_SdCardDiskMount(uint8_t lun)
{
    FRESULT res;
    FATFS* p_FS = &g_mmc_fatfs; /* point to the file system object for the mmc drive*/
            
    res = f_mount (lun, p_FS);      /* Mount/Unmount the drive */
    if(res == FR_OK)
    {
        return true;
    }
    return false; /* got an error */
} /* end R_SdCardDiskMount */


/*******************************************************************************
* Function Name: mmc_disk_format_filesystem()
* Description  : format the SdCard; only works on STD capacity card 1GB or less!
* Argument     :
* Return value :
*
*******************************************************************************/
#if 0
static bool mmc_disk_format_filesystem(void)
{
    FRESULT res;
    uint8_t lun = SDCARD_LUN;
    
    res = f_mkfs(lun, FDISK, 512);
    if(res == FR_OK)
    {
        return true;
    }
    return false; /* got an error */
    
} /* end mmc_disk_format_filesystem */

#endif
