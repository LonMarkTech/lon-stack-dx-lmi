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
 * pal_platform.h - Persistence Abstraction Layer - Platform Specific Definitions
 */

#ifndef PAL_PLATFORM_H
#define PAL_PLATFORM_H

#include "pal.h"

#if PLATFORM_IS(SIM)

#define PAL_PART_LOG_AREA_START_PAGE 600	// FB: bogus? - allows one 150K image
#define PAL_PART_DYN_AREA_START_PAGE 700	// FB: bogus
#define PAL_PART_LAST_PAGE (100 - 1)

#else // !PLATFORM_IS(SIM)

#define PAL_PART_LOG_AREA_START_PAGE 600	// FB: bogus? - allows one 150K image
#define PAL_PART_DYN_AREA_START_PAGE 700	// FB: bogus
#define PAL_PART_LAST_PAGE (PAL_EXT_NUM_PAGES - 1)	

#endif // PLATFORM_IS(SIM)

// TBD: this should be tuned for the final product
#define PAL_CUR_PAGE_TABLE_MAX_ENTRIES 20

// External NVM block types
typedef enum
{
	PAL_BLOCK_TYPE_LCS_EEPROM = PAL_BLOCK_TYPE_BEGIN,	// First type shall be defined this way

	PAL_BLOCK_TYPE_END			// Reserved, used to calculate the number of types
} PalSlbBlockTypes;


#endif // PAL_PLATFORM_H