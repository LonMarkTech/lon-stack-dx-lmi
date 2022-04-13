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

/********************************************************************
          File:        stktimer.c

       Version:        1.7

       Purpose:        general purpose 32 bit hardware timer
                       stack timer for timeouts profiling.

*********************************************************************/

/*------------------------------------------------------------------------------ 
Section: Includes
------------------------------------------------------------------------------*/ 
#include <eia709_1.h>
#include <node.h>
#include <stktimer.h>

/*-------------------------------------------------------------------
Section: Constant Definitions
-------------------------------------------------------------------*/
#define DPRB            0x01000000UL  /* Dual Port Ram Base on Arnewsh Board */
#define REGB            (DPRB + 0x1000UL)     /* Register Base = DPRB + 4 k  */

/* timer defines */
#define TGCR      (REGB + 0x580UL)  /* 16 bit timer general config register */

#define TRR3      (REGB + 0x5A4UL)  /* 16 bit timer reference register */
                                    /* or 32 bit if cascaded  with timer 4 */
#define TCN3      (REGB + 0x5ACUL)  /* 16 bit timer counter reg 3 or 32 bit 3 & 4 */
                                    /* or 32 bit if cascaded  with timer 4 */
                                    
#define TMR4      (REGB + 0x5A2UL)  /* 16 bit timer mode register 4 */
#define TRR4      (REGB + 0x5A6UL)  /* 16 bit timer reference register 4 */
#define TCR4      (REGB + 0x5AAUL)  /* 16 bit timer capture register 4 */
#define TCN4      (REGB + 0x5AEUL)  /* 16 bit timer counter register 4 */
#define TER4      (REGB + 0x5B6UL)  /* 16 bit timer event register 4 */

/*-------------------------------------------------------------------
Section: Type Definitions
-------------------------------------------------------------------*/

 

/* timer typedefs */

typedef struct    /* timer general config register  16 bit */
{
   unsigned cas4 : 1;   /* cascade timer 4 */
   unsigned frz4 : 1;   /* freeze timer 4 */
   unsigned stp4 : 1;   /* stop timer 4 */
   unsigned rst4 : 1;   /* reset timer 4 */
   unsigned gm2  : 1;   /* gate mode pin 2 */
   unsigned frz3 : 1;   /* freeze timer 3 */
   unsigned stp3 : 1;   /* stop timer 3 */
   unsigned rst3 : 1;   /* reset timer 3 */
   unsigned cas2 : 1;   /* cascade timer 2 */
   unsigned frz2 : 1;   /* freeze timer 2 */
   unsigned stp2 : 1;   /* stop timer  2 */
   unsigned rst2 : 1;   /* reset timer 2 */
   unsigned gm1  : 1;   /* gate mode pin 1 */
   unsigned frz1 : 1;   /* freeze timer 1 */
   unsigned stp1 : 1;   /* stop timer 1 */
   unsigned rst1 : 1;   /* reset timer 1 */
} TimerGenConfigReg;

typedef struct    /*  timer mode register 16 bit */ 
{
   unsigned ps   : 8;    /* prescaler */
   unsigned ce   : 2;    /* capture edge */
   unsigned om   : 1;    /* output mode */
   unsigned ori  : 1;    /* output ref interrupt enable */
   unsigned frr  : 1;    /* free run/restart */
   unsigned iclk : 2;    /* input clock source */
   unsigned ge   : 1;    /* gate enable */
} TimerModeReg;

typedef struct    /* timer event register 16 bit */
{
   unsigned     : 14;    /* unused */
   unsigned ref : 1;    /* output reference event */
   unsigned cap : 1;    /* input capture event */
} TimerEventReg;

/*-------------------------------------------------------------------
Section: Globals 
-------------------------------------------------------------------*/
/* pointers to timer registers  all are constant pointers to volatiles 
        volatile is needed to insure mem access occurs at each reference */
        
                
/* Timer 4 stuff */
static volatile TimerGenConfigReg  * const tgcrPtrGbl =
                                                         (volatile TimerGenConfigReg *) TGCR;
static volatile TimerModeReg  * const tmr4PtrGbl =
                                                         (volatile TimerModeReg *) TMR4;
static volatile uint16  * const trr4PtrGbl =
                                                         (volatile uint16 *) TRR4;
static volatile uint16  * const tcr4PtrGbl =
                                                         (volatile uint16 *) TCR4;
static volatile uint16  * const tcn4PtrGbl =
                                                         (volatile uint16 *) TCN4;
static volatile TimerEventReg  * const ter4PtrGbl =
                                                         (volatile TimerEventReg *) TER4;
/* cascaded types are 32 bit */
static volatile uint32  * const trr3PtrGbl =
                                                         (volatile uint32 *) TRR3;                                                       
static volatile uint32  * const tcn3PtrGbl =
                                                         (volatile uint32 *) TCN3;
        
        
/*-------------------------------------------------------------------
Section: Function Definintions
-------------------------------------------------------------------*/   

/*****************************************************************
Function:  StackTimerInit
Returns:   
Globals:   
Reference: 
Purpose:   initialize hardware timers
Comments:  
******************************************************************/     
        
        
uint32 * StackTimerInit(void)
{
   tgcrPtrGbl->cas4 = 1; /* 0 = don't cascade 3 and 4 */
   tgcrPtrGbl->frz4 = 0; /* ignore freeze pin */
   tgcrPtrGbl->stp4 = 0; /* normal unstopped operation */
   tgcrPtrGbl->rst4 = 0; /* reset timer */
   
   tmr4PtrGbl->ps = 0xff; /* prescaler */
   tmr4PtrGbl->ce = 0; /* input capture disabled */
   tmr4PtrGbl->om = 0; /* output mode pulse */
   tmr4PtrGbl->ori = 0; /* diable output reference interrupt */
   tmr4PtrGbl->frr = 0; /* free running counter */
   tmr4PtrGbl->iclk = 1; /* internal general system clock source */
   tmr4PtrGbl->ge = 0; /* tgate is ignored */
   
   *trr3PtrGbl = (uint32) 0xFFFFFFFFU; /* set reference to max all ones */
   
   ter4PtrGbl->ref = 1;  /* writing a 1 resets the event bit */
   ter4PtrGbl->cap = 1;  /* writing a 1 resets the event bit */
   
   *tcn3PtrGbl = 0;
   
   tgcrPtrGbl->rst4 = 1; /* start timer running */
   
   return(tcn3PtrGbl); /* 32 bit timer */
}

/************************stktimer.c********************************************/