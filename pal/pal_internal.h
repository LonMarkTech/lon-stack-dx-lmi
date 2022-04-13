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
 * pal_internal.h - Persistence Abstraction Layer internal definitions
 */

#ifndef PAL_INTERNAL_H
#define PAL_INTERNAL_H

// Include product-specific headers for variable definitions
#include "pal_platform.h"

C_API_START

//***********************
// Internal Definitions

typedef UInt32 PalNvmAddr;
typedef UInt16 FlashBlockNum;
typedef UInt16 PalPageSeqNum;

#define PAL_SEQ_NUM_INVALID 0
#define PAL_SEQ_NUM_MIN 1

// Partition table for external flash
typedef struct
{
	PalNvmPageNum logAreaStartPage;
	PalNvmPageNum dynamicAreaStartPage;
	PalNvmPageNum lastPage;
} PalPartitionTable;

typedef struct
{
	PalBlockType blockType;
	PalNvmPageNum curPageNum;
	PalPageSeqNum seqNum;
	// TBD: implement block deletion, obsolete block detection and elimination
	UInt8			deleted : 1;	// block type has been deleted, but block(s) still exist
	UInt8			multiple : 1;	// obsolute blocks still exist
} PalTypeCurPageEntry;


// The actual number of types defined via separate product header must be validated against this value,
// to protect against the below table overflowing.

extern PalTypeCurPageEntry palCurPageTable[PAL_CUR_PAGE_TABLE_MAX_ENTRIES];


// Page header
// The header will come first in a page, then the user data


// This must equal 8 bytes
typedef struct
{
	PalBlockType	blockType;
	UInt8			flags;
	PalPageSeqNum	seqNum;
	UInt16			reserved;
	UInt16			crc;	// This must come at the end of the fixed header data.
							// If a user data CRC were added, it should take this slot, moving the header CRC up.
} PalPageHdr;
// Compile time assertion to check structure size
COMPILE_TIME_DEF_ASSERT(sizeof(PalPageHdr) == 8);

#define PAL_PAGE_HDR_FLAG_VALID 0x01	// Default bit value should be 1, to allow invalidating page without erasing


#if PLATFORM_IS(SIM)
void PAL_SimInit(void);
#endif

EchErr PAL_ExtScanForCurrentPages(void);
PalNvmPageNum PAL_GetNextFreePageNum(void);
PalPageSeqNum PAL_IncrementSeqNum(PalPageSeqNum seqNum);
void PAL_FillHeaderCrc(PalPageHdr *pPageHdr);
EchErr PAL_ExtFindBlockTypeEntry(const PalBlockType blockType, PalTypeCurPageEntry **ppEntry);
EchErr PAL_ExtFindCreateBlockTypeEntry(const PalBlockType blockType, PalTypeCurPageEntry **ppEntry);
EchErr PAL_ExtReadPageSplit(PalPageHdr *pPageHdr, void *pPageData, const UInt16 len, const PalNvmPageNum page);
EchErr PAL_ExtReadPageHdr(PalPageHdr *pPageHdr, const PalNvmPageNum page);
Bool PAL_ValidateHdrCrc(PalPageHdr *pPageHdr);
Bool PAL_ExtIsNvmBusy(void);
EchErr PAL_ExtWaitForNvmDone(UInt16 msecTimeout);
void PAL_DebugFunc(int cmd);
void PAL_ClearExtCurPageTable();
UInt8 PAL_ComputeHwInfoChecksum(PalHwInfoStruct *pHwInfoStruct);


//***********************
// SPI Definitions
// FB: rename Darron's FLA_ macros?

// Definitions for NVM SPI port
#define PAL_EXT_CHIP_SELECT()	(FLA_PORT1_OUT &= ~FLA_PIN_CS)
#define PAL_EXT_CHIP_DESELECT()	(FLA_PORT1_OUT |= FLA_PIN_CS)
#define IOCHARTXBUF				FLA_TXBUF
#define IOCHARRXBUF				FLA_RXBUF
#define UARTCSR                 FLA_STAT

// Mask bit definitions for SPI master side configuration
typedef enum
{
    MaskBitSlaveSelect                     = 0x10,  // mask bit for Slave Select/Deselect
    MaskBitSelectSlaveRelevant             = 0x20,  // check whether MaskBit_SelectSlave is necessary

} PalSpiConfigOptionsMask;

// Acceptable values for SPI master side configuration
typedef enum _SpiMasterConfigOptions
{
	PalSpiOptNull						   = 0,
	PalSpiOptSelectSlave				   = 0x30,
	PalSpiOptDeselectSlave				   = 0x20,

} PalSpiConfigOptions;

typedef struct
{
    UInt8* pData;                                // buffer address that holds the data
    UInt16 length;                               // length of the stream in bytes
} PalSpiByteStream;

#define PAL_EXT_INSTR_EXTEN_SIZE 4
typedef struct
{
	Byte	cmd;
	Byte	addrHigh;
	Byte	addrMid;
	Byte	addrLow;
	Byte	exten[PAL_EXT_INSTR_EXTEN_SIZE];	// For dummy bytes
} PalExtInstructionBlock;

// This definition is for page size 264
#define ATMEL_264_PAGE_ADDR_SHIFT 9

// Use enum to contain Atmel flash commands
enum
{
	EXT_FLASH_CMD_BLOCK_ERASE     = 0x50,		// Block Erase
	EXT_FLASH_CMD_PAGE_ERASE      = 0x81,		// Page Erase
	EXT_FLASH_CMD_BUF1_WRITE      = 0x84,		// Write data bytes to buf1
	EXT_FLASH_CMD_PROG_FROM_BUF1  = 0x88,		// Program flash from buf1
	EXT_FLASH_CMD_READ_DIRECT     = 0xD2,		// Read page directly (no buffer)
	EXT_FLASH_CMD_READ_STATUS     = 0xD7,		// Read Status Register
	EXT_FLASH_CMD_CHIP_ERASE1	  = 0xC7,		// Chip Erase 1 - a four-byte sequence
	EXT_FLASH_CMD_CHIP_ERASE2	  = 0x94,		// Chip Erase 2
	EXT_FLASH_CMD_CHIP_ERASE3	  = 0x80,		// Chip Erase 3
	EXT_FLASH_CMD_CHIP_ERASE4	  = 0x9A,		// Chip Erase 4
	EXT_FLASH_CMD_READ_STS_REG    = 0xD7,		// Read Status Register
};

#define EXT_FLASH_READY_BIT 0x80				// Ready/~busy bit
	
void PAL_SpiInit(void);

void PAL_ConfigureSpiMaster(PalSpiConfigOptions opt);

Bool PAL_SpiExecute(
               const PalSpiByteStream* sendByteStream,   // char stream to be sent to the memory(incl. instruction, address etc)
               PalSpiByteStream* recvByteStream,         // char stream to be received from the memory
               PalSpiConfigOptions optBefore,     // Pre-Configurations on the SPI master side
               PalSpiConfigOptions optAfter       // Post-Configurations on the SPI master side
              );

void PAL_SpiWriteDefaultBytes(UInt8 val, UInt16 len);

C_API_END

#endif // PAL_INTERNAL_H
