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
 *	WinLdvl2.cpp
 *
 *  Maps LDV calls to VNI L2 MIP
 *
 */

#include <windows.h>

#include "echstd.h"
#include "vldv.h"
#include "LtMip.h"
#include "VniProtocolAnalyzerControl.h"
#include "time.h"
#include "direct.h"
#include "delayimp.h"

//#include "comnincl.h"

C_API_START
//#include "common.h"
PfnDliHook   __pfnDliNotifyHook = NULL;
PfnDliHook   __pfnDliFailureHook = NULL;
C_API_END

#define MAX_HANDLES 10

#define SICB_SIZE(s)		((s).len + 2)

#ifdef NO_VNI
#define	LDV32ALWAYS true
#else
#define LDV32ALWAYS false
#endif

typedef struct LtSicbQ
{
	struct LtSicbQ* pNext;
	BYTE data;
} LtSicbQ;

CRITICAL_SECTION ldvLock;
#define LDV_LOCK_Q EnterCriticalSection(&ldvLock);
#define LDV_UNLOCK_Q LeaveCriticalSection(&ldvLock);

class WinLdv : public VniProtocolAnalyzerControl
{
public:
	WinLdv()
	{
		pSicbs = NULL;
		bTerminating = false;
		InitializeCriticalSection(&ldvLock);
	}

	virtual void packetArrivedEx(TimeStampedPkt *pPacket, int length, int packetNumber);

	static WinLdv* getWinLdv(short handle, bool purge=false);

	static WinLdv* handles[MAX_HANDLES];

	LtSicbQ* pSicbs;

	bool bTerminating;
};

WinLdv* WinLdv::handles[MAX_HANDLES];

void WinLdv::packetArrivedEx(TimeStampedPkt *pPacket, int length, int packetNumber)
{
	// Add this to the queue.
	if (!bTerminating)
	{
		int len = SICB_SIZE(*(LtSicb*)(pPacket->packetData));
		LDV_LOCK_Q;
		LtSicbQ** pq = &pSicbs;
		LtSicbQ* p = (LtSicbQ*)malloc(sizeof(LtSicbQ) + len);
		p->pNext = NULL;
		memcpy(&p->data, pPacket->packetData, len);
		while (*pq != NULL) pq = &(*pq)->pNext;
		*pq = p;
		LDV_UNLOCK_Q;
	}
}

WinLdv* WinLdv::getWinLdv(short handle, bool purge)
{
	WinLdv* p = NULL;
	
	if (handle < MAX_HANDLES)
	{
		p = WinLdv::handles[handle];
		if (purge)
		{
			WinLdv::handles[handle] = NULL;
		}
	}
	return p;
}

LDVCode vldv_open(const char* pName, pShort handle)
{
	LDVCode rtn = LDV_NO_RESOURCES;

	for (int i=0; i<MAX_HANDLES; i++)
	{
		if (WinLdv::handles[i] == NULL)
		{
			HRESULT hr;
			WinLdv* p;

			try
			{
				p = new WinLdv;
			}
			catch (...)
			{
				break;
			}

			// We found a vacant slot.  Fill it in.
			WinLdv::handles[i] = p;
			rtn = LDV_OK;

			// Now start up the VNI client
			hr = p->open(true, pName);
			if (hr)
			{
				rtn = LDV_NOT_OPEN;
			}
			*handle = i;
			break;
		}
	}
	return rtn;
}

LDVCode vldv_close(short handle)
{
	LDVCode rtn = LDV_NOT_OPEN;

	WinLdv* p = WinLdv::getWinLdv(handle);
	if (p != NULL)
	{
		LtSicb temp;
		p->bTerminating = true;

		while (vldv_read(handle, &temp, sizeof(temp))==LDV_OK);

		WinLdv* p = WinLdv::getWinLdv(handle, true);

		if (p != NULL)
		{
			p->close();
			delete p;
			rtn = LDV_OK;
		}
	}
	return rtn;
}

LDVCode vldv_read(short handle, pVoid msg_p, short len)
{
	LDVCode rtn = LDV_NOT_OPEN;

	WinLdv* pLdv = WinLdv::getWinLdv(handle);

	if (pLdv)
	{
		rtn = LDV_NO_MSG_AVAIL;
		LtSicbQ* p = pLdv->pSicbs;

		if (p != NULL)
		{
			LtSicb* pSicb = (LtSicb*)&p->data;
			int inlen = SICB_SIZE(*pSicb);
			if (inlen <= len)
			{
				LDV_LOCK_Q;
				memcpy(msg_p, pSicb, inlen);
				pLdv->pSicbs = p->pNext;
				free(p);
				rtn = LDV_OK;
				LDV_UNLOCK_Q;
			}
			else
			{
				rtn = LDV_INVALID_BUF_LEN;
			}
		}
	}
	return rtn;
}

LDVCode vldv_write(short handle, pVoid msg_p, short len)
{
	UNUSED_ALWAYS(len);
	LDVCode rtn = LDV_NOT_OPEN;
	byte* pSicb = (byte*)msg_p;

	// Layer 2 VNI doesn't support local NM
	if (*pSicb>>4 == 0x01)
	{
		WinLdv* p = WinLdv::getWinLdv(handle);

		if (p != NULL)
		{
			rtn = LDV_OK;

			p->sendPacket(pSicb+2, *(pSicb+1));
		}
	}
	return rtn;
}

#pragma warning (disable:4706)
#undef STRICT
#include <delayhlp.cpp>
