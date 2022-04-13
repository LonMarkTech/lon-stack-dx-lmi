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
 * pal.c - Persistence Abstraction Layer
 *
 */


#include "echstd.h"
#include "pal.h"
#include "pal_internal.h"
#include "boot_util.h"
#if !PLATFORM_IS(SIM)
#include "MSP430.h"
#endif

//**************************
// Global or Static Data
PalTypeCurPageEntry palCurPageTable[PAL_CUR_PAGE_TABLE_MAX_ENTRIES];

PalNvmPageNum lastDynamicPageUsed = 0;

const PalPartitionTable palPartitionTable =
{
	PAL_PART_LOG_AREA_START_PAGE,
	PAL_PART_DYN_AREA_START_PAGE,
	PAL_PART_LAST_PAGE
};


//*********************************
// API Functions for External Non-volatile memory

EchErr PAL_Init(void)
{
	EchErr sts = ECHERR_OK;
	
#if !PLATFORM_IS(SIM)
// FB: DEBUG: send MCLK to pin 25 (TP28)
//P2SEL |= (BIT0);	// config as clock
//P2DIR |= (BIT0);	// send MCLK
// FB: DEBUG: use pin 25 (TP28) as signal for logic analyzer
P2SEL &= ~(BIT0);	// Config as I/O
P2DIR |= (BIT0);	// direction is output
P2OUT &= ~(BIT0);	// set bit to zero
//P2OUT |= (BIT0);	// set bit to one
#endif

#if PLATFORM_IS(SIM)
	PAL_SimInit();
#else
	PAL_SpiInit();
#endif

	sts = PAL_ExtScanForCurrentPages();

	return sts;
}

// Used for periodic refresh
void PAL_IoRefresh(void)
{
#if !PLATFORM_IS(SIM)
	PAL_SpiInit();
#endif
}

EchErr PAL_ExtWriteNvmBlockByType(const void *pBlockData, const UInt16 len, const PalBlockType blockType)
{
	EchErr sts = ECHERR_OK;
	PalPageHdr pageHdr;
	PalNvmPageNum nextPageNum;
	PalTypeCurPageEntry *pEntry;

	if ((len > PAL_EXT_BLOCK_SIZE) || (len == 0))
	{
		sts = ECHERR_INVALID_PARAM;
	}
	else
	{
		sts = PAL_ExtFindCreateBlockTypeEntry(blockType, &pEntry);

		if (sts == ECHERR_OK)
		{
			pageHdr.blockType = blockType;
			pageHdr.flags = 0xFF;
			nextPageNum = PAL_GetNextFreePageNum();
			pageHdr.seqNum = PAL_IncrementSeqNum(pEntry->seqNum);
			pageHdr.reserved = 0xFFFF;

			PAL_FillHeaderCrc(&pageHdr);

			sts = PAL_ExtWritePageSplit(&pageHdr, pBlockData, len, nextPageNum, 0);
			if (sts == ECHERR_OK)
			{
				pEntry->curPageNum = nextPageNum;
				pEntry->seqNum = pageHdr.seqNum;
			}
		}
	}
	return sts;
}

EchErr PAL_ExtReadNvmBlockByType(void *pBlockData, const UInt16 len, const PalBlockType blockType)
{		
	EchErr sts = ECHERR_OK;
	PalTypeCurPageEntry *pEntry;
	PalPageHdr pageHdr;

	if ((len > PAL_EXT_BLOCK_SIZE) || (len == 0))
	{
		sts = ECHERR_INVALID_PARAM;
	}
	else
	{
		sts = PAL_ExtFindBlockTypeEntry(blockType, &pEntry);

		if (sts == ECHERR_OK)
		{
			sts = PAL_ExtReadPageSplit(&pageHdr, pBlockData, len, pEntry->curPageNum);
			if (sts == ECHERR_OK)
			{
				if (!PAL_ValidateHdrCrc(&pageHdr))
				{
					sts = ECHERR_DATA_INTEGRITY;
					// FB: Do we try to recover from this at all?
				}
			}
		}
	}
	return sts;
}

