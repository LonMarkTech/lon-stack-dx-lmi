//
// VniMonitorSet.h
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

// Interface for the VniMonitorSet class.
//
//  Client representation of a monitor set defined on a particular VNI.

#if !defined(AFX_VNIMONITORSET_H__DA69AF4C_14E2_11D3_80B2_00105A202B29__INCLUDED_)
#define AFX_VNIMONITORSET_H__DA69AF4C_14E2_11D3_80B2_00105A202B29__INCLUDED_
#include "vnidefs.h"
#include "vniInterfaces.h"

class VniPointInit;
class VniNvPointInit;
class VniMsgPointInit;
class VniClientMonitorSet;

class VniTemporaryNvParms;
class VniTemporaryMsgParms;
/* This class is used to open or close a monitor set, and contains VniNvPoint
   and VniNvMsgPoint objects.  When opening a monitor set, callbacks will be
   made asking the application to create VniNvPoint and VniMsgPoint objects.
*/
template class VNICLIENT_DECL VniObjBase<class VniClientMonitorSet>;
class VNICLIENT_DECL VniMonitorSet : public VniObjBase<VniClientMonitorSet>, public VniCommon
{

public:

    VniMonitorSet *firstOpenedMonitorSet(void) const;
    VniMonitorSet *nextOpenedMonitorSet(void) const;
    VniStack *stack() const;
    LtMsIndex msIndex() const;
    int incrementOpenCount();
    boolean isOpen();
    boolean isTemporary();

	    // Set the enable type for the monitor set.,
    VniSts setMonitorEnableType(VniMonitorEnableType enableType);

	    // Return the enable type for the monitor set. 
    VniMonitorEnableType getMonitorEnableType() const;

        // Set the default options for temporary NV points.
    VniSts setDefaultTemporaryNvPointOptions(LtMpUpdateOptions  updateOptions,
                                             LtMpPollInterval   pollInterval,
                                             LtMpNotifyOptions  notifyOptions,
                                             LtMpNotifyInterval notifyInterval);

    LtMpUpdateOptions  updateOptions()  const;
    LtMpPollInterval   pollInterval()   const;
    LtMpNotifyOptions  notifyOptions()  const;
    LtMpNotifyInterval notifyInterval() const;

        // Set the default options for temporary Msg points.
    VniSts setDefaultTemporaryMsgPointOptions(LtMpFilterOptions filterOptions);

    LtMpFilterOptions  filterOptions() const;

    const void *description() const;
    int         descLen() const;

    VniSts setDescription(const void *description, int descLen, boolean copyDesc = TRUE, boolean allocated = FALSE);

    /**************************************************************************
     *                      VniNvPoint Functions                              *
     **************************************************************************/

        // Get the first network variable in this monitor set.  Used to iterate
        // through all open network variables in the monitor set
    class VniNvPoint *getFirstNetworkVariablePoint(void) const;
        // Find the NvPoint with the specified mpIndex;
    class VniNvPoint *findNvPoint(LtMpIndex mpIndex) const;
        // Add an NV point to a temporary monitor set.
    VniSts addTemporaryNvPoint(VniNvPoint &nvPoint,
                               VniTemporaryNvParms *pAddTempNvParms,
                               LtMpUpdateOptions  updateOptions,
                               LtMpPollInterval   pollInterval,
                               LtMpNotifyOptions  notifyOptions,
                               LtMpNotifyInterval notifyInterval);

        // Same as above, but uses default options set by setDefaultTemporaryNvPointOptions.
    VniSts addTemporaryNvPoint(VniNvPoint &nvPoint,
                               VniTemporaryNvParms *pAddTempNvParms);

        // Add a temporary NV point for the specified NV.  If nvName is NULL, add all NVS.
    VniSts addTemporaryPointForNv(const char *nvName,
                                  LtMpUpdateOptions        updateOptions,
                                  LtMpPollInterval         pollInterval,
                                  LtMpNotifyOptions        notifyOptions, 
                                  LtMpNotifyInterval       notifyInterval);

    VniSts removeTemporaryNvPoint(VniNvPoint &nvPoint);

     /********* Callback functions **********/
        // This virtual method is called by the VniMonitorSet when the client must create
        // a network variable object, either when the monitor set is first opened, or
        // due to a configuration change.
        //  nvInitData
        //      Initialization data to be passed to the VniNvPoint constructor
        //               
        //  After creating the point VniNvPoint::init() must be called, prior to invoking any methods
        //  on the point.
    virtual class VniNvPoint *createNetworkVariablePoint(IN class VniNvPointInit& nvInitData) = 0;
        // This method is called 
    virtual void removeNetworkVariablePoint(IN VniNvPoint& nvToRemove) = 0;
    virtual void networkVariablePointModified(IN VniNvPoint& nvPoint) {}

