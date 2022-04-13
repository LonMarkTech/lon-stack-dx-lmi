//
// VniStack.h
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

// Interface for the VniStack class.
//
//  Client representation of VNI.

#if !defined(VniStack__INCLUDED_)
#define VniStack__INCLUDED_
#include "vnidefs.h"
#include "VniMiscLt.h"

class VniClientStack;

template class VNICLIENT_DECL VniObjBase<class VniClientStack>;
class VNICLIENT_DECL VniStack : public VniObjBase<VniClientStack>, public VniCommon
{

public:
    virtual int release();

    /**************************************************************************
     *                      Stack Manipulation Functions.                     *
     **************************************************************************/

        // Used to iterate through all open stacks.  
    VniStack *nextOpenStack();
        // Used to iterate through all open stacks.
    VniStack *getFirstOpenStack();
    const char *vniName(void) const;

	    // Set the enable type for the stack as a whole.  This does not 
	    // effect the enable of monitor sets or monitor points, but 
	    // may effect the effective enable type of points.  Limited to this client
    VniSts setMonitorEnableType(VniMonitorEnableType enableType);

	    // Return the enable type of the stack is enabled (this client only).
    VniMonitorEnableType getMonitorEnableType() const;

    	// Set the enable type for the stack as a whole, for all processes.  This does not 
	    // effect the enable of monitor sets or monitor points, but 
	    // may effect the effective enable type of points.  If myClient 
	    // is true, the stack's is client type is set, otherwise the 
	    // the stack's global enable type is set.  Note that a global 
	    // set does not undo a client set.  A global 
	    // set invokes the vniMonitorEnableTypeUpdate callback
    VniSts setMonitorEnableTypeForAllProcesses(VniMonitorEnableType enableType);

	    // Return the enable type of the stack is enabled, for all processes
    VniMonitorEnableType getMonitorEnableTypeForAllProcesses() const;

    VniServerControl *vniControl();

    int maxOustandingMessages();
    void setMaxOustandingMessages(int max);
    boolean isOpen();

    VniSts prepareForBackup(void);
    VniSts backupComplete(void);
    VniSts readyForBackup(boolean &ready);
    VniSts waitForPendingInterfaceUpdates(void);
    
    // Register a message hook. If a network management message is "hooked", 
    // the stack will not process the network mangament command, and instead 
    // deliver the message as if it is an application message.  A message is hooked
    // if the first n bytes (including the code) of the message match the data pattern
    // exactly.
    VniSts registerHookedMessage(int filterSize, const byte *pFilterData); 
    // Deregister a specific filer, or specify size = 0 to deregister all.
    VniSts deregisterHookedMessage(int filterSize, const byte *pFilterData);

    /**************************************************************************
     *                      Stack Management Callbacks.                       *
     **************************************************************************/

	    // The following method is called by the VniStack to inform the
	    // client of a change in monitor enable status.
    virtual void vniMonitorEnableTypeUpdate(VniMonitorEnableType enableType, 
                                            VniMonitorEnableType previousEnableType, 
                                            boolean myClient) {}

    /**************************************************************************
     *                      Device Control                                    *
     **************************************************************************/

    VniSts goOffline();
    VniSts goUnconfigured();
    VniSts changeState(int newState);

    VniSts clearStatus();
	VniSts initiateReset();

    VniSts setEepromLock(boolean locked);

    VniSts sleep(boolean commIgnore);
    VniSts flush(boolean commIgnore);
    VniSts flushCancel();

    VniSts sendServicePinMessage();
    VniSts setServiceLed(boolean ledOn);

	VniSts setTxIdLifetime(int duration);

    // This method is called to send a command to the xdriver.  Note that it bypass normal 
    // message queueing.
    VniSts sendXdriverCommand(byte xDriverCommand, void *pData, int length);

    VniSts setErrorLog(byte errorNumber);
    /**************************************************************************
     *                      Device Status                                     *
     **************************************************************************/

    VniSts getReadOnlyData(byte* readOnlyData, int maxLen, int &actualLen);
    VniSts retrieveStatus(VniStatus& status);
        // get the lowest & highest layers implemented on the host. 
        // 3,6 = Vni running on a Layer 2 mip.  6,6 = VNI running on Layer 5 mip.
    VniSts getLayerRange(int &bottom, int &top); 
    VniSts getNmVersion(int &nmVer, int &capabilities);

    /**************************************************************************
     *                      Device Configuration                              *
     **************************************************************************/

    VniSts setAuthenticationKey(int domainIndex, byte* key);
	VniSts getDomainConfiguration(int nIndex, VniDomainConfiguration* pDc);
    VniSts updateDomainConfiguration(int nIndex, VniDomainConfiguration* pDomain, boolean bGroups = true);

    VniSts getAddressConfiguration(int nIndex, VniAddressConfiguration* pAc);
    VniSts updateAddressConfiguration(int nIndex, VniAddressConfiguration* pAddress);

    VniSts getNvConfiguration(int nIndex, VniNetworkVariableConfiguration* pNvc);
    VniSts updateNvConfiguration(int nIndex, VniNetworkVariableConfiguration* pNvc);
    VniSts getNvAttributes(IN int        nIndex, 
                           OUT int      &nLength,		// Network variable length (for array, for each element)
	                       OUT LtNvType &nvType,		// NV type
	                       OUT int      &arrayLength,   // Array length, 0 for scalar.
	                       OUT boolean  &isOutput,		// true if output
	                       OUT int      &flags,		    // SD flags (see NdNetworkVariable.h)
	                       OUT VniStr   &name,			// NV name
                           OUT VniStr   &selfDoc,	    // Self doc string
	                       OUT int      &rateEstimate,	  // Rate estimate
	                       OUT int      &maxRateEstimate);  // Max rate estimate

    VniSts getConfigurationData(byte* pData, int offset, int length);
    VniSts updateConfigurationData(byte* pData, int offset, int length);

	VniSts updateSubnetNode(int domainIndex, int subnetNodeIndex, const VniSubnetNode &subnetNode);
	VniSts getSubnetNode(int domainIndex, int subnetNodeIndex, VniSubnetNode &subnetNode);

    /**************************************************************************
     *                      Comm Paramaters                                   *
     **************************************************************************/
        // Comm Parms.
    VniSts setXcvrId(int xcvrId);
    VniSts getXcvrId(int &xcvrId);
    VniSts setCommParameters(VniCommParams& commParams);
    VniSts setXcvrReg(byte* xcvrReg);

    /**************************************************************************
     *                      General Device Callbacks                          *
     **************************************************************************/

    /**
     * This method is invoked when the LonTalk Stack resets.  A reset could result
     * from a network management command or via the reset pin of the LON-C block.
     */
    virtual void reset() {};

    /**
     * This method is invoked when a wink request is received over the network.
     */
    virtual void wink() {};

    /**
     * This method is invoked when an offline request is received over the network.
     */
    virtual void offline() {};

    /**
     * This method is invoked when an online request is received over the network.
     */
    virtual void online() {};

    /**
     * This method informs the application when the service pin has been depressed.  The
     * protocol stack automatically sends a service pin message as a result of service pin 
     * depression.  This callback allows the application to do additional actions if so
     * desired.
     */
    virtual void servicePinPushed() {};

    /**
     * This method informs the application when a flush request has completed.
     */
    virtual void flushCompletes() {};

	/**
	 * This method is called when an application's persistent data is lost.
	 * @param reason
	 *			Reason for data loss.
	 */
	virtual void persistenceLost(VniPersistenceLossReason reason) {};

    /**
     * This method is called when a network interface event occurs (Xdriver events).
     *
     */
    virtual void networkInterfaceEventArrived(NetworkInterfaceEventType eventType) {};

    /**************************************************************************
     *                      Monitor Set Functions.                            *
     **************************************************************************/
        // Set afterMsIndex to -1 to get the first defined monitor set.
        // Set afterMsIndex to msIndex to iteratate.
        // Terminate when VNI_E_NO_MORE_MONITOR_SETS is returned.
    VniSts getNextDefinedMonitorSet(LtMsIndex afterMsIndex, VniDescriptionData &monitorSetDescription, LtMsIndex &msIndex);

        // Open the monitor set.
    VniSts openMonitorSet(LtMsIndex msIndex, class VniMonitorSet &monitorSet, VniMonitorEnableType enableType = MONITOR_ENABLED); 

        // Close the monitor set.
    VniSts closeMonitorSet(class VniMonitorSet &monitorSet); 


        // Create and open a tempoary monitor set.  Add monitor entry viat the monitoar set.
    VniSts createTemporaryMonitorSet(class VniMonitorSet &monitorSet,
                                     const void *description, int descLen,
                                     VniMonitorEnableType enableType, boolean copyDesc = TRUE);

        // Used to iterate through all linked monitor sets.
    class VniMonitorSet *getFirstOpenMonitorSet() const;

        // Return the VniMonitor set used to represent this msIndex.
    VniMonitorSet *findOpenMonitorSet(LtMsIndex msIndex) const;


    /**************************************************************************
     *                      Monitor Set Callbacks.                            *
     **************************************************************************/

        // A monitor set has been added.
    virtual void monitorSetAdded(LtMsIndex msIndex, const void *pDescription, int descLen) {}

        // A monitor set has been removed.  If the monitor set is currently open, 
        // pMonitorSet will point to it, otherwise pMonitorSet will be NULL.
    virtual void monitorSetRemoved(LtMsIndex msIndex, const void *pDescription, int descLen, 
                                  VniMonitorSet *pMonitorSet) 
    {
        if (pMonitorSet != NULL)
        {
            closeMonitorSet(*pMonitorSet);
        }
    }

        // A monitor set has been modified.  If the monitor set is currently open, 
        // pMonitorSet will point to it, otherwise pMonitorSet will be NULL.
    virtual void monitorSetModified(LtMsIndex msIndex, const void *pDescription, int descLen, 
                                    VniMonitorSet *pMonitorSet) {}

    /**************************************************************************
     *                      Explicit Message Functions.                       *
     **************************************************************************/

        /* Message allocation methods */
    class VniMsgOut *msgAlloc();
    class VniMsgOut *msgAllocPriority();
    class VniRespOut *respAlloc(IN class VniMsgIn *msgIn);

    void release(class VniMsgIn* msg);
    void release(class VniRespIn* msg);
    
        // Send an explicit message.
    VniSts send(class VniMsgOut &msgOut, boolean throttle = true);
    VniSts send(IN class VniRespOut *pRespOut);

        // Given the domain index and a subnet node, returns whether or not this
        // is one of my subnet/node addresses.
    VniSts isMyAddress(int domainIndex, const VniSubnetNode &sn, boolean &isMine);

    Msec getMessageCompletionEventTimeout();
    Msec getNvCompletionEventTimeout();


    /**************************************************************************
     *                      Explicit Message Callbacks.                       *
     **************************************************************************/
    virtual void msgArrives(IN class VniMsgIn &msgIn,  IN class VniMsgPoint * pMsgPoint) {};
    virtual void msgCompletes(IN class VniMsgTag &tag, BOOLEAN success) {};
    virtual void respArrives(IN class VniMsgTag &tag, class VniRespIn &respIn) {};

protected:
    VniStack(IN VniServerControl &serverControl);
    virtual ~VniStack();
    virtual void releaseObj();

};

#endif
 