// Find page numbers for all block types.
// Run once at startup.
EchErr PAL_ExtScanForCurrentPages(void)
{
	EchErr sts = ECHERR_OK;
	PalNvmPageNum page;
	PalNvmPageNum maxPage;
	PalPageHdr pageHdr;
	PalTypeCurPageEntry *pEntry;

	maxPage = palPartitionTable.lastPage;

	for (page = palPartitionTable.dynamicAreaStartPage; page <= maxPage; page++)
	{
		PAL_ExtReadPageHdr(&pageHdr, page);
		if ((pageHdr.blockType != PAL_BLOCK_TYPE_NONE) && (pageHdr.blockType != PAL_BLOCK_TYPE_ERASED))
		{
			sts = PAL_ExtFindCreateBlockTypeEntry(pageHdr.blockType, &pEntry);
			
			if (sts == ECHERR_OK)
			{
				// Update the cur page entry
				if (pEntry->seqNum < pageHdr.seqNum)
				{
					pEntry->curPageNum = page;
					pEntry->seqNum = pageHdr.seqNum;
				}
			}
		}
	}
	return sts;
}

//****************************
// API Functions for MCU Flash


EchErr PAL_McuReadMainFlashSeg(void *pSegData, const UInt16 len, const PalMcuMainSegNum segNum)
{
	EchErr sts = ECHERR_OK;
#if !PLATFORM_IS(SIM)
	int i;	// Could save on stack by using 'len' directly
	UInt8 __data20 *pFlash;	
	UInt8 *pData;	// Will be in RAM (__data16)	

	pFlash = (UInt8 __data20 *)(PAL_MCU_MAIN_START_ADDR + (segNum * (unsigned long)PAL_MCU_MAIN_SEG_SIZE));
    pData = (UInt8 *)pSegData;	// Use local for compilation problems

	// Write
	// Don't use CLIB function!
	// memcpy(pData, pFlash, len);
	// pFlash must be a __data20 pointer
	for	(i = 0; i < len; i++)
	{
		*pData++ = *pFlash++;
	}
#endif
	return sts;
}

EchErr PAL_McuWriteMainFlashSeg(const void *pSegData, const UInt16 len, const PalMcuMainSegNum segNum)
{
	EchErr sts = ECHERR_OK;
#if !PLATFORM_IS(SIM)
	int i;	// Could save on stack by using 'len' directly
	UInt8 __data20 *pFlash;
	UInt8 *pData;	// Will be in RAM (__data16)	

	pFlash = (UInt8 __data20 *)(PAL_MCU_MAIN_START_ADDR + (segNum * (unsigned long)PAL_MCU_MAIN_SEG_SIZE));
    pData = (UInt8 *)pSegData;	// Use local for compilation problems
	
	// Unlock the flash
	FCTL3 = FWKEY;
	FCTL1 = FWKEY + WRT;
	
	// Write
	// Don't use CLIB function!
	// memcpy(pFlash, pData, len);
	// pFlash must be a __data20 pointer
	for	(i = 0; i < len; i++)
	{
		*pFlash++ = *pData++;
	}

	// Lock the flash
	FCTL1 = FWKEY;
	FCTL3 = FWKEY +	LOCK;
#endif
	return sts;
}

EchErr PAL_McuEraseMainFlashSeg(const PalMcuMainSegNum segNum)
{
	EchErr sts = ECHERR_OK;
#if !PLATFORM_IS(SIM)
	UInt8 __data20 *segAddr;
	
	segAddr = (UInt8 __data20 *)(PAL_MCU_MAIN_START_ADDR + (segNum * (unsigned long)PAL_MCU_MAIN_SEG_SIZE));

	FCTL3 = FWKEY;
	FCTL1 = FWKEY + ERASE;

	*segAddr = 0;	// Dummy write to start erase

	while (FCTL3 & BUSY );
	FCTL1 = FWKEY;				
	FCTL3 = FWKEY +  LOCK;
#endif
	return sts;
}

