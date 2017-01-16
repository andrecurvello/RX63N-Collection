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
* File Name     : disk_iostreams.h
* Version       : 1.0
* Device(s)     : RX63N
* Tool-Chain    : Renesas RX Standard Toolchain 1.0.0
* OS            : None
* H/W Platform  : YRDKRX63N
*        : 
*******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version                     
*******************************************************************************/

#ifndef DISK_IOSTREAMS_H
#define DISK_IOSTREAMS_H

#define PATH_MAXLEN 64	/* 63 characters for path string and 1 for null */

/* I/O stream low level function declarations */
long ios_disk_open(const char* , long);
long ios_disk_close(long);
long ios_disk_read(long, uint8_t*, long);
long ios_disk_write( long, const uint8_t*, long);
long ios_disk_lseek(long, long, long);


#endif