//
// VniMsgPoint.h
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

// Interface for the VniMsgPoint class.
//
//  Client representation of a message point defined in a 
//  a particular monitor set.  

#if !defined(AFX_VNIMSGPOINT_H__DA69AF4C_14E2_11D3_80B2_00105A202B29__INCLUDED_)
#define AFX_VNIMSGPOINT_H__DA69AF4C_14E2_11D3_80B2_00105A202B29__INCLUDED_
#include "vnidefs.h"
#include "vniInterfaces.h"
/* This class is used to send and recieve explicit messages. */
template class VNICLIENT_DECL VniObjBase<class VniClientMsgPoint>;
class VNICLIENT_DECL VniMsgPoint : public VniObjBase<class VniClientMsgPoint>, public VniCommon
{
    friend class VniClientStack;
    friend class VniClientMonitorSet;
    friend class VniMsgIn;
    /**************************************************************************
     *                      Management Functions                              *
     **************************************************************************/
protected:
    // This constructor must be used when creating a message points as a
        // result of VniStack::CreateNetworkVariable.
    VniMsgPoint(IN class VniMsgPointInit& msgInitData);


            // This constructor is used used when creating temporary monitor sets.
    VniMsgPoint(void *description, int descLen, VniMonitorSet *pMonitorSet, boolean copyDesc = TRUE);

    virtual ~VniMsgPoint();
    virtual void releaseObj();
public:
    /**************************************************************************
     *                    Attributes                                          *
     **************************************************************************/
    // Sets the monitoring options defined on the MsgPoint.

        // This method must be called after a permanent MsgPoint is constructed, before
        // it can be used.
    VniSts init();

    static VniMsgPoint *newObj(IN class VniMsgPointInit& msgInitData);
    static VniMsgPoint *newObj(void *description, int descLen, VniMonitorSet *pMonitorSet, boolean copyDesc = TRUE);
    VniSts setOptions(byte  filterCode, LtMpFilterOptions filterOptions);

    byte               filterCode();
    LtMpFilterOptions  filterOptions();

	        // Set the enable type for the msg point 
    VniSts setMonitorEnableType(VniMonitorEnableType enableType);

	    // Return the enable type for the msg point. 
    VniMonitorEnableType getMonitorEnableType() const;
    VniSts setMsgOverride(VniMsgOverride &ltMsgOverride);
    VniSts getMsgOverride(VniMsgOverride &ltMsgOverride);

    LtMsIndex               msIndex() const;
    LtMpIndex               mpIndex() const;
    int                     addrIndex() const;
    const void              *description() const;
    int                     descLen() const;
    VniSts setDescription(const void *description, int descLen, boolean copyDesc = TRUE, boolean allocated = FALSE);


    /**************************************************************************
     *                    Monitor Set Cross References                        *
     **************************************************************************/

        // Returns a pointer to the monitor to which this message belongs.
    VniMonitorSet *getMonitorSet(void) const;
        // Returns the first VniMsgPoint in the monitor set (for purposes of iteration).
    VniMsgPoint *firstMsgInMonitorSet() const;
        // Returns the next VniMsgPoint in the monitor set.
    VniMsgPoint *nextMsgInMonitorSet() const;

};

#endif

