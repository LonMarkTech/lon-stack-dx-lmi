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

/*******************************************************************************
          File:        lcs_main.c

       Version:        1

     Reference:        None

       Purpose:        The main scheduler for the protocol stack.
                       The scheduler first initializes the eeprom
                       based on custom.h and custom.c. A node
                       reset is then performed. The physical layer
                       is initialized to enable ISRs (Interrupt Service
                       Routine). Finally, the scheduler gets into a loop
                       where it calls DoApp followed by Send fns of all the
                       layers followed by Receive functions of
                       all the layers.

          Note:        None

         To Do:        None

*******************************************************************************/
/*------------------------------------------------------------------------------
Section: Includes
------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "echstd.h"
#include "lcs.h"
#include "tmr.h"

/*------------------------------------------------------------------------------
Section: Constant Definitions
------------------------------------------------------------------------------*/
#define LED_TIMER_VALUE      1000  /* How often to flash in ms */
#define CHECKSUM_TIMER_VALUE 1000  /* How often to check config checksum? */
/*------------------------------------------------------------------------------
Section: Type Definitions
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Section: Globals
------------------------------------------------------------------------------*/
/* None */

/*------------------------------------------------------------------------------
Section: Local Globals
------------------------------------------------------------------------------*/
/* packetLossPercent is used to simulate packet losses in simulation
   mode. Valid values are 0-100. */
static uint8   packetLossPercent = 0;

/*------------------------------------------------------------------------------
Section: Function Prototypes
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
Section: Local Function Prototypes.
------------------------------------------------------------------------------*/
void PhysicalLinkStub(void);
uint32 RandomNumber(int from, int to);

/*------------------------------------------------------------------------------
Section: Function Definitions
------------------------------------------------------------------------------*/
void main(void)
{
	LCS_Init();

    /* Loop forever. 'done' used to silence the compiler from
       generating warning. Later on, 'done' can be used to stop the
       scheduler under certain conditions, if necessary. */

	while (1)
    {
		LCS_Service();
		
		TAKE_A_BREAK;
    } 
}

uint32 RandomNumber(int from, int to)
{
    uint32 randValue = rand();

    return ( from + (randValue % (to - from + 1)) );
}
/*********************************End of main.c********************************/

