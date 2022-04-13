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
 * pal_sim_driver.c - Persistence Abstraction Layer simulator driver
 *
 */

#include "windows.h"
#include "stdio.h"
#include "memory.h"
#include "echstd.h"
#include "pal.h"
#include "pal_internal.h"
#ifdef _DEBUG_PAL
#include "upgrd.h"
#endif


// Simulated external flash
typedef struct
{
	union
	{
		struct
		{
			union
			{
				PalPageHdr palHdr;
#ifdef _DEBUG_PAL
				UpgrdPageHdr upgrdHdr;
#endif
				UInt8 hdrBytes[8];
			};
			byte userData[PAL_EXT_BLOCK_SIZE];
		};
		byte rawPageData[PAL_EXT_PAGE_SIZE];
	};
} PalExtSimPage;
PalExtSimPage palExtSimFlash[PAL_EXT_NUM_PAGES];

const char *palSimFlashFileName = "pal_sim_flash.dat";


void PAL_SimFlushFile()
{
	DWORD nWritten;
	HANDLE handle;

	handle = CreateFile(
					  palSimFlashFileName,          // pointer to name of the file
					  GENERIC_WRITE,       // access (write-only) mode
					  FILE_SHARE_READ,           // share mode
					  NULL,						 // pointer to security attributes
					  OPEN_EXISTING,  // how to create
					  (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH),  // file attributes
					  NULL         // handle to file with attributes to copy
					);

	if (handle == INVALID_HANDLE_VALUE)
	{
		printf("Failed to open simulated flash file!\007\n");
	}
	else
	{
		if (!WriteFile(handle, palExtSimFlash, sizeof(palExtSimFlash), &nWritten, NULL))
		{
			printf("Failed to write simulated flash file!\007\n");
		}
		CloseHandle(handle);
	}
}

void PAL_SimInit()
{
	DWORD nWritten;
	HANDLE handle;

	handle = CreateFile(
					  palSimFlashFileName,          // pointer to name of the file
					  GENERIC_READ,       // access (read-write) mode
					  FILE_SHARE_READ,           // share mode
					  NULL,						 // pointer to security attributes
					  OPEN_EXISTING,  // how to create
					  (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH),  // file attributes
					  NULL         // handle to file with attributes to copy
					);
	if (handle == INVALID_HANDLE_VALUE)
	{
		
		handle = CreateFile(
						  palSimFlashFileName,          // pointer to name of the file
						  GENERIC_WRITE,       // access (read-write) mode
						  FILE_SHARE_READ,           // share mode
						  NULL,						 // pointer to security attributes
						  CREATE_NEW,  // how to create
						  (FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH),  // file attributes
						  NULL         // handle to file with attributes to copy
						);
		if (handle == INVALID_HANDLE_VALUE)
		{
			printf("Failed to open/create simulated flash file!\007\n");
		}
		else
		{
			CloseHandle(handle);
			memset(palExtSimFlash, 0xFF, sizeof(palExtSimFlash));
			PAL_SimFlushFile();
		}
	}
	else
	{
		if (!ReadFile(handle, palExtSimFlash, sizeof(palExtSimFlash), &nWritten, NULL))
		{
			printf("Failed to read simulated flash file!\007\n");
		}
		CloseHandle(handle);
	}

	// Product specific init
	if (PRODUCT_IS(SLB))
	{
	}
	else if (PRODUCT_IS(ZIGBEE_IEM))
	{
	}

}

EchErr PAL_SimExtEraseChip()
{
	memset(palExtSimFlash, 0xFF, sizeof(palExtSimFlash));
	PAL_SimFlushFile();
	return ECHERR_OK;
}

EchErr PAL_ExtErasePage(const PalNvmPageNum page)
{
	memset(&palExtSimFlash[page], 0xFF, PAL_EXT_PAGE_SIZE);
	PAL_SimFlushFile();
	return ECHERR_OK;
}

// Write a full page, with up to a full block of data, but supply the header and data separately
// Page is automatically erased first
EchErr PAL_ExtWritePageSplit(const void *pPageHdr, const void *pPageData, const UInt16 len, const PalNvmPageNum page, UInt8 fillVal)
{
	EchErr sts = ECHERR_OK;
	PalExtSimPage *pPage;

	if ((len > PAL_EXT_BLOCK_SIZE) || (len == 0))
	{
		sts = ECHERR_INVALID_PARAM;
	}
	else
	{
		PAL_ExtErasePage(page);	
		pPage = &palExtSimFlash[page];
		memcpy(&pPage->hdrBytes, pPageHdr, sizeof(PalPageHdr));
		memcpy(&pPage->userData, pPageData, len);
		if (len < PAL_EXT_BLOCK_SIZE)
		{
			// Fill unwriten bytes with fillValue (0 for user data, 0xFF for upgrade data)
			memset(&pPage->userData[len], fillVal, (PAL_EXT_BLOCK_SIZE-len));
		}

		// Flush the sim flash to disk
		PAL_SimFlushFile();
	}

	return sts;
}

// Write some page data, potentially in the middle of the page. The remainder of the page is masked by 0xFF (thus unchanged)
// The offset is from the beginning of the page, not from the header.
// The page is assumed to have already been erased, and the portion to write has not yet been modified
EchErr PAL_ExtWritePageMasked(const void *pPageData, const UInt16 offset, const UInt16 len, const PalNvmPageNum page)
{
	EchErr sts = ECHERR_OK;
	PalExtSimPage *pPage;

	if ((offset + len > PAL_EXT_PAGE_SIZE) || (len == 0))
	{
		sts = ECHERR_INVALID_PARAM;
	}
	else
	{
		// No actual masking needed for the simulation
		pPage = &palExtSimFlash[page];
		memcpy(&pPage->rawPageData[offset], pPageData, len);

		// Flush the sim flash to disk
		PAL_SimFlushFile();
	}

	return sts;
}

EchErr PAL_ConvertImageOffset(UInt32 imageOffset, PalNvmPageNum *pPage, UInt16 *pBlockOffset)
{
	EchErr sts = ECHERR_OK;

	// TBD: determine starting page
	*pPage = 0;

	*pPage += imageOffset / PAL_EXT_BLOCK_SIZE; // Do not count header space
	*pBlockOffset = imageOffset % PAL_EXT_BLOCK_SIZE; // Do not count header space

	return sts;
}

// Read up to a full page, but return the header and user data separately.
// The user data may be skipped by supplying a NULL pointer.
EchErr PAL_ExtReadPageSplit(PalPageHdr *pPageHdr, void *pPageData, const UInt16 len, const PalNvmPageNum page)
{
	EchErr sts = ECHERR_OK;
	PalExtSimPage *pPage;

	if ((pPageData != NULL) && ((len > PAL_EXT_BLOCK_SIZE) || (len == 0)))
	{
		sts = ECHERR_INVALID_PARAM;
	}
	else
	{
		pPage = &palExtSimFlash[page];
		memcpy(pPageHdr, &pPage->palHdr, sizeof(PalPageHdr));
		if (pPageData != NULL)
		{
			memcpy(pPageData, &pPage->userData, len);
		}
	}
	return sts;
}

// A utility function for various test/debug actions
void PAL_DebugFunc(int cmd)
{
	PalNvmPageNum page;

	page = 0;	
	switch (cmd)
	{
	case 1:	
		PAL_ExtErasePage(page);	// set page to something real
		break;

	case 2:
		PAL_SimExtEraseChip();
		break;

	case 0:
	default:
		break;
	}
}


