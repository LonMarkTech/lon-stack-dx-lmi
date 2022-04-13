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

/*********************************************************************
          File:     	template.c

     Reference:      	None

       Purpose:      	A template file for the Adept version of the 
                     	Neuron C programming model.
                                 
          Note:        

      Comments:  		
*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <node.h>

/*-------------------------------------------------------------------
Section: Constant Definitions -- add #define statements here
-------------------------------------------------------------------*/

/*-------------------------------------------------------------------
Section: Type Definitions -- add typedefs here, perhaps for SNVTs...
-------------------------------------------------------------------*/

/*-------------------------------------------------------------------
Section: Globals: declare timers, MsgTag(s) and global variables here
-------------------------------------------------------------------*/
/*	Timer   	lightTimer;
	Boolean 	lightOn;

	int8   	whichOutVar;
	int    	intSum;
	long   	ongSum;

	MsgTag 	tag1; */

/*****************************************************************
          Network Variable Definition Format

   Prior  Dir    SelHi SelLo    bind Turn   Serv     Auth 
   Sync   length snvtDesc snvtExt snvtType  rateEst  maxrEst
   arrayCnt nvName nvSdoc

   Note that snvtDesc corresponds to the bit fields for the
   config|non-config options for authentication, priroity, service,
   synch, polled, offline, and config class. 0x0E is the default.
   You need a definition for each NV in the program. The examples below
   are for non-SNVT types--they use nint and nlong for data types.
*****************************************************************/
/*
API_NV_def intOutDef = 
{ 
   FALSE, NV_OUTPUT, 0,    0,   1, FALSE, ACKD,    FALSE, 
   FALSE, sizeof(nint), 0x0E, 0, 0,  0,  0,  
   0,  "intOut",  "intOutVar" 
};

API_NV_def longOutDef = 
{ 
   FALSE, NV_OUTPUT, 0,    0,   1, FALSE, ACKD,    FALSE, 
   FALSE, sizeof(nlong), 0x0E, 0, 0,  0,  0,  
   0,  "longOut",  "longOutVar" 
};

API_NV_def intArrayOutDef = 
{ 
   FALSE, NV_OUTPUT, 0,    0,   1, FALSE, ACKD,    FALSE, 
   FALSE, sizeof(nint), 0x0E, 0, 0,  0,  0,  
   2,  "intArrayOut",  "intArrayOutVar" 
};

API_NV_def intInDef = 
{ 
   FALSE, NV_INPUT, 0,    0,   1, FALSE, ACKD,    FALSE, 
   FALSE, sizeof(nint), 0x0E, 0, 0,  0,  0,  
   0,  "intIn",  "intInVar" 
};

API_NV_def longInDef = 
{ 
   FALSE, NV_INPUT, 0,    0,   1, FALSE, ACKD,    FALSE, 
   FALSE, sizeof(nlong), 0x0E, 0, 0,  0,  0,  
   0,  "longIn",  "longInVar" 
};

API_NV_def intArrayInDef = 
{ 
   FALSE, NV_INPUT, 0,    0,   1, FALSE, ACKD,    FALSE, 
   FALSE, sizeof(nint), 0x0E, 0, 0,  0,  0,  
   2,  "intArrayIn",  "intArrayInVar" 
};
*/

/* Beside the above declarations, you must provide a 16 bit integer to
hold the index of each NV declared above. In the example above, there
were 6 NVs--3 inputs and 3 outputs declared, so we need the following
declarations to go with them. */

/* Indices for network output variables */
/*	int16 intOut;
	int16 longOut;
	int16 intArrayOut; */

/* Indices for network input variables */
/*	int16 intIn;
	int16 longIn;
	int16 intArrayIn;
*/
/*-------------------------------------------------------------------
Section: Local Function Prototypes
-------------------------------------------------------------------*/


/*-------------------------------------------------------------------
Section: Function Definitions
	   There are 3 MANDATORY functions that the application must 
	   provide: DoApp(), AppReset(), and AppInit(). DoApp() is called
	   with each iteration of the scheduler. AppReset() is called 
	   whenever the node is reset. Its job is to re-register and 
	   re-initialize any NVs and message tags. AppInit() is called
	   on power up and is where you put all the one time only init-
         ializations.
        
         Programming DoApp() is similar to running in scheduler bypass
	   mode -- you have to do everything explicitly.
-------------------------------------------------------------------*/
void DoApp(void)
{ /*
   nint  *ip;
   nlong *lp;

   if (TimerExpired(&lightTimer))
   {
      LightOff();
   }

   if (IOChanges(0) && gp->ioInputPin0)
   {
      gp->msgOut.priorityOn             = FALSE;
      gp->msgOut.tag                    = tag1;
      gp->msgOut.code                   = 1;
      gp->msgOut.data[0]                = 5;
      gp->msgOut.len                    = 1;
      gp->msgOut.authenticated          = FALSE;
      gp->msgOut.service                = ACKD;
      gp->msgOut.addr.snode             = snodeAddr;
   S1:
      MsgSend();
         
      switch (whichOutVar)
      {
         case 0:
            ip = GetNVAddr(intOut);
            *ip = *ip + 1;
            NVUpdated(intOut);
            break;
         case 1:
            lp = GetNVAddr(longOut);
            *lp = *lp + 1;
            NVUpdated(longOut);
            break;
         case 2:
            /* Update whole array */
            ip = GetNVAddr(intArrayOut);
            ip[0] = ip[0] + 1;
            ip[1] = ip[1] + 1;
            ip[2] = ip[2] + 1;
            NVUpdated(intArrayOut);
            break;
         case 3:
            ip = GetArrayNVAddr(intArrayOut, 0); /* Index 0 */
            *ip = *ip + 1;
            NVArrayUpdated(intArrayOut, 0);
            break;
         case 4:
            ip = GetArrayNVAddr(intArrayOut, 1); /* Index 1 */
            *ip = *ip + 1;
            NVArrayUpdated(intArrayOut, 1);
      }
      whichOutVar = (whichOutVar + 1) % 5;
   }

   /* Ignore incoming messages */
   if(gp->msgReceive) 
   {
      MsgFree(); 
   }
   
   /* Ignore incoming responses */
   if (gp->respReceive)
   {
      RespFree();
   } */
}

void  AppReset(void)
{/*
   lightOn = FALSE;
   SetTimer(&lightTimer, 0); /* Init to 0 */
   whichOutVar = 0;
*/
}

void AppInit(void)
{ /*
   /* Register Network variables */
   intOut      = AddNV(&intOutDef);
   longOut     = AddNV(&longOutDef);
   intArrayOut = AddNV(&intArrayOutDef);
   intIn       = AddNV(&intInDef);
   longIn      = AddNV(&longInDef);
   intArrayIn  = AddNV(&intArrayInDef);
   
   tag1        = NewMsgTag(BINDABLE);
*/
}

void  MsgCompletes(Status stat, MsgTag tag)
{

}

void NVUpdateOccurs(int16 nvIndex)
{ /*
   nint  *ip;
   nlong *lp;

   if (nvIndex == intIn)
   {
      ip = GetNVAddr(intIn);
      intSum = intSum + *ip;
   }
   else if (nvIndex == longIn)
   {
      lp = GetNVAddr(longIn);
      longSum = longSum + *lp;
   }
   else if (nvIndex == intArrayIn)
   {
      /* gp->nvArrayIndex has the index value */
      ip = GetArrayNVAddr(intArrayIn, gp->nvArrayIndex);
      intSum = intSum + *ip;
   }
 */
}


void  Wink(void)
{
}

void OfflineEvent()
{
}

void OnlineEvent()
{
}

void NVUpdateCompletes(Status status, int16 nvIndex)
{
}

/*************************End of *************************/

 
