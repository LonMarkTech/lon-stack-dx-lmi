//
// VniPointBase.h
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

// Interface for the VniPointBase class, encapsulating
// common properties of the VniNvPoint and VniMsgPoint classes.
//
// Encapsulates common properties of the VniNvPoint and VniMsgPoint 
// classes.  

#if !defined(VNIPOINTBASE__INCLUDED_)
#define VNIPOINTBASE__INCLUDED_
#include "vnidefs.h"
#include "vniInterfaces.h"
class VNICLIENT_DECL VniPointBase : public VniDescription
{

public:
    virtual LtMsIndex msIndex() const = 0;
    virtual LtMpIndex mpIndex() const = 0;
    virtual VniMonitorSet *getMonitorSet(void) const = 0;
    virtual VniMonitorEnableType getMonitorEnableType() const = 0;

    virtual VniSts setMsgOverride(VniMsgOverride &ltMsgOverride) = 0;
    virtual VniSts getMsgOverride(VniMsgOverride &ltMsgOverride) = 0;

};

#endif