EchErr PAL_McuEraseMainFlashBank(const PalMcuMainSegNum bankNum)
{
	EchErr sts = ECHERR_OK;
#if !PLATFORM_IS(SIM)
	UInt8 __data20 *bankAddr;
	
	bankAddr = (UInt8 __data20 *)(PAL_MCU_MAIN_START_ADDR + (bankNum * (unsigned long)PAL_MCU_MAIN_BANK_SIZE));

	FCTL3 = FWKEY;
	while (FCTL3 & BUSY );
	FCTL1 = FWKEY + MERAS;

	*bankAddr = 0;	// Dummy write to start erase

	while (FCTL3 & BUSY );
	FCTL1 = FWKEY;
	FCTL3 = FWKEY +  LOCK;
#endif
	return sts;
}

EchErr PAL_McuReadInfoFlashSeg(void *pSegData, const UInt16 len, const PalMcuInfoSegNum segNum)
{
	EchErr sts = ECHERR_OK;
#if !PLATFORM_IS(SIM)
	int i;	// Could save on stack by using 'len' directly
	UInt8 *pFlash;	// Info flash is alway in __data16 space
	UInt8 *pData;

	// Note that the Info segments actually progress toward lower memory
	pFlash = (UInt8 *)(PAL_MCU_INFO_SEG_A_ADDR - (segNum * PAL_MCU_INFO_SEG_SIZE));
	pData = (UInt8 *)pSegData;	// Use local for compilation problems
	
	// Read
	// Don't use CLIB function!
	// memcpy(pData, pFlash, len);
	for	(i = 0; i < len; i++)
	{
		*pData++ = *pFlash++ ;
	}
#endif
	return sts;
}

// This relies on the flash segment already being erased
EchErr PAL_McuWriteInfoFlashSeg(const void *pSegData, const UInt16 len, const PalMcuInfoSegNum segNum)
{
	EchErr sts = ECHERR_OK;
#if !PLATFORM_IS(SIM)
	int i;	// Could save on stack by using 'len' directly
	UInt8 *pFlash;	// Info flash is alway in __data16 space
	const UInt8 *pData;

	// Note that the Info segments actually progress toward lower memory
	pFlash = (UInt8 *)(PAL_MCU_INFO_SEG_A_ADDR - (segNum * PAL_MCU_INFO_SEG_SIZE));
	pData = (const UInt8 *)pSegData;	// Use local for compilation problems

	// Unlock the flash
	FCTL3 = FWKEY;
	FCTL1 = FWKEY + WRT;
	
	// Write
	// Don't use CLIB function!
	// memcpy(pFlash, pData, len);
	for	(i = 0; i < len; i++)
	{
		*pFlash++ = *pData++;
	}

	// Lock the flash
	FCTL1 = FWKEY;
	FCTL3 = FWKEY +	LOCK;
#endif
	return sts;
}

EchErr PAL_McuEraseInfoFlashSeg(const PalMcuInfoSegNum segNum)
{
	EchErr sts = ECHERR_OK;
#if !PLATFORM_IS(SIM)
	UInt8 *segAddr;	// Info flash is alway in __data16 space
		
	// Note that the Info segments actually progress toward lower memory
	segAddr = (UInt8*)(PAL_MCU_INFO_SEG_A_ADDR - (segNum * PAL_MCU_INFO_SEG_SIZE));

	FCTL3 = FWKEY;
	if ((segNum == PAL_MCU_INFO_SEG_A) && (FCTL3 & LOCKA))
	{
		// Unlock InfoA. Don't relock it
		FCTL3 = FWKEY + LOCKA;
	}

	FCTL1 = FWKEY + ERASE;

	*segAddr = 0;		// Dummy write to start erase

	while (FCTL3 & BUSY );
	FCTL1 = FWKEY;				
	FCTL3 = FWKEY +  LOCK;
#endif
	return sts;
}

