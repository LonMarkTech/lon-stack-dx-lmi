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
          File:        physical.c

       Version:        1.7

       Purpose:        Physical Layer Functions.

*********************************************************************/
/*------------------------------------------------------------------------------ 
Section: Includes
------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <eia709_1.h>
#include <node.h>
#include <physical.h>

/*-------------------------------------------------------------------
Section: Constant Definitions
-------------------------------------------------------------------*/
/* #define DEBUG  */


/*-------------------------------------------------------------------
Section: Type Definitions
-------------------------------------------------------------------*/
/* None */

/*-------------------------------------------------------------------
Section: Globals 
-------------------------------------------------------------------*/
/* None */

/*-------------------------------------------------------------------
Section: Local Function Prototypes
-------------------------------------------------------------------*/
/* None */

/*-------------------------------------------------------------------
Section: Function Definitions
-------------------------------------------------------------------*/

#ifdef SIMULATION
/*******************************************************************************
Function:  PhysicalLinkStub
Returns:   None
Reference: None
Purpose:   To transfer packets from link queue of current
           stack to link queue of all other stacks.

Comments:  Used only in simulation mode.
*******************************************************************************/
void PhysicalStub(void)
{
   uint16  sn;
   uint16 size1, size2;
   Byte  *phyQueuePtr1, *phyQueuePtr2;
   ProtocolStackData *rgp; /* Remote node's gp pointer. */
   char msg[100];

   phyQueuePtr1= gp->phyOutPriQHeadPtr;
   if (*phyQueuePtr1 == 1)
   {
      size1   = gp->phyOutPriBufSize;
      /* Transfer from current stack to every other stack. */
      for (sn = 0; sn < NUM_STACKS; sn++)
      {
         rgp = &protocolStackDataGbl[sn];
         if (gp == rgp)
         {
            /* Current stack. Don't want to receive our own pkt. */
            continue;
         }
         size2        = rgp->lkInBufSize;
         phyQueuePtr2 = rgp->lkInQTailPtr;
         if (size2 < size1)
         {
            sprintf(msg,
                    "PhysicalStub: Pri Pkt Lost in %d. Too Large\n",
                    sn+1);
            ErrorMsg(msg);
         }
         else if (*phyQueuePtr2 == 1)
         {
            sprintf(msg,
                    "PhysicalStub: Pri Pkt Lost in %d. No Space.\n",
                    sn+1);
            ErrorMsg(msg);
         }
         else if (RandomNumber(1,100) <= packetLossPercent)
         {
            sprintf(msg,
                    "PhysicalStub: Pri Pkt Lost in %d.\n",
                    sn+1);
            ErrorMsg(msg);
         }
         else
         {
            /* Copy the whole packet including the flag */
            memcpy(phyQueuePtr2, phyQueuePtr1, size1);
            /* Increment the Tail Ptr */
            rgp->lkInQTailPtr = rgp->lkInQTailPtr + size2;
            if (rgp->lkInQTailPtr ==
                rgp->lkInQ + size2 * rgp->lkInQCnt)
            {
               rgp->lkInQTailPtr = rgp->lkInQ;
            }
         }
      }
      /* Increment the head pointer of sending stack. */
      gp->phyOutPriQHeadPtr = gp->phyOutPriQHeadPtr +
                                  gp->phyOutPriBufSize;
      if (gp->phyOutPriQHeadPtr == gp->phyOutPriQ +
                                       gp->phyOutPriQCnt *
                                       gp->phyOutPriBufSize)
      {
         gp->phyOutPriQHeadPtr = gp->phyOutPriQ;
      }
      /* Make the queue item available again. */
      *phyQueuePtr1 = 0;
   }

   /* Do the same thing for non-priority queue. */
   phyQueuePtr1= gp->phyOutQHeadPtr;
   if (*phyQueuePtr1 == 1)
   {
      size1   = gp->phyOutBufSize;
      /* Transfer from current stack to every other stack. */
      for (sn = 0; sn < NUM_STACKS; sn++)
      {
         rgp = &protocolStackDataGbl[sn];
         if (gp == rgp)
         {
            /* Current stack. Don't want to receive our own pkt. */
            continue;
         }
         size2        = rgp->lkInBufSize;
         phyQueuePtr2 = rgp->lkInQTailPtr;
         if (size2 < size1)
         {
            sprintf(msg,
                    "PhysicalStub: Pri Pkt Lost in %d. Too Large\n",
                    sn+1);
            ErrorMsg(msg);
         }
         else if (*phyQueuePtr2 == 1)
         {
            sprintf(msg,
                    "PhysicalStub: Pri Pkt Lost in %d. No Space.\n",
                    sn+1);
            ErrorMsg(msg);
         }
         else if (RandomNumber(1,100) <= packetLossPercent)
         {
            sprintf(msg,
                    "PhysicalStub: Pri Pkt Lost in %d.\n",
                    sn+1);
            ErrorMsg(msg);
         }
         else
         {
            /* Copy the whole packet including the flag. */
            memcpy(phyQueuePtr2, phyQueuePtr1, size1);
            /* Increment the Tail Ptr */
            rgp->lkInQTailPtr = rgp->lkInQTailPtr + size2;
            if (rgp->lkInQTailPtr ==
                rgp->lkInQ + size2 * rgp->lkInQCnt)
            {
               rgp->lkInQTailPtr = rgp->lkInQ;
            }
         }
      }
      /* Increment the head pointer of sending stack. */
      gp->phyOutQHeadPtr = gp->phyOutQHeadPtr +
                                  gp->phyOutBufSize;
      if (gp->phyOutQHeadPtr == gp->phyOutQ +
                                       gp->phyOutQCnt *
                                       gp->phyOutBufSize)
      {
         gp->phyOutQHeadPtr = gp->phyOutQ;
      }
      /* Make the queue item available again. */
      *phyQueuePtr1 = 0;
   }
}
#endif

