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
          File:        apppgm.c

       Version:        1.7

       Purpose:        Sample Application Program.
                                 
*******************************************************************************/

/*------------------------------------------------------------------------------ 
Section: Includes
------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <lcs_node.h>

/*------------------------------------------------------------------------------
Section: Constant Definitions
------------------------------------------------------------------------------*/
#define LIGHT_DURATION 2   /* In Seconds. Can be a fraction too */

/*------------------------------------------------------------------------------
Section: Type Definitions
------------------------------------------------------------------------------*/
/* None */

/*------------------------------------------------------------------------------
Section: Globals 
------------------------------------------------------------------------------*/
MsTimer lightTimer;
MsTimer svcTimer;
Boolean lightOn;

int8   whichOutVar;
int    intSum;
long   longSum;

MsgTag tag0;
MsgTag tag1;


/*******************************************************************************
          Network Variable Definition Format

   Prior  Dir    SelHi SelLo    bind Turn   Serv     Auth 
   explode length snvtDesc snvtExt snvtType  rateEst  maxrEst
   arrayCnt nvName nvSdoc nvAddr
*******************************************************************************/
typedef nulong SNVT_count;

nint intOut;
NVDefinition intOutDef = 
{ 
   TRUE, NV_OUTPUT, 0,   1, FALSE, ACKD,    FALSE,		 
   0, sizeof(nint), 0x80, 0x30, 0,  0,  0,  
   0,  "intOut",  "intOutSD", &intOut
};
int16 intOutIndex;

SNVT_count longOut;
NVDefinition longOutDef = 
{ 
   FALSE, NV_OUTPUT, 0,   1, FALSE, ACKD,    FALSE,	
   0, sizeof(SNVT_count), 0x80, 0x30, 8,  0,  0,  
   0,  "longOut",  "longOutVar", &longOut
};
int16 longOutIndex;

nint intArrayOut[3];
NVDefinition intArrayOutDef = 
{ 
   TRUE, NV_OUTPUT, 0,   1, FALSE, ACKD,    FALSE,	
   0, sizeof(nint), 0x80, 0x38, 0,  0,  0,  
   3,  "intArrayOut",  "intArrayOutSD", intArrayOut
};
int16 intArrayOutIndex;

nint intIn; /* Polled */
NVDefinition intInDef = 
{ 
   FALSE, NV_INPUT, 0,   1, FALSE, ACKD,    FALSE,		
   0, sizeof(nint), 0xA0, 0x30, 0,  0,  0,  
   0,  "intIn",  "intInSD", &intIn
};
int16 intInIndex;

nlong longIn;
NVDefinition longInDef = 
{ 
   FALSE, NV_INPUT, 0,   1, FALSE, ACKD,    FALSE,		
   0, sizeof(nlong), 0x80, 0x30, 0,  0,  0,  
   0,  "longIn",  "longInSD", &longIn
};
int16 longInIndex;

nint intArrayIn[2];
NVDefinition intArrayInDef = 
{ 
   FALSE, NV_INPUT, 0,   1, FALSE, ACKD,    FALSE,		
   0, sizeof(nint), 0x80, 0x38, 0,  0,  0,  
   2,  "intArrayIn",  "intArrayInSD", intArrayIn
};
int16 intArrayInIndex;


/*------------------------------------------------------------------------------
Section: Local Function Prototypes
------------------------------------------------------------------------------*/
void LightOn(void);
void LightOff(void);

/*------------------------------------------------------------------------------
Section: Function Definitions
------------------------------------------------------------------------------*/
void DoApp(void)
{ 
   if (MsTimerExpired(&lightTimer))
   {
      LightOff();
   }

   if (MsTimerExpired(&svcTimer))
   {
	   ManualServiceRequestMessage();
	   //MsTimerSet(&svcTimer, 1000);
   }

   /* IOChanges should be called before checking the status of io pins */
   if (IOChanges(0) && gp->ioInputPin0)
   {
      intOut++;
      PropagateNV(intOutIndex);
         
      /* Update whole array */
      intArrayOut[0]++;
      intArrayOut[1]++;
      intArrayOut[2]++;
      PropagateNV(intArrayOutIndex);
          
   }

   /* Ignore incoming messages */
   msg_receive();
   
   /* Ignore incoming responses */
   resp_receive();
}

Status AppInit(void)
{
   /* Register Network variables */
   intOutIndex      = AddNV(&intOutDef);
   longOutIndex     = AddNV(&longOutDef);
   intArrayOutIndex = AddNV(&intArrayOutDef);
   intInIndex       = AddNV(&intInDef);
   longInIndex      = AddNV(&longInDef);
   intArrayInIndex  = AddNV(&intArrayInDef);

   /* Make sure we were successful in registering all variables */
   if (intOutIndex      == -1 ||
       longOutIndex     == -1 ||
       intArrayOutIndex == -1 ||
       intInIndex       == -1 ||
       longInIndex      == -1 ||
       intArrayInIndex  == -1)
   {
      return(FAILURE);
   }
       
   
   tag0        = NewMsgTag(BINDABLE);
   tag1        = NewMsgTag(BINDABLE);
   
   /* Make sure we got the tags successfully */
   if (tag0 == -1 || tag1 == -1)
   {
      return(FAILURE);
   }
   
   return(SUCCESS);
}

void  AppReset(void)
{
   lightOn = FALSE;
   MsTimerSet(&lightTimer, 0); /* Init to 0 */
   whichOutVar = 0;
   if (intOutIndex == -1 || longOutIndex == -1 || intArrayOutIndex == -1 ||
       intInIndex == -1 || longInIndex == -1 || intArrayInIndex == -1)
   {
      gp->resetOk = FALSE;
   }   
   MsTimerSet(&svcTimer, 1000);
}

void  MsgCompletes(Status stat, MsgTag tag)
{

}

void NVUpdateOccurs(int16 nvIndex, int16 nvArrayIndex)
{
   if (nvIndex == intInIndex)
   {
      intSum = intSum + intIn;
   }
   else if (nvIndex == longInIndex)
   {
      longSum = longSum + longIn;
   }
   else if (nvIndex == intArrayInIndex)
   {
      intSum = intSum + intArrayIn[nvArrayIndex];
   }
}


void  Wink(void)
{
   gp->ioOutputPin1 = TRUE;
   MsTimerSet(&lightTimer, LIGHT_DURATION * 1000);
}

void OfflineEvent()
{
}

void OnlineEvent()
{
}

void NVUpdateCompletes(Status status, int16 nvIndex, int16 nvArrayIndex)
{
}

void LightOn(void)
{
   gp->ioOutputPin1 = TRUE;
   MsTimerSet(&lightTimer, LIGHT_DURATION * 1000);
   lightOn = TRUE;
}

void LightOff(void)
{
   gp->ioOutputPin1 = FALSE;
   lightOn = FALSE;
}


/*********************************End of apppgm.c******************************/

 