    /**************************************************************************
     *                      VniMsgPoint Functions                          *
     **************************************************************************/
        // Get the first explicit msg in this monitor set.  Used to iterate
        // through all explicit msg in the monitor set
    class VniMsgPoint *getFirstMsgPoint(void) const;

        // Find the MsgPoint with the specified mpIndex;
    class VniMsgPoint *findMsgPoint(LtMpIndex mpIndex) const;

        // Add a Msg point to a temporary monitor set.
    VniSts addTemporaryMsgPoint(VniMsgPoint        &msgPoint,
                                VniTemporaryMsgParms *pAddTemporaryMsgParms,
                                LtMpFilterOptions  filterOptions);

        // Same as above, but use default options
    VniSts addTemporaryMsgPoint(VniMsgPoint        &msgPoint,
                                VniTemporaryMsgParms *pAddTemporaryMsgParms);

    VniSts removeTemporaryMsgPoint(VniMsgPoint  &msgPoint);

    /********* Callback functions **********/
        // This virtual method is called by the VniMonitorSet when the client must create
        // an explicit message object.
        //  msgInitData
        //      Initialization data to be passed to the VniMsgPoint constructor
        //               
        //  After creating the point VniMsgPoint::init() must be called, prior to invoking any methods
        //  on the point.
    virtual class VniMsgPoint *createMsgPoint(IN class VniMsgPointInit& explicitMsgInitData) = 0;
        // This method is called 
    virtual void removeMsgPoint(IN VniMsgPoint& msgPoint) = 0;
    virtual void msgPointModified(IN VniMsgPoint& msgPoint) {}

protected:
    VniMonitorSet(IN VniStack &stack); 
    virtual ~VniMonitorSet();
    virtual void releaseObj();

};

class VNICLIENT_DECL VniTemporaryNvParms
{
public:
    VniTemporaryNvParms();
    ~VniTemporaryNvParms();
	VniTemporaryNvParms& operator=( const VniTemporaryNvParms& nvParms);

    int                  getNLength();
    void                 setNLength(int nLength);
    LtNvType             getNvType();		  
    void                 setNvType(LtNvType nvType);
    boolean              getTargetIsOutput();  // true if target is an output
    void                 setTargetIsOutput (boolean targetIsOutput);
    boolean              getIsBound();
    void                 setIsBound(boolean isBound);
    boolean              getTargetIsLocal();
    void                 setTargetIsLocal(boolean targetIsLocal  );
    boolean              getReadByIndex();     // Normally TRUE
    void                 setReadByIndex(boolean readByIndex    );
    boolean              getWriteByIndex();    // Only if node supports write by index commands
    void                 setWriteByIndex(boolean writeByIndex   );
    int                  getTargetNvIndex();
    void                 setTargetNvIndex(int targetNvIndex);
    int                  getTargetSelector(); 
    void                 setTargetSelector(int targetSelector);
    VniOutgoingAddress  *getOutgoingAddress();
    VniOutgoingAddress  *setOutgoingAddress(VniOutgoingAddress *pOutAddr);
    int                  getFlags();			  // NV state flags (same as SD flags, see NdNetworkVariable.h)
    void                 setFlags(int flags);
    boolean              getWaitForCompletion();
    void                 setWaitForCompletion(boolean waitForCompletion);
    VniMonitorEnableType getEnableType();
    void                 setEnableType(VniMonitorEnableType enableType);

    struct VniAddTemporaryNvParms *getImplementor();

private:
    struct VniAddTemporaryNvParms *p;
};

class VNICLIENT_DECL VniTemporaryMsgParms
{
public:
    VniTemporaryMsgParms();
    ~VniTemporaryMsgParms();
	VniTemporaryMsgParms& operator=( const VniTemporaryMsgParms& msgParms);

    byte                    getFilterCode();
    void                    setFilterCode(byte filterCode);
    VniOutgoingAddress     *getOutgoingAddress();
    VniOutgoingAddress     *setOutgoingAddress(VniOutgoingAddress *pOutAddr);
    boolean                 getWaitForCompletion();
    void                    setWaitForCompletion(boolean waitForCompletion);
    VniMonitorEnableType    getEnableType();
    void                    setEnableType(VniMonitorEnableType enableType);
    boolean                 getUseImplicitAddress();
    void                    setUseImplicitAddress(boolean useImplicitAddress);
    int                     getImplicitAddressIndex();
    void                    setImplicitAddressIndex(int implicitAddressIndex);

    struct VniAddTemporaryMsgParms *getImplementor();
private:
    struct VniAddTemporaryMsgParms *p;
};


#endif
