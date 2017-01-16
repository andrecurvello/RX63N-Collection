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
* File Name     : lowsrc.h
* Version       : 1.00
* Device        : RX
* Tool Chain    : RX Family C Compiler
* H/W Platform  :
* Description   : Header file of I/O Stream file
******************************************************************************/
/*******************************************************************************
* History : DD.MM.YYYY     Version     Description
*         : 27.06.2013
*******************************************************************************/

/*******************************************************************************
System Definitions & Global Variables
*******************************************************************************/
/*Number of I/O Stream*/
#define IOSTREAM 8     /* The maximum number of iostream files that can be open. */

/* file number */
#define STDIN  0                    /* Standard input (console)        */
#define STDOUT 1                    /* Standard output (console)       */
#define STDERR 2                    /* Standard error output (console) */
#define IOS_DISK_FILEMIN 3          /* The first file number available after stdin,stdout,stderr. */
#define IOS_DISK_FILEMAX IOSTREAM-1 /* For multiple file access need to provide */
                                    /* a range of file numbers, one unique number */
                                    /* for each open file */

#define _MOPENR    0x1
#define _MOPENW    0x2
#define _MOPENA    0x4
#define _MTRUNC    0x8
#define _MCREAT    0x10
#define _MBIN      0x20
#define _MEXCL     0x40
#define _MALBUF    0x40
#define _MALFIL    0x80
#define _MEOF      0x100
#define _MERR      0x200
#define _MLBF      0x400
#define _MNBF      0x800
#define _MREAD     0x1000
#define _MWRITE    0x2000
#define _MBYTE     0x4000
#define _MWIDE     0x8000
/* File Flags */
#define O_RDONLY 0x0001 /* Read only                                       */
#define O_WRONLY 0x0002 /* Write only                                      */
#define O_RDWR   0x0004 /* Both read and Write                             */
#define O_CREAT  0x0008 /* A file is created if it is not existed          */
#define O_TRUNC  0x0010 /* The file size is changed to 0 if it is existed. */
#define O_APPEND 0x0020 /* The position is set for next reading/writing    */
                        /* 0: Top of the file 1: End of file               */

/* Special character code */
#define CR 0x0d                     /* Carriage return */
#define LF 0x0a                     /* Line feed       */


/* _INIT_IOLIB function declaration */
void _INIT_IOLIB( void );

