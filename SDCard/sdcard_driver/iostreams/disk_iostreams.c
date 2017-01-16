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
* File Name        : disk_iostreams.c
* Version         : .1
* Device         : Renesas Generic MCU.
* Tool Chain     : HEW
* H/W Platform    : RDK Generic
* Description     : C library low level I/O stream support functions for 
*                : disk-like block devices            
******************************************************************************/

/*******************************************************************************
Includes   <System Includes> , "Project Includes"
*******************************************************************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "disk_iostreams.h"
#include "board/rdkrx63n/lowsrc.h"
#include "r_diskio.h"
#include "diskio_drivers.h"

#ifdef USE_USB
extern void SCSI_SetSenseDataAttention(void);
#endif

/* list of path/file name strings */
struct {
    uint8_t _filename[PATH_MAXLEN];    
    FIL f_handle;    
}g_OpenFilesList[IOS_DISK_FILEMAX+1] = {0};     


/*************************************************************************************
* Function definitions
************************************************************************************/

/*******************************************************************************
* Function Name    : ios_disk_open(const char *name, long  mode)
* Description    : implements file open() function for C stdio iostreams support
* Argument        : (const char *name, long  mode)
* Return value    : long the file number if successful, -1 if not.
*
*******************************************************************************/
long ios_disk_open(const char *name, long  mode)
{
    FRESULT res;                 /* FatFs function common result code */
    long fileno;
    uint8_t FAT_mode;            /* file open access mode */    
        
    /* scan file list for open slot available. Skip 0-2, reserved for stdin, stdout, stderr */
    for( fileno = IOS_DISK_FILEMIN; fileno <= IOS_DISK_FILEMAX; fileno++ ) 
    {
        if (!g_OpenFilesList[fileno]._filename[0]) /* found an empty slot. Use for this file. */
        {        
            FAT_mode = (uint8_t)(mode & 0x1F); /* just use the bottom 5 bits */
                
            strncpy((char*)g_OpenFilesList[fileno]._filename, (char*)name, PATH_MAXLEN);    
                            
            g_OpenFilesList[fileno]._filename[PATH_MAXLEN-1] = 0; /* always terminate it */
                
            res = f_open(&g_OpenFilesList[fileno].f_handle, (char *)g_OpenFilesList[fileno]._filename, FAT_mode);

            if (FR_OK == res)
            {            
                if (O_APPEND & mode) /* Move to end of the file to append data */
                {
                    res = f_lseek(&g_OpenFilesList[fileno].f_handle, g_OpenFilesList[fileno].f_handle.fsize); 
                }            
                return fileno;
            }
            else /* open failed, so free the file slot */
            {
                g_OpenFilesList[fileno]._filename[0] = 0; 
            }
        }
    }
    return -1;
}

/*******************************************************************************
* Function Name    : ios_disk_write()
* Description    : implements file write() function for C stdio iostreams support
*                 : File must be already open. 
* Argument        : ( long fileno, const uint8_t *buf, long  count )
* Return value    : long the number of bytes written if successful, -1 if not.
*
*******************************************************************************/
long ios_disk_write( long fileno, const uint8_t *buff, long  count )
{
    FRESULT res;                     /* FatFs function common result code */
    uint32_t written_count = 0;
    
    if (g_OpenFilesList[fileno]._filename[0])    /*Check if file has been opened */
    {
        res = f_write(&g_OpenFilesList[fileno].f_handle, buff, (UINT)count, (unsigned int*)&written_count);
        
        #ifdef USE_USB            
        SCSI_SetSenseDataAttention();  /* notify the MSC_SCSI channel that file has changed */  
        #endif
                
        if ((res == FR_OK) && ( written_count == count ))
        {
            return (long)written_count;
        }        
    }
    return -1;    
}

/*******************************************************************************
* Function Name    : ios_disk_read()
* Description    : implements file read() function for C stdio iostreams support
*                 : File must be already open. 
* Argument        : ( long fileno, uint8_t *buff, long count )
* Return value    : long the number of bytes read if successful, -1 if not.
*
*******************************************************************************/
long ios_disk_read( long fileno, uint8_t *buff, long count )
{
    FRESULT res;                     /* FatFs function common result code */
    uint32_t read_count = 0;
    
    if (g_OpenFilesList[fileno]._filename[0])    /*Check if file has been opened */
    {
        res = f_read(&g_OpenFilesList[fileno].f_handle, buff, (UINT)count, (unsigned int*)&read_count);
        if (res == FR_OK )
        {
            return (long)read_count;
        }            
    }
    return -1;
}

/*******************************************************************************
* Function Name    : ios_disk_close()
* Description    : implements file close() function for C stdio iostreams support
* Argument        : ( long fileno )
* Return value    : 0 if successful, -1 if not.
*
*******************************************************************************/
long ios_disk_close( long fileno )
{
    FRESULT res;                     /* FatFs function common result code */
    
    if (g_OpenFilesList[fileno]._filename[0])    /*Check if file has been opened */
    {
        res = f_close(&g_OpenFilesList[fileno].f_handle);
        if (res == FR_OK)
        {
            g_OpenFilesList[fileno]._filename[0] = 0; /* free the file list entry */
            return 0;
        }            
    }
    return -1;    
}

/*******************************************************************************
* Function Name    : ios_disk_lseek()
* Description    : implements file lseek() function for C stdio iostreams support
*                : Sets the position within the file, in bytes, for reading from 
*                : and writing to the file.
* Argument        : (long fileno, long offset, long base)
* Return value    : the new position for reading/writing returned as an offset 
*                : from the file beginning if successful, -1 if not.
*
*******************************************************************************/
long ios_disk_lseek(long fileno, long offset, long base)
{
    FRESULT res;                     /* FatFs function common result code */
    FIL* f_handle;
    
    /*
    The position within a new file should be calculated and set using the following
    methods, depending on the third parameter (base).
    (1) When base is 0: Set the position at offset bytes from the file beginning
    (2) When base is 1: Set the position at the current position plus offset bytes
    (3) When base is 2: Set the position at the file size plus offset bytes
    
    The value of the offset parameter may be positive or negative.
    If the new file position is negative or the file size is exceeded, an error occurs. 
    When the file position is set correctly, the new position for reading/writing is 
    returned as an offset from the file beginning; 
    when the operation is not successful, -1 is returned.
    */
    
    if (g_OpenFilesList[fileno]._filename[0])    /*Check if file has been opened */
    {
        f_handle = &g_OpenFilesList[fileno].f_handle; /* for readability */
        
        /* offset should be positive value in case of base 0 */
        if ((base == 0) && (offset <= f_handle->fsize) && (offset >= 0))
        {
            res = f_lseek(f_handle, (DWORD)offset);
            if (FR_OK == res )
            {
                return (long)f_handle->fptr;
            }            
        }
        /* offset may be positive or negative value in case of base 1 */
        else if ((base == 1) && (offset + (long)(f_handle->fptr) <= f_handle->fsize) && (offset + (long)f_handle->fptr >= 0))
        {
            res = f_lseek(f_handle, (DWORD)(offset + (long)f_handle->fptr));
            if (FR_OK == res )
            {
                return (long)f_handle->fptr;
            }    
        }
        /* offset should be negative value in case of base 2 */
        else if ((base == 2 )&& ((long)f_handle->fsize + offset <= f_handle->fsize) && ((long)f_handle->fsize + offset >= 0))
        {
            res = f_lseek(f_handle, (DWORD)((long)f_handle->fsize + offset));
            if (FR_OK == res )
            {
                return (long)f_handle->fptr;
            }    
        }
    }    
    return -1;    /* error */
}
