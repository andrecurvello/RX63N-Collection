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
 
/****************************************************************************
* File Name        : sdcard.h
* Version         : 1.00
* Device         : generic
* Tool Chain     : HEW
* H/W Platform    : 
* Description     : definitions pertaining to the sdcard  
*            
******************************************************************************/
#ifndef SDCARD_H
#define SDCARD_H

#include <stdint.h>

typedef union
{
    unsigned char CSD_ARRAY[16];
    struct {
        unsigned char CSD_VERSION       :8;
        //            CSD_STRUCTURE     :2;    // 00b        R   [127:126]    CSD structure    
        //                              :6;    // 0000b      R   [125:120]    reserved        

        unsigned char TAAC              :8;    // xxh        R   [119:112]    data read access-time-1    
        unsigned char NSAC              :8;    // xxh        R   [111:104]    data read access-time-2 CLK cycles
        unsigned char TRAN_SPEED        :8;    // 32h or 5Ah R    [103:96]    max. data transfer rate 

        unsigned char CCC8              :8;         
        unsigned char CCC4_READ_BL_LEN  :8;    
        //            CCC               :12;// 01x110110101b R    [95:84]     card command classes    
        //            READ_BL_LEN       :4; // 9h        R        [83:80]     max read data block length
        
        unsigned char RBP_WBM_RBM_DSR_CSZ2 :8; // 1+1+1+1+2+2 
        //            READ_BL_PARTIAL   :1;    // 1b     R     [79:79]        partial blocks for read allowed    
        //            WRITE_BLK_MISALIGN:1;    // xb     R     [78:78]        write block misalignment
        //            READ_BLK_MISALIGN :1;    // xb     R     [77:77]        read block misalignment    
        //            DSR_IMP           :1;    // xb     R     [76:76]        DSR implemented
        //                              :2;    // 00b    R     [75:74]        reserved
        //            C_SIZE            :12;   // xxxh   R     [73:62]        device size
        unsigned char CSZ8               :8;   // 
        unsigned char CSZ2_VRMIN_VRMAX  :8;    // 2+3+3    
        //            VDD_R_CURR_MIN    :3;    // xxxb   R     [61:59]        max. read current @VDD min
        //            VDD_R_CURR_MAX    :3;    // xxxb   R     [58:56]        max. read current @VDD max

        unsigned char VWMIN_VWMAX_CSM2  :8;    // 3+3+2        
        //            VDD_W_CURR_MIN    :3;    // xxxb   R     [55:53]        max. write current @VDD min
        //            VDD_W_CURR_MAX    :3;    // xxxb   R     [52:50]        max. write current @VDD max
        //            C_SIZE_MULT       :3;    // xxxb   R     [49:47]        device size multiplier
        unsigned char CSM1_EBE_SSZ6     :8;    // 1+1+6
        //            ERASE_BLK_EN      :1;    // xb       R   [46:46]        erase single block enable
        //            SECTOR_SIZE       :7;    // xxxxxxxb  R  [45:39]        erase sector size
        unsigned char SSZ1_WGRPSZ       :8;    // 1+7
        //            WP_GRP_SIZE       :7;    // xxxxxxxb  R  [38:32]        write protect group size

        unsigned char WGE_R2W_WBL2      :8;    // 1+2+3+2
        //            WP_GRP_ENABLE     :1;    // xb     R     [31:31]        write protect group enable
        //                              :2;    // 00b    R     [30:29]        reserved (Do not use)    
        //            R2W_FACTOR        :3;    // xxxb   R     [28:26]        write speed factor    
        unsigned char WBL2_WBP          :8;    // 2+1+5
        //            WRITE_BL_LEN      :4;    // xxxxb  R     [25:22]        max. write data block length    
        //            WRITE_BL_PARTIAL  :1;    // xb     R     [21:21]        partial blocks for write allowed    
        //                              :5;    // 00000b R     [20:16]        reserved    
        
        unsigned char FFG_CP_PWP_TWP_FF :8;    // 1+1+1+1+2+2
        //            FILE_FORMAT_GRP   :1;    // xb     R/W(1) [15:15]        File format group    
        //            COPY              :1;    // xb     R/W(1) [14:14]        copy flag    
        //            PERM_WRITE_PROTECT :1;   // xb     R/W(1) [13:13]        permanent write protection    
        //            TMP_WRITE_PROTECT :1;    // xb     R/W    [12:12]        temporary write protection    
        //            FILE_FORMAT       :2;    // xxb    R/W(1) [11:10]        File format    
        //                              :2;    // 00b    R/W      [9:8]        reserved    
        
        unsigned char CSD_CRC           :8; // 7+1
        //            CSD_CRC           :7;    // xxxxxxxb  R/W   [7:1]        CRC    
        //                              :1;    // 1b    -         [0:0]        not used, always'1'
    }CSD_REG_V1;    /* The CSD Register Fields (CSD Version 1.0) */
    
    struct {
        unsigned char CSD_VERSION       :8;
        //            CSD_STRUCTURE     :2;    // 01b    R        [127:126]    CSD structure    
        //                              :6;    // 00 0000b  R     [125:120]    reserved
            
        unsigned char TAAC              :8;    // 0Eh    R        [119:112]    data read access-time-1    
        unsigned char NSAC              :8;    // 00h    R        [111:104]    data read access-time-2 CLK cycles
        unsigned char TRAN_SPEED        :8;    // 32h|5Ah|0Bh|2Bh R[103:96]    max data transfer rate 
            
        unsigned char CCC8              :8;         
        unsigned char CCC4_READ_BL_LEN  :8;    
        //            CCC               :12;   // 01x110110101b R   [95:84]    card command classes    
        //            READ_BL_LEN       :4;    // 9h       R        [83:80]    max read data block length
            
        unsigned char RBP_WBM_RBM_DSR_4 :8;    // 1+1+1+1+4
        //            READ_BL_PARTIAL   :1;    // 0b        R       [79:79]    partial blocks for read allowed    
        //            WRITE_BLK_MISALIGN :1;   // 0b        R       [78:78]    write block misalignment
        //            READ_BLK_MISALIGN :1;    // 0b        R       [77:77]    read block misalignment    
        //            DSR_IMP           :1;    // xb        R       [76:76]    DSR implemented
        //                              :6;    // 00 0000b  R       [75:70]    reserved
        
        unsigned char _2_C_SIZE_6_      :8;    // 2+6
        unsigned char _C_SIZE_MID_8_    :8;    // 
        unsigned char _C_SIZE_LAST_8    :8;
        //unsigned long C_SIZE            :22; // xxxxxxh   R       [69:48]    device size    
                        
        unsigned char EBE_SSZ6          :8;    // 1+1+6
        //                              :1;    // 0b        R       [47:47]    reserved
        //            ERASE_BLK_EN      :1;    // 1b        R       [46:46]    erase single block enable
        //            SECTOR_SIZE       :7;    // 7Fh       R       [45:39]    erase sector size        
        unsigned char SSZ1_WGRPSZ       :8;    // 1+7
        //            WP_GRP_SIZE       :7;    // 0000000b  R       [38:32]    write protect group size

        unsigned char WGE_R2W_WBL2      :8;    // 1+2+3+2
        //            WP_GRP_ENABLE     :1;    // 0b        R       [31:31]    write protect group enable
        //                              :2;    // 00b       R       [30:29]    reserved (Do not use)    
        //            R2W_FACTOR        :3;    // 010b      R       [28:26]    write speed factor    
        unsigned char WBL2_WBP          :8;    // 2+1+5
        //            WRITE_BL_LEN      :4;    // 9h        R       [25:22]    max. write data block length
        //            WRITE_BL_PARTIAL  :1;    // 0b        R       [21:21]    partial blocks for write allowed
        //                              :5;    // 00000b    R       [20:16]    reserved
            
        unsigned char FFG_CP_PWP_TWP_FF :8;    // 1+1+1+1+2+2
        //            FILE_FORMAT_GRP   :1;    // 0b        R/W     [15:15]    File format group    
        //            COPY              :1;    // xb        R/W     [14:14]    copy flag    
        //            PERM_WRITE_PROTECT :1;   // xb        R/W     [13:13]    permanent write protection    
        //            TMP_WRITE_PROTECT :1;    // xb        R/W     [12:12]    temporary write protection    
        //            FILE_FORMAT       :2;    // 00b       R/W     [11:10]    File format    
        //                              :2;    // 00b       R/W       [9:8]    reserved
        
        unsigned char CSD_CRC           :8;    // 7+1
        //            CSD_CRC           :7;    // xxxxxxxb  R/W       [7:1]    CRC    
        //                              :1;    // 1b    -             [0:0]    not used, always'1'
    }CSD_REG_V2;    /* The CSD Register Fields (CSD Version 2.0) */
}CSD_REG;

/* The CSD Register values decoded from the raw register bitfields */
typedef struct
{
    uint8_t CSD_STRUCTURE;      // CSD structure    
    uint8_t TAAC;               // data read access-time-1    
    uint8_t NSAC;               // data read access-time-2 CLK cycles
    uint8_t TRAN_SPEED;         // max. data transfer rate 
    uint16_t CCC;               // card command classes    
    uint8_t READ_BL_LEN;        // max read data block length
    uint8_t READ_BL_PARTIAL;    // partial blocks for read allowed    
    uint8_t WRITE_BLK_MISALIGN; // write block misalignment
    uint8_t READ_BLK_MISALIGN;  // read block misalignment    
    uint8_t DSR_IMP;            // DSR implemented
    uint32_t C_SIZE;            // device size
    uint8_t VDD_R_CURR_MIN;     // max. read current @VDD min
    uint8_t VDD_R_CURR_MAX;  // max. read current @VDD max
    uint8_t VDD_W_CURR_MIN;     // max. write current @VDD min
    uint8_t VDD_W_CURR_MAX;     // max. write current @VDD max
    uint8_t C_SIZE_MULT;        // device size multiplier
    uint8_t ERASE_BLK_EN;       // erase single block enable
    uint8_t SECTOR_SIZE;        // erase sector size
    uint8_t WP_GRP_SIZE;        // write protect group size
    uint8_t WP_GRP_ENABLE;      // write protect group enable
    uint8_t R2W_FACTOR;         // write speed factor    
    uint8_t WRITE_BL_LEN;       // max. write data block length    
    uint8_t WRITE_BL_PARTIAL;   // partial blocks for write allowed    
    uint8_t FILE_FORMAT_GRP;    // File format group    
    uint8_t COPY;               // copy flag    
    uint8_t PERM_WRITE_PROTECT; // permanent write protection    
    uint8_t TMP_WRITE_PROTECT;  // temporary write protection    
    uint8_t FILE_FORMAT;        // File format    
    uint8_t CSD_CRC;            // CRC    
}CSD_DECODED;

#endif