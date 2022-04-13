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

//
// SLBsim.c : Defines the entry point for the console application.
//


#include "memory.h"
#include "module_platform.h"
#include "echstd.h"
#include "pal.h"

void palSimInit();
void palSimDebugFunc(int cmd);

int main(int argc, char* argv[])
{
	ErrSts sts;
	PalBlockType blockType;
	byte pageData[PAL_EXT_BLOCK_SIZE];
	byte *pPageData = &pageData[0];

	palSimInit();

	palSimDebugFunc(0);	// Option to erase flash, ...

	palScanForCurrentPages();

	blockType = 1;

	sts = palReadExtFlashPageByType(pPageData, blockType);

	memset(pageData, 0xA5, sizeof(pageData));
	sts = palWriteExtFlashPageByType(pPageData, blockType);

	sts = palReadExtFlashPageByType(pPageData, blockType);

	return 0;
}

