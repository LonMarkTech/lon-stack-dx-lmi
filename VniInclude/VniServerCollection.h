//
// VniServerCollection.h
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

// Interface for the VniServerCollection class.
//
// This (optional) class can be used to maintain a collection of 
// VniServerControl objects.  When used, it provides a single thread to
// process messages sent to all VniServerControl objects registered with it.
//
// To use this object:
//   Allocate an instance and add a reference to it.
//
//   For each VniServerControl object to be managed by this service, call
//      VniServerControl::registerWithServerCollection passing a pointer to this object.
//          This is done instead of calling "VniServerControl::createReaderThread".
//      Before closing the VniServerControl, call 
//      VniServerControl::deregisterWithServerCollection
//          This is done instead of calling "VniServerControl::waitForReaderThreadToQuit".
//
//   When done release the reference to it.
//  
// The VniServerControl::registerWithServerCollection and VniServerControl::deregisterWithServerCollection
// methods must not be called from the VniServerCollection reader thread.
//
// When a VniServerCollection is used, the throttle parameters come from it,
// and the parameters from the VniServerControl are ignored.

#if !defined(VNISERVERCOLLECTION_H__INCLUDED_)
#define VNISERVERCOLLECTION_H__INCLUDED_
#include "vnidefs.h"
#include "vniInterfaces.h"

class VniServerControl;
class VniClientServerCollection;

template class VNICLIENT_DECL VniObjBase<VniClientServerCollection>;

class VNICLIENT_DECL VniServerCollection : public VniObjBase<VniClientServerCollection>
{
public:
    static VniServerCollection* newObj(int maxEvents = 0, Msec maxEventPeriod = 100);
    void setThrottle(int maxEvents, Msec maxEventPeriod);
    void getThrottle(int &maxEvents, Msec &maxEventPeriod);
    class VniServerControl *getFirstServerControl(void) const;


protected:
	VniServerCollection(int maxEvents = 0, Msec maxEventPeriod = 100);
	virtual ~VniServerCollection();
    virtual void releaseObj();
};

#endif