/*****************************************************************
Function:  PHYReset
Returns:   None
Reference: None
Purpose:   To Initialize the queue for physical layer. 
Comments:  Since the physical layer functions are interrupt based,
           we use a queue in which the first byte is used as a flag
           to indicate the status of the item. The rest of the
           queue item are the actual data to be processed.
           For Output Queue, flag = 1 => item needs to be sent.
                                         (i.e Phy needs to process)
                             flag = 0 => item has been sent.
                                         (i.e Link can reuse it)
           For Input Queue,  flag = 1 => item was received.
                                         (i.e Link needs to take it)
                             flag = 0 => item has been processed.
                                         (i.e Link layer took it)
Format of queue: flag pdusize lpduheader npdu crc
  size in bytes    1    2       
******************************************************************/
void PHYReset(void)
{
   Byte *p;
   uint16 i;

   /* Allocate and Initialize Output Queue */
   /* 6 = 2 CRC + 1 for header + 1 for flag + 2 for pdusize */
   gp->phyOutBufSize  = 
      DecodeBufferSize((uint8) eep->readOnlyData.nwOutBufSize) + 6;
   gp->phyOutQCnt     = 
      DecodeBufferCnt((uint8) eep->readOnlyData.nwOutBufCnt);

   gp->phyOutQ = AllocateStorage((uint16) (gp->phyOutBufSize * gp->phyOutQCnt));
   if (gp->phyOutQ == NULL)
   {
      ErrorMsg("PHYReset: Unable to init Output Queue.\n");
      gp->resetOk = FALSE;
      return;
   }

   /* Init all flags to 0 so that the queue is empty */
   p = gp->phyOutQ;
   for (i = 0; i < gp->phyOutQCnt; i++)
   {
      *p = 0;
      p = p + gp->phyOutBufSize;
   }
   gp->phyOutQHeadPtr = gp->phyOutQTailPtr = gp->phyOutQ;

   /* Allocate and Initialize Pri Output Queue */
   gp->phyOutPriBufSize = gp->phyOutBufSize;
   gp->phyOutPriQCnt    = 
      DecodeBufferCnt((uint8) eep->readOnlyData.nwOutBufPriCnt);

   gp->phyOutPriQ = AllocateStorage((uint16) (gp->phyOutPriBufSize * 
                                    gp->phyOutPriQCnt));
   if (gp->phyOutPriQ == NULL)
   {
      ErrorMsg("PHYReset: Unable to init Priority Output Queue.\n");
      gp->resetOk = FALSE;
      return;
   }
   /* Init all flags to 0 */
   p = gp->phyOutPriQ;
   for (i = 0; i < gp->phyOutPriQCnt; i++)
   {
      *p = 0;
      p = p + gp->phyOutPriBufSize;
   }
   gp->phyOutPriQHeadPtr = gp->phyOutPriQTailPtr = gp->phyOutPriQ;

   return;
}

