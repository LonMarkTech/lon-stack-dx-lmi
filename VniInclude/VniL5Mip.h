//
// VniL5Mip.h
//
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


// Definitions used to open a layer 5 MIP.  This object is designed to be 
// "stand-alone", in that it requires minimal VNI include files, and manages 
// the VniServerControl itself. 

#if !defined(VniL5Mip__INCLUDED_)
#define VniL5Mip__INCLUDED_

#ifdef VNICLIENT_EXPORTS
#define VNI_L5MIP_CLIENT_DECL __declspec(dllexport) 
#else
#define VNI_L5MIP_CLIENT_DECL __declspec(dllimport) 
#endif

#include "LtMip.h"

typedef unsigned char VniUniqueId[6];

class VNI_L5MIP_CLIENT_DECL VniL5Mip
{
public:
	VniL5Mip();
	virtual ~VniL5Mip();

    HRESULT start(const char *deviceName, VniUniqueId* pUniqueId, const char *pathName);
    HRESULT stop();

	// The MIP application is designed to accept an SICB stream and to deliver an SICB stream.  Therefore,
	// its interface is very simple.  It consists of a send routine and receive routine.
	//

	// Defined by the application for receipt of uplink SICBs.  The SICB contents must be copied by the 
	// function since it will be freed immediately upon return.  This function is called from a unique
	// thread context so it is OK to suspend (e.g. to send the SICB via a socket).
	virtual void receive(LtSicb* pSicb) = 0;

	// Used by the application to send SICBs downlink.  The SICB contents are copied by the called function.
	// The SICB memory must be freed by the caller.  This routine can return LT_NO_RESOURCES if the 
	// maximum number of allowed outgoing SICBs has been hit.  It is up to the caller to suspend and retry
	// in this case.
	HRESULT send(LtSicb* pSicb);

private:
	class VniClientL5Mip      *m_pImpl;
	class VniServerControl	  *m_pControl;
};
// Returns temporary error message string.  String will be overwritten
// on each call, so client should copy it if needed longer term.
VNI_L5MIP_CLIENT_DECL LPCSTR VniErrorMessage(HRESULT sts);

#undef VNI_L5MIP_CLIENT_DECL
#endif