// Error return ECHERR_PAL_UNKNOWN_VERSION may still mean the known data is usable.
// It is up to the caller to decide.
EchErr PAL_ReadHwInfoStruct(PalHwInfoStruct *pHwInfoStruct)
{
	EchErr sts = ECHERR_OK;
	UInt8 buf[PAL_MCU_INFO_SEG_SIZE];	// This is 128 bytes!
	UInt8 *pCksum;
	UInt8 *pByte;
	UInt8 i;

	// To allow downgrading FW, anticipate a larger struture than we know about.
	// Read the whole segment and compute the appropriate checksum.
	sts = PAL_McuReadInfoFlashSeg(buf, PAL_MCU_INFO_SEG_SIZE, PAL_MCU_INFO_SEG_A);
	if (sts == ECHERR_OK)
	{
		pCksum = &buf[0] + ((PalHwInfoStruct *)&buf[0])->structLen - 1;
		if (PAL_ComputeHwInfoChecksum((PalHwInfoStruct *)&buf[0]) != *pCksum)
		{
			sts = ECHERR_DATA_INTEGRITY;
		}
		else
		{
			// Don't use CLIB function!
			// memcpy(buf, pHwInfoStruct, sizeof(PalHwInfoStruct));
			pByte = (UInt8 *)pHwInfoStruct;
			for (i = 0; i < sizeof(PalHwInfoStruct); i++)
			{
				*pByte++ = buf[i];
			}
			// If the version doesn't match, let the caller know.
			// The available data may still be usable.
			if (pHwInfoStruct->version != PAL_HW_INFO_STRUCT_VER)
			{
				sts = ECHERR_PAL_UNKNOWN_VERSION;
			}
		}
	}
	return sts;
}

EchErr PAL_WriteHwInfoStruct(PalHwInfoStruct *pHwInfoStruct)
{
	EchErr sts = ECHERR_OK;
	
	// Enforce a few things
	pHwInfoStruct->version = PAL_HW_INFO_STRUCT_VER;
	pHwInfoStruct->structLen = sizeof(PalHwInfoStruct);
	
	pHwInfoStruct->checksum = PAL_ComputeHwInfoChecksum(pHwInfoStruct);
	
	sts = PAL_McuEraseInfoFlashSeg(PAL_MCU_INFO_SEG_A);
	if (sts == ECHERR_OK)
	{
		// We only need to write the structure bytes. The rest stays erased (0xFF)
		sts = PAL_McuWriteInfoFlashSeg(pHwInfoStruct, sizeof(PalHwInfoStruct), PAL_MCU_INFO_SEG_A);
	}
	return sts;
}

UInt8 PAL_ComputeHwInfoChecksum(PalHwInfoStruct *pHwInfoStruct)
{
	UInt8 cksum;
	UInt8 i;
	UInt8 *pByte;
	
	cksum = 0;
	pByte = (UInt8 *)pHwInfoStruct;
	for (i = 0; i < (pHwInfoStruct->structLen-1); i++)
	{
		cksum ^= *pByte++;
	}
	return cksum;
}

//**************************
// Internal PAL Functions

// External Flash

// Find the indicated page table entry, or the next free entry
EchErr PAL_ExtFindBlockTypeEntry(const PalBlockType blockType, PalTypeCurPageEntry **ppEntry)
{
	EchErr sts = ECHERR_OK;
	int i;

	// Find the cur page table entry
	i = 0;
	while ((i < PAL_CUR_PAGE_TABLE_MAX_ENTRIES) && (palCurPageTable[i].blockType != blockType) &&
		(palCurPageTable[i].blockType != PAL_BLOCK_TYPE_NONE))
	{
		i++;
	}
	if (i >= PAL_CUR_PAGE_TABLE_MAX_ENTRIES)
	{
		sts = ECHERR_OVERFLOW;
	}
	else
	{
		*ppEntry = &palCurPageTable[i];
		if (palCurPageTable[i].blockType != blockType)
		{
			// This entry could be used
			sts = ECHERR_NOT_FOUND;
		}
	}
	return sts;
}

