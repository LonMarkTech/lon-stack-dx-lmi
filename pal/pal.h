// Copyright (C) 2022 Dialog Semiconductor
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of
// this software and associated documentation files (the "Software"), to deal in 
// the Software without restriction, including without limitation the rights to
// use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
// of the Software, and to permit persons to whom the Software is furnished to do
// so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

/*
 * pal.h - Persistence Abstraction Layer
 */

#ifndef PAL_H
#define PAL_H


C_API_START

//**************************
// External Definitions

// PAL Error Codes
#define ECHERR_PAL_UNKNOWN_VERSION ECHERR_SET_AREA(1,ECHERR_AREA_PAL)

// For external NVM, use "page" for physical unit, "block" for logical/user unit.

// Use generic scalar here. Enum definitions for actual type must be defined in a product-specific header.
typedef UInt8 PalBlockType;
typedef UInt16 PalMcuMainSegNum;
typedef UInt8 PalMcuInfoSegNum;
// This is not an actual flash address
typedef UInt16 PalNvmPageNum;
#define PAL_BLOCK_TYPE_NONE 0	// No type, used to invalidate pages
#define PAL_BLOCK_TYPE_ERASED (PalBlockType)(~0) // Flash erased state, all ones
#define PAL_BLOCK_TYPE_BEGIN 1	// First allowable type number

/* Example of product-specific types
typedef enum
{
	PAL_BLOCK_TYPE_NVCNFG = PAL_BLOCK_TYPE_BEGIN,	// First type shall be defined this way
	PAL_BLOCK_TYPE_ADDRTBL,

	PAL_BLOCK_TYPE_END			// Reserved, used to calculate the number of types
} PalSlbBlockTypes;
*/


// External flash user data blocks are 256 bytes
// Physical pages add 8 bytes for header
#define PAL_EXT_BLOCK_SIZE 256
#define PAL_EXT_PAGE_SIZE 264
#define PAL_EXT_NUM_PAGES 2048

//Internal flash sizes in bytes
#define PAL_MCU_MAIN_SEG_SIZE 512
#define PAL_MCU_INFO_SEG_SIZE 128
#define PAL_MCU_MAIN_BANK_SIZE (64*1024UL)
#define PAL_MCU_MAIN_SEGS_PER_BANK (PAL_MCU_MAIN_BANK_SIZE / PAL_MCU_MAIN_SEG_SIZE) // 128
// Total main segments is defined by the specific MCU
#define PAL_MCU_MAIN_NUM_SEGS_5437 512
#define PAL_MCU_MAIN_START_ADDR 0x5C00
#define PAL_MCU_INFO_SEG_A_ADDR 0x1980
#define PAL_MCU_INFO_SEG_A 0
#define PAL_MCU_INFO_SEG_B 1
#define PAL_MCU_INFO_SEG_C 2
#define PAL_MCU_INFO_SEG_D 3

// HW Info Struct kept in InfoA segment
/* Description of Layout:
 * Info flash layout version	1 byte, starting at 1 for this layout
 * Flash data length		1 byte, total length of structure, including checksum (20)
 * Magic number/mfg test use	4 bytes, ASCII for �ELON� (or defined by mfg test)
 * IEEE 802.15.4 MACID		8 bytes (mostly for ZigBee)
 * Temperature calibration		2 bytes (signed 16 bit), millivolts, little endian
 * Hardware revision of IEM	1 byte
 * Hardware revision of host product	1 byte
 * Locale code			1 byte (determines target area of use)
 * Checksum			1 byte, XOR checksum of previous bytes
 */
typedef struct
{
  UInt8 version;
  UInt8 structLen;
  UInt8 magicNumber[4];
  UInt8 macid[8];
  Int16 temperatureOffset;
  UInt8 iemHwRevision;
  UInt8 hostHwRevision;
  UInt8 localeCode;
  UInt8 checksum;
} PalHwInfoStruct;
#define PAL_HW_INFO_STRUCT_VER 1

// External NVM functions

EchErr PAL_Init(void);
void PAL_IoRefresh(void);
EchErr PAL_ExtWriteNvmBlockByType(const void *pBlockData, const UInt16 len, const PalBlockType blockType);
EchErr PAL_ExtReadNvmBlockByType(void *pBlockData, const UInt16 len, const PalBlockType blockType);
EchErr PAL_ExtWritePageSplit(const void *pPageHdr, const void *pPageData, const UInt16 len, const PalNvmPageNum page, UInt8 fillVal);
EchErr PAL_ExtWritePageMasked(const void *pPageData, const UInt16 offset, const UInt16 len, const PalNvmPageNum page);
EchErr PAL_ConvertImageOffset(UInt32 imageOffset, PalNvmPageNum *pPage, UInt16 *pBlockOffset);
EchErr PAL_ExtErasePage(const PalNvmPageNum page);

// MCU flash functions

// MCU main flash holds program code and const data
EchErr PAL_McuReadMainFlashSeg(void *pSegData, const UInt16 len, const PalMcuMainSegNum segNum);
EchErr PAL_McuWriteMainFlashSeg(const void *pSegData, const UInt16 len, const PalMcuMainSegNum segNum);
EchErr PAL_McuEraseMainFlashSeg(const PalMcuMainSegNum segNum);
EchErr PAL_McuEraseMainFlashBank(const PalMcuMainSegNum segNum);

// MCU info flash holds HW specific config data
EchErr PAL_McuReadInfoFlashSeg(void *pSegData, const UInt16 len, const PalMcuInfoSegNum segNum);
EchErr PAL_McuWriteInfoFlashSeg(const void *pSegData, const UInt16 len, const PalMcuInfoSegNum segNum);
EchErr PAL_McuEraseInfoFlashSeg(const PalMcuInfoSegNum segNum);
EchErr PAL_ReadHwInfoStruct(PalHwInfoStruct *pHwInfoStruct);
EchErr PAL_WriteHwInfoStruct(PalHwInfoStruct *pHwInfoStruct);

// Misc
void PAL_DebugFunc(int cmd);

C_API_END

#endif // PAL_H