/*****************************************************************
Function:  PHYSend
Returns:   None
Reference: None
Purpose:   To process outgoing messages in gp->phyOutQ or
           gp->phyOutPriQ
Comments:  None
******************************************************************/
void PHYSend(void)
{
#ifdef SIMULATION
   PhysicalSTUB();
#else
   PHYIO(); /* Call PHYIO to perform i/o (Pins, Leds etc) */

   if (macGbl.tpr)
   {
      return; /* There is no space to send. We wait */
   }

   if (*gp->phyOutPriQHeadPtr == 1)
   {
      /* need to test for buffer length before copy, PKT_BUF_LEN */
      /* then lost message */
      /* There is message waiting to be sent in pri queue */
      if (*(uint16 *)(gp->phyOutPriQHeadPtr+1) <= PKT_BUF_LEN)
      {
         macGbl.tl = *(uint16 *)(gp->phyOutPriQHeadPtr+1) ;
         macGbl.tc = 0;
         macGbl.priorityPkt = TRUE;
         memcpy(macGbl.tPkt, gp->phyOutPriQHeadPtr + 3, macGbl.tl);
         /* get delta backlog value */
         macGbl.deltaBLTx = macGbl.tPkt[0] & 0x3F; 
         if ((macGbl.tPkt[0] & 0x40) != 0)
         {
            macGbl.altPathBit = 1; /* set alternate path bit */
         }
         else
         {
            macGbl.altPathBit = 0; /* set alternate path bit */
         }
#ifdef DEBUG
         DebugMsg("PHYSend: Sending a new packet to SPM.");
#endif                  
         macGbl.tpr = TRUE;
      }
      else
      {
         ErrorMsg("PHYSend: Pri Message Length too"
                  " long for ISR buffer.\n");
      }
      *gp->phyOutPriQHeadPtr = 0;
      gp->phyOutPriQHeadPtr += gp->phyOutPriBufSize;
      if (gp->phyOutPriQHeadPtr == 
               gp->phyOutPriQ + gp->phyOutPriBufSize * 
                                gp->phyOutPriQCnt)
      {
               gp->phyOutPriQHeadPtr = gp->phyOutPriQ;
      }
   }
   else if (*gp->phyOutQHeadPtr == 1)
   {
      /* There is message waiting to be sent in non-pri queue */
      if (*(uint16 *)(gp->phyOutQHeadPtr+1) <= PKT_BUF_LEN)
      {
         macGbl.tl = *(uint16 *)(gp->phyOutQHeadPtr+1);
         macGbl.tc = 0;
         macGbl.priorityPkt = FALSE;
         memcpy(macGbl.tPkt, gp->phyOutQHeadPtr + 3, macGbl.tl);
         /* get delta backlog */
         macGbl.deltaBLTx = macGbl.tPkt[0] & 0x3F; 
         if ((macGbl.tPkt[0] & 0x40) != 0)
         {
            macGbl.altPathBit = TRUE; /* set alternate path bit */
         }
         else
         {
            macGbl.altPathBit = FALSE; /* set alternate path bit */
         }
#ifdef DEBUG
         DebugMsg("PHYSend: Sending a new packet to SPM.");
#endif          
         macGbl.tpr = TRUE;
      }
      else
      {
         ErrorMsg("PHYSend: Message Length too long "
                  "for ISR buffer.\n");
      }
      
      *gp->phyOutQHeadPtr = 0;
      gp->phyOutQHeadPtr += gp->phyOutBufSize;
      if (gp->phyOutQHeadPtr == 
               gp->phyOutQ + gp->phyOutBufSize * gp->phyOutQCnt)
      {
              gp->phyOutQHeadPtr = gp->phyOutQ;
      }
   }
   else
   {
      return; /* Nothing to send */
   }

   return;
#endif
}

/*****************************************************************
Function:  PHYReceive
Returns:   None
Reference: None
Purpose:   To process incoming messages
Comments:  Interrupt Driven
******************************************************************/
void PHYReceive(void)
{
   /* superceded by ISR function. Hence return right away */
   return;
}


/*************************End of physical.c*************************/