EchErr PAL_ExtFindCreateBlockTypeEntry(const PalBlockType blockType, PalTypeCurPageEntry **ppEntry)
{
	EchErr sts = ECHERR_OK;

	sts = PAL_ExtFindBlockTypeEntry(blockType, ppEntry);
	if (sts == ECHERR_NOT_FOUND)
	{
		// Fill in mostly bogus info - it will get updated when actually used
		(*ppEntry)->blockType = blockType;
		(*ppEntry)->curPageNum = 0;
		(*ppEntry)->seqNum = PAL_SEQ_NUM_INVALID;
		sts = ECHERR_OK;
	}
	return sts;
}

PalPageSeqNum PAL_IncrementSeqNum(PalPageSeqNum seqNum)
{
	seqNum++;
	if ((seqNum == (PalPageSeqNum)(~0))	|| (seqNum == PAL_SEQ_NUM_INVALID))
	{
		seqNum = PAL_SEQ_NUM_MIN;
	}
	return seqNum;
}

Bool PAL_PageInUse(const PalNvmPageNum pageNum)
{
	Bool inUse = FALSE;
	int i;

	for (i = 0; ((i < PAL_CUR_PAGE_TABLE_MAX_ENTRIES) &&
				(palCurPageTable[i].blockType != PAL_BLOCK_TYPE_NONE));
				i++)
	{
		if (pageNum == palCurPageTable[i].curPageNum)
		{
			inUse = TRUE;
			break;
		}
	}
	return inUse;
}

PalNvmPageNum PAL_GetNextFreePageNum(void)
{
	PalNvmPageNum pageNum;

	// Self initialize the starting page number
	if (lastDynamicPageUsed == 0)
	{
		// TBD: random function, not from cLib?
		lastDynamicPageUsed	= palPartitionTable.dynamicAreaStartPage;
	}

	// Find a page that is not in use
	pageNum = lastDynamicPageUsed;
	do
	{
		// TBD: algorithm to pick next page? Just use sequential for now
		pageNum++;

	} while (PAL_PageInUse(pageNum));

	return pageNum;
}

void PAL_FillHeaderCrc(PalPageHdr *pPageHdr)
{
	if (PLATFORM_IS(SIM))
	{
		// We currently ignore the CRC for the Simulator
		pPageHdr->crc = 0;
	}
	else
	{
		pPageHdr->crc = BOOT_Crc16CcittLen(pPageHdr, (sizeof(PalPageHdr) - sizeof(pPageHdr->crc)));
	}
}

Bool PAL_ValidateHdrCrc(PalPageHdr *pPageHdr)
{
	if (PLATFORM_IS(SIM))
	{
		// We currently ignore the CRC for the Simulator
		return TRUE;
	}
	else
	{
		UInt16 crc;
		
		crc = BOOT_Crc16CcittLen(pPageHdr, (sizeof(PalPageHdr) - sizeof(pPageHdr->crc)));
		
		return (pPageHdr->crc == crc);
	}
}

EchErr PAL_ExtReadPageHdr(PalPageHdr *pPageHdr, const PalNvmPageNum page)
{
	return PAL_ExtReadPageSplit(pPageHdr, NULL, 0, page);
}

// FB: is this needed?
/*
EchErr PAL_ExtReadPage(UInt8 *pPageData, PalNvmPageNum page)
{
	// TBD
	return ECHERR_OK;
}
*/

// This may be a debug only func...
void PAL_ClearExtCurPageTable()
{
	int i;
	UInt8 *pData;

	pData = (UInt8 *)&palCurPageTable;
	for (i = 0; i < sizeof(palCurPageTable); i++)
	{
		*pData++ = 0;
	}
}





