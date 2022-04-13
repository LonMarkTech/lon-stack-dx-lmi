//
// VniNvPoint.h
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

// Interface for the VniNvPoint class.
//
// Client representation of a network variable point defined in a 
// a particular monitor set.  

#if !defined(AFX_VNINVPOINT_H__DA69AF4C_14E2_11D3_80B2_00105A202B29__INCLUDED_)
#define AFX_VNINVPOINT_H__DA69AF4C_14E2_11D3_80B2_00105A202B29__INCLUDED_
#include "vnipointbase.h"
#include "vniInterfaces.h"
#include "vniTimer.h"

class VniNvTx;
class VniClientNvPoint;

/* This class represents a local or monitor network variable monitoring point. 
   */
template class VNICLIENT_DECL VniObjBase<class VniClientNvPoint>;
class VNICLIENT_DECL VniNvPoint : public VniObjBase<VniClientNvPoint>, public VniCommon
{
    friend class VniClientMonitorSet;
    friend class VniStack;
    friend class VniNvTx;

    /**************************************************************************
     *                      Management Functions                              *
     **************************************************************************/
protected:
        // This constructor must be used when creating a network variable as a
        // result of VniStack::CreateNetworkVariable.
    VniNvPoint(IN class VniNvPointInit& nvInitData);
        
        // This constructor is used used when creating temporary monitor sets.
        // If copy desc is TRUE, make a copy of the description, otherwise just point
        // to the user provided description.
    VniNvPoint(const void *description, int descLen, VniMonitorSet *pMonitorSet, boolean copyDesc = TRUE);


    virtual ~VniNvPoint();
public:
    
        // This method must be called after a permanent NvPoint is constructed, before
        // it can be used.
    VniSts init();

	    // Set the enable type for the nv point 
    VniSts setMonitorEnableType(VniMonitorEnableType enableType);

	    // Return the enable type for the nv point. 
    VniMonitorEnableType getMonitorEnableType() const;
    /**************************************************************************
     *                      Network Variable Access Functions                 *
     **************************************************************************/
        // General information concerning NV updates and fetches:
        // 1) getLocalValue never generates update or completion events.
        //    getNvValue always generates completion events.  setNvValue generates
        //    a completion event if and only if propagate is TRUE.
        // 2) Network reads and writes are queued (within the server). getNvValue always
        //    queues, setNvValue queues iff and only if propagate is true, and getLocalValue 
        //    never queues.
        // 3) If multiple updates are queued to the same synch nv, 
        //    the specified value is queued with the request.  
        //    The local nv value is written at the time that the write
        //    is initiated, UNLESS a local write has been performed since the
        //    propagate was requested.  This results in the propagate overwritting
        //    fetch responses, but NOT local writes that were requested after the propagate.
        // 4) If multiple updates are queued to the same NON-synch nv, 
        //    the MOST RECENTLY UPDATED value will be sent out, and all requests will
        //    receive completion events when the completion event for the first update
        //    comes in.  When the update is sent, the local NV value will be updated, 
        //    This results in the propagate overwritting fetch responses.
        //    Note that an update is suppressed only if it was queued BEFORE another 
        //    update was sent.  Thus if nothing is queued for NV X, and 5 updates requests
        //    are made quickly in a row, the NV will be updated twice.  The first update request
        //    will be sent out imediately.  The next 4 will be queued.  When the first completes,
        //    the second will be sent (using the most recent value), and when it completes
        //    completion events for requests 2 - 5 will delivered. 
        // 5) Fetching a value (from the network) overwrites the value, unless the 
        //    local value was written after the fetch request was made.
    
        // Get the local copy of the network variable value.
    VniSts getLocalValue(NvValue& nvValue);

        // Returns a time stamp representing when the value was last updated.
        // The stamp is recorded by the VNI server when the update occurs, and
        // passed into the VNI client whenever the client receives the update.
        // Thus the stamp refers to when the last value that the client has
        // seen was recieved by the server.
    const VniTimeStamp& lastUpdateTime(void);

        // Get the value of the network variable.  
        //  netAccessCondition 
        //      determines whether to access the local copy or read it from the 
        //      network.
        // mSecBeforeStale 
        //      indicates how old an NV value must be before it is considered stale
        // pMsgOverride
        //      If non-NULL, specifies the message parameters.  
        //      Otherwise they are retrieved from the network variable configuration.
        // userTag
        //      A tag value which can be used by the client to correlate NV sets and gets
        //      and thier respective completion events.  The client can retrieve this
        //      tag in the nvUpdateCompletes callback by calling getUserTag().
        //
        // The value is reported by the callback NvUpdateArrived followed by
        // NvUpdateComplete.
    VniSts getNvValue(NetAccessCondition netAccessCondition, 
                      Msec msecBeforeStale,
                      IN VniMsgOverride *pMsgOverride = NULL,
                      VniNvTagId userTag = VNI_NULL_NV_TAG_ID);

        // Sets the local copy of the network variable value, optionally 
        // propagating it on the network.  This method returns immediately.  
        // Completion notification is made via the NvUpdateComplete callback.
        // (expect an NvUpdateComplete callback, unless propagate = TRUE).
        //  nvValue
        //      The value and length of the network variable.
        //  propagate
        //      If true, propogate onto the network. 
        // pMsgOverride
        //      If non-NULL, specifies the message parameters.  
        //      Otherwise they are retrieved from the network variable configuration.
        //     
        // userTag
        //      A tag value which can be used by the client to correlate NV sets and gets
        //      and thier respective completion events.  The client can retrieve this
        //      tag in the nvUpdateCompletes callback by calling getUserTag().
    VniSts setNvValue(IN NvValue& nvValue, BOOLEAN propagate, 
                      IN VniMsgOverride *pMsgOverride = NULL,
                      VniNvTagId userTag = VNI_NULL_NV_TAG_ID);

        // Gets the most recent user tag.  Generally only useful when called during
        // the nvUpdateCompletes callback.
    VniNvTagId getUserTag();

    VniSts getLastUpdateLength(int &updateLength);  // Returns 0 if unknown.

    /**************************************************************************
     *                      Network Variable Callbacks                        *
     **************************************************************************/
    
        // This method is called when a network variable update arrives, due a bound
        // update, response to a poll or a fetch, or a request of a local value.
    virtual void nvUpdateOccurs(IN NvValue *pNvValue,
                                 NvUpdateType updateType, 
                                 IN VniIncomingAddress *pIncommingAddr) = 0;

        // This method is called when a network variable update is complete, and indicates
        // whether it was successful or not.
    virtual void nvUpdateCompletes(BOOLEAN success, NvUpdateType updateType) =0;

        // This method is called to inform the client that a fetch on the NV represented
        // by the point has failed.  The client can assume that the NV value is stale
        // until another nvUpdateOccurs comes in.  
        // Note that this is NOT sent when the client does an explicit fetch of the
        // NV, since the failure is reported as an nvUpdateCompletes event.
    virtual void nvCommFailure() = 0;

    /**************************************************************************
     *                    Monitor Set                                         *
     **************************************************************************/
        // Returns a pointer to the monitor to which this network variable belongs.
    VniMonitorSet *getMonitorSet(void) const;
        // Returns the first NV in the monitor set (for purposes of iteration).
    VniNvPoint *firstNvInMonitorSet() const;
        // Returns the next NV in the monitor set.
    VniNvPoint *nextNvInMonitorSet() const;

    /**************************************************************************
     *                    Attributes                                          *
     **************************************************************************/
    LtMsIndex msIndex() const;
    LtMpIndex mpIndex() const;
    const void *description() const;
    int  descLen() const;
    VniSts setDescription(const void *description, int descLen, boolean copyDesc = TRUE, boolean allocated = FALSE);
    int nvIndex() const;

        // Sets the monitoring options defined on the NvPoint.
        // pMsgOverride
        //      If non-NULL, specifies the service and message parameters.  
        //      Otherwise they are retrieved from the network variable configuration.
    VniSts setOptions(LtMpUpdateOptions updateOptions, 
                      LtMpPollInterval  pollInterval, 
                      LtMpNotifyOptions notifyOptions, 
                      LtMpNotifyInterval notifyInterval);

    LtMpUpdateOptions       updateOptions();
    LtMpPollInterval        pollInterval();
    LtMpNotifyOptions       notifyOptions();
    LtMpNotifyInterval      notifyInterval();
    VniSts setMsgOverride(VniMsgOverride &ltMsgOverride);
    VniSts getMsgOverride(VniMsgOverride &ltMsgOverride);

protected:
    void releaseObj();
};

#endif
