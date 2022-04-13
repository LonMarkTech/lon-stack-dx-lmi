//
// VniServerInfo.h
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

// Interface for the VniServerControlBase class.

#if !defined(AFX_VNISERVERCONTROLBASE_H__4A364AA2_12BC_11D3_80B2_00105A202B29__INCLUDED_)
#define AFX_VNISERVERCONTROLBASE_H__4A364AA2_12BC_11D3_80B2_00105A202B29__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VniDefs.h"
#include "RmoControl.h"

class VNIBASE_DECL VniServerInfo 
{
public:
	VniServerInfo();
	virtual ~VniServerInfo();

protected:
    VniSts lock(void);
    VniSts unlock(void);
	BOOLEAN isStartupPending();
	void startupPending();
	void clearServerInfo();

	RmoClientId  getServerClientId();
	void setServerClientId(RmoClientId clientId);
    DWORD        getServerProcessId();
	void		 setServerProcessId(DWORD processId);
    VniSts       getFatalError();
	void		 setFatalError(VniSts sts);
    const RmoObjectId&  getServerObjectControlId();
    void		 setServerObjectControlId(const RmoObjectId& objectId);
	BOOLEAN		 isServerProcessAlive();


private:
	int			m_lockCount;
    static void init(void *p, int size);
	SharedMemoryControlBlock    m_VniServerInfo;
	struct VniServerDirectory *getDir();
};

#endif // !defined(AFX_VNISERVERCONTROLBASE_H__4A364AA2_12BC_11D3_80B2_00105A202B29__INCLUDED_)
