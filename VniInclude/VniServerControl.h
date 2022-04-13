//
// VniServerControl.h
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

// Interface for the VniServerControl class.
//
//  VNI Client API's highest level interface to the VNI server.

#if !defined(AFX_VNISERVERCONTROL_H__DA69AF4C_14E2_11D3_80B2_00105A202B29__INCLUDED_)
#define AFX_VNISERVERCONTROL_H__DA69AF4C_14E2_11D3_80B2_00105A202B29__INCLUDED_
#include "vnidefs.h"
#include "vniInterfaces.h"
#include "vniIpAuthKey.h"

#include "VniTimer.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class VniIncomingAddress;
class NvValue;

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/* The VNI client API is composed of the following main classes:
    VniServerControl   
        This class is used to to control the server lifetime, communicate with the server,
        and to list VniStack objects.  There is only one VniServerObject object defined.  
        The VniServerObject owns 0 or more stacks.  Some or all of these stacks may
        be represented by an open VniStack object.

    VniStack
        This class represents the LONTalk stack, and provides global functions on the stack.
        
        The VniStack is  used to list the monitor sets that it owns.  Note that monitor set names
        are unique only within a stack.  Once open, a monitor set may be represented by a
        VniMonitorSet object, which in turn is owned by an open VniStack object.

        The VniStack object contains virtual callbacks to report network variable updates 
        and completion events.
        
        The VniStack is used to send explicit messages and responses, and, contains virtual 
        methods for  recieving messages, completion events and responses.

     VniMonitorSet
        This class represents a single monitor set.  Typically this class serves as
        a base class for the application's monitor set.  When a monitor set is opened 
        (by specifying the name and the owning VniStack object) it may call 
        methods to create VniNvPoint objects and ExplicitMsg objects, which are owned by the
        monitor set.  Typically these virtual methods will be overridden to allow the 
        application to supply its own method of creation, since the application must coordinate
        deletion and creation of its monitor points with the application.  If configuration 
        changes are made, additional VniNvPoint and VniMsgPoint objects may be added, 
        or existing instances may be removed or modified.

     VniNvPoint 
        This class represents an instance of a local or remote network variable in a monitor
        set.  The actual network variable may in fact be a member of multiple monitor sets:
        the application will have a separate VniNvPoint for each instance of that network variable
        in an opened monitor set.  
        
        When a network variable update arrives, the applicaiton is informed via a callback 
        method in the VNI stack, which passes a refernce to the appropriate VniNvPoint.

        In addition to providing basic access to local and remote network variables, 
        this object supports timed polling.

     VniMsgPoint
        This class is the explicit message counterpart to a VniNvPoint.  It is used to 
        address messages (sent via VniStack) and when explicit messages are recived (by the
        VniStack) a reference to a VniMsgPoint will be included in the parameter list.  

*/    


/* This support object is used to store strings allocated by the VNI. */
class VniStack;
class VniClientServerControl;

class VniServerCollection;
template class VNICLIENT_DECL VniObjBase<VniClientServerControl>;

/* This object is used to control the VNI server.  It must be instantiated before
   any other VNI access can be performed. It is used to create/list/open/close/and remove
   VniStack objects. */
class  VNICLIENT_DECL VniServerControl : public VniObjBase<VniClientServerControl>
{

public:
    
    static VniServerControl *newObj();
    /**************************************************************************
     *                      VNI Server Control.                               *
     **************************************************************************/
        // Start up the VNI server.
    VniSts open(int maxEvents = 0, Msec maxEventPeriod = 100, int ipcSpace = 0 /* default */);

        /* Get event handle to wait for VNI messages. */
    HANDLE getVniMessageReadyHandle(void);
         /* Process a VNI message, if any exists.  Should call this whenever 
           handle passed back by GetVniMessageReadyHandle is signaled. */
    VniSts processInputMessage(void) ;
    VniSts close();
    VniSts debugServer(void);
    VniSts setDesktop(void);
    int serverUseCount();

        // Set the IP channel secret key for the currently defined IP device.
    VniSts setVniIpChannelSecretKey(const char *ipDeviceName, const VniIpChannelSecretKey key);

        // Dump LONTalk IP routing information file for specified channel.
    VniSts dumpVniIpChannelXmlConfig(const char *ipDeviceName);


     /**************************************************************************
     *                            Stack Management                             *
     **************************************************************************/

        /* Create a new VNI with the specified interface. */
    VniSts createVni(IN const char         *vniName, 
                     IN const char         *pVniDirectory,
                     IN const char         *pNetworkInterface,
                     IN const VniProgramId  programId,
                     IN int                 maxDomains,
                     IN int                 maxAddresses,
                     IN int                 maxStaticNvs,
                     IN int                 maxDynamicNvs,
                     IN int                 maxAliases,
                     IN const char*         nodeSelfDoc,
                     IN int                 numMonitorNvEntries = VNI_DEFAULT_NUM_MONITOR_NV_ENTRIES,
                     IN int                 numMonitorPointEntries = VNI_DEFAULT_NUM_MONITOR_POINT_ENTRIES,
                     IN int                 numMonitorSetEntries = VNI_DEFAULT_NUM_MONITOR_SET_ENTRIES,
                     IN int                 numMessageTags = VNI_DEFAULT_NUM_MESSAGE_TAGS
                    );

    VniSts importVni(IN const char *vniName, IN const char *pVniDirectory);

    VniSts deleteVni(IN const char *vniName);
    VniSts coldResetVni(IN const char *vniName);
        // Open the stack specifed by the vni NAME.
    VniSts openStack(IN const char *vniName, VniStack &stack, 
                     VniMonitorEnableType enableType = MONITOR_ENABLED);

        // DEBUGGING ONLY---.  Open a stack specifying a xif for its external interface.  Be careful,
        // if someon alread has the stack open, the xfb is ignored.  This is exposed only to allow
        // LNS testing of different types of hosts.
    VniSts openStackXfb(IN const char *vniName, VniStack &stack, const char *xfbPath,
                        VniMonitorEnableType enableType = MONITOR_ENABLED);


    VniSts closeStack(VniStack &stack);
    VniSts closeStack(const char *vniName);

        // Used to iterate through open stack objects.
    VniStack *getFirstOpenStack() const;
    VniStack *findStack(const char *vniName) const;

        // Used to iterate through all stacks currently opened by any process
        // using the specified device.  Initially afterVniIterator should be 0,
        // then it should be set to the value of iteratorId to iterate.  
        // layerRangeBottom and layerRangeTop indicate what layers the PC
        // runs.  Terminates with VNI_S_NO_MORE_VNIS.
    VniSts getNextStackUsingDevice(IN const char *deviceName, 
                                   IN DWORD       afterVniIterator,    
                                   OUT VniStr    &vniName,
                                   OUT DWORD     &iteratorId,
                                   OUT int       &layerRangeBottom, 
                                   OUT int       &layerRangeTop);

     /**************************************************************************
     *                            Tracing                                      *
     **************************************************************************/

    VniSts setTraceOptions(char *fileName, VniTraceType traceType, 
                           VniTraceClientType traceClientType,
                           BOOLEAN enableLog);

    BOOLEAN getTraceOptions(VniStr &fileName, VniTraceType &traceType, 
                            VniTraceClientType &traceClientType,
							BOOLEAN &logEnabled);

    void disableTrace();
    void clearTraceFile();
    void flushTraceFile();

    /**************************************************************************
     *                      Server Communication                              *
     *
     * There are two ways for the VniServerControl to recieve messages from 
     * VNI server:
     *  1) Using its own dedicated reader thread.  To use this method:
     *      a) Open the VniServerControl (VniServerControl::open())
     *      b) create the dedicated reader thread (VniServerControl::createReaderThread()).
     *      When done
     *      c) destroy the dedicated reader thread (VniSeverControl::waitForReaderThreadToQuit()).
     *      d) close the server control: (VniServerControl::close()).
     * 
     *  2) Registering with a VniServerCollection object.  To use this method
     *      a) If not already done, create a VniServerCollection object and add a refrence to it.
     *      b) open the VniServerControl (VniServerControl::open())
     *      c) register the VniServerControl with the VniServerCollection object
     *         (VniSeverControl::registerWithServerCollection).
     *      When done
     *      d) deregister with the VniServerCollection object 
     *          (VniSeverControl::deregisterFromServerCollection)
     *      e) close the VniServerControl (VniSeverControl::close()).
     *      f) If you are done with the VniServerCollection release it.
     *
     * Note:  The VniServerControl throttle parameters are not used when used as
     *        a member of a collection - instead throttling is managed via
     *        the VniServerCollection.
     *
     * A given server control must use one or the other of these methods - not
     * both!
     **************************************************************************/


    /**************************************************************************
     *                      VNI Server Collection.                            *
     **************************************************************************/
        
        // Register with the server collection. This will enable reciept of messages
        // from the Vni server.
    VniSts registerWithServerCollection(VniServerCollection *pServerCollection);
        // Deregister with the server collection - This must be done before closing
        // the VniServerControl.  It cannot be done in a callback from the 
        // VniServerCollection reader thread!
    VniSts deregisterFromServerCollection(void);

    VniServerControl *nextServerControl(void) const;
    VniServerCollection *getVniServerCollection();


     /**************************************************************************
     *                            Reader Thread                               *
     **************************************************************************/
        
        // Create a dedicated reader thread. Must be done after opening the server.
    void createReaderThread(void);
        // Destroy the reader thread, and wait for it to quit.  Must be done
        // prior to closing the server control.  Must not be done in a 
        // callback from the reader thread.
    void waitForReaderThreadToQuit(void);

    void setThrottle(int maxEvents, Msec maxEventPeriod);
    void getThrottle(int &maxEvents, Msec &maxEventPeriod);

     /**************************************************************************
     *                            Misc                                         *
     **************************************************************************/

    VniAlarmControl *alarmControl();

    
     /**************************************************************************
     *                         Access to VniRegistry                           *
     **************************************************************************/
        // Used to enumerate VNIs.  Index starts at 0.  Terminates (succefully)
        // with VNI_W_NO_MORE_VNIS.  The regPath may be in one of three forms:
        // a) Default.  regPath is NULL, and the VNIs are found in
        //    \HKEY_LOCAL_MACHINE\Software\LonWorks\VNI\Configuration.  Only the
        //    vniBaseName is returned.
        // b) Relative.  regPath is not NULL, and does not start with \.  The VNI
        //    is found at \HKEY_LOCAL_MACHINE\Software\LonWorks\<regPath>\VNIs.  The
        //    name of the VNI returned is <regPath>\<vniBaseName>
        // c) Absolute.  regPath starts with \.  VNIs are found in <regPath>\VNIs.  THe
        //    VNI name is <regPath>\<vniBaseName>, unless it could be expressed as a 
        //    relative path or a default path - in which case that name is used instead.
        //    ABSOLUTE NAMES NOT IMPLEMENTED.
    VniSts enumerateVni(const char *regPath, OUT VniStr &vniName, DWORD index) const;
        // Deprecated version, always looks in LONWORKS\VNI\Configuration
    VniSts enumerateVni(OUT VniStr &vniName, DWORD index) const;
        // Get a "short" VNI name, for display purposes.
    static void getVniShortName(const char *longName, OUT VniStr &shortName);
        // Translate the short VNI name back to a long VNI name.
    static void getVniLongName(const char *shortName, OUT VniStr &longName);
        // Get the name of the VNI device driver
    VniSts getVniDevice(IN const char *vniName, OUT VniStr &deviceName);
         // Set the name of the VNI device driver
    VniSts setVniDevice(IN const char *vniName, IN const char *deviceName);
       // Get the path of the VNI directory
    VniSts getVniDirectoryPath(IN const char *vniName, OUT VniStr &directoryPath);
    BOOLEAN vniExists(IN const char *vniName);

    VniSts getVniProgramId(IN const char *vniName, OUT VniProgramId *programId);
    VniSts getVniMaxDomains(IN const char *vniName, OUT int &maxDomains);
    VniSts getVniMaxAddresses(IN const char *vniName, OUT int &maxAddresses);
    VniSts getVniMaxStaticNvs(IN const char *vniName, OUT int &maxStaticNvs);
    VniSts getVniMaxDynamicNvs(IN const char *vniName, OUT int &maxDynamicNvs);
    VniSts getVniMaxAliases(IN const char *vniName, OUT int &maxAliases);
    VniSts getVniNodeSelfDoc(IN const char *vniName, OUT VniStr &nodeSelfDoc);
    VniSts getVniUniqueId(IN const char *vniName, OUT VniUniqueId *uniqueId);
    VniSts setVniUniqueId(IN const char *vniName, IN VniUniqueId *uniqueId);
    VniSts getVniNumMonitorNvEntries(IN const char *vniName, OUT int &numMonitorNvEntries);
    VniSts getVniNumMonitorPointEntries(IN const char *vniName, OUT int &numMonitorPointEntries);
    VniSts getVniNumMonitorSetEntries(IN const char *vniName, OUT int &numMonitorSetEntries);
    VniSts getVniNumMessageTags(IN const char *vniName, OUT int &numMessageTags);
    VniSts getVniPollingLimits(IN const char *vniName, 
                               OUT int &numReservedMsgs, 
                               OUT int &pollingLimitThreshold,
                               OUT int &initialPollLimit,
                               OUT int &minimumPollLimit);
    VniSts setVniPollingLimits(IN const char *vniName, 
                               IN int numReservedMsgs, 
                               IN int pollingLimitThreshold,
                               IN int initialPollLimit,
                               IN int minimumPollLimit);


        // Get and set global VNI parameters (do not effect external interface)
    VniSts getVniMaxPrivateNvs(OUT int &maxPrivatNvs);
    VniSts setVniMaxPrivateNvs(OUT int maxPrivatNvs);
    VniSts getVniReceiveTx(OUT int &recieveTx);
    VniSts setVniReceiveTx(IN int recieveTx);
    VniSts getVniTransmitTx(OUT int &transmitTx);
    VniSts setVniTransmitTx(IN int transmitTx);
    VniSts getVniTransactionIdLifeTime(OUT int &transactionIdLifeTime);
    VniSts setVniTransactionIdLifeTime(IN int transactionIdLifeTime);
    VniSts getVniMessageEventMaximum(OUT int &count);
    VniSts setVniMessageEventMaximum(IN int count);
    VniSts getVniMessageOutMaximum(OUT int &count, OUT int &countPri);
    VniSts setVniMessageOutMaximum(IN int count, IN int countPri);
    VniSts getVniServerCmdThreadStackSize(OUT int &stackSize);
    VniSts setVniServerCmdThreadStackSize(IN int stackSize);
    VniSts getVniDefaultPollingLimits(OUT int &numReservedMsgs, OUT int &pollingLimitThreshold,
                                      OUT int &initialPollLimit, OUT int &minimumPollLimit);
    VniSts setVniDefaultPollingLimits(IN int numReservedMsgs, IN int pollingLimitThreshold,
                                      IN int initialPollLimit, IN int minimumPollLimit);


        // Get and set device parameters
    VniSts getDeviceTimeouts(const char *deviceName, int &openTimeout, int &responseTimeout, int &openRetries);
    VniSts getDeviceConfiguredMipLayer(const char *deviceName, int &mipLayer, int &xcvrId, boolean &bIsNsaMip);
    VniSts getDeviceXcvrId(const char *deviceName, int &xcvrId);
    VniSts getDeviceConfiguredBufferSizes(const char *deviceName, int &maxSicbData);
    VniSts getDeviceConfiguredAdvancedTxTickler(const char *deviceName, boolean &advancedTxTickler);
    VniSts getDeviceConfiguredNmVersion(const char *deviceName, int &nmVersion, int &nmCapabilities);
	VniSts getDeviceTurnaround(const char *deviceName, int &turnAround);
    VniSts getDeviceSupportsEncryption(const char *deviceName, boolean &supportsEncryption);

    VniSts setDeviceTimeouts(const char *deviceName, int openTimeout, int responseTimeout, int openRetries);
    VniSts setDeviceTimeoutDefaults(int openTimeout, int responseTimeout, int openRetries);
    VniSts setDeviceConfiguredMipLayer(const char *deviceName, int mipLayer, int xcvrId, boolean bIsNsaMip);
    VniSts setDeviceXcvrId(const char *deviceName, int xcvrId);
    VniSts setDeviceConfiguredBufferSizes(const char *deviceName, int maxSicbData);
    VniSts setDeviceConfiguredAdvancedTxTickler(const char *deviceName, boolean advancedTxTickler);
    VniSts setDeviceConfiguredNmVersion(const char *deviceName, int nmVersion, int nmCapabilities);
	VniSts setDeviceTurnaround(const char *deviceName, int turnAround);
    VniSts setDeviceSupportsEncryption(const char *deviceName, boolean supportsEncryption);

    VniSts deleteDeviceTimeouts(const char *deviceName);
    VniSts deleteDeviceTimeoutDefaults();
    VniSts deleteDeviceConfiguredMipLayer(const char *deviceName);
    VniSts deleteDeviceXcvrId(const char *deviceName);
    VniSts deleteDeviceConfiguredBufferSizes(const char *deviceName);
    VniSts deleteDeviceConfiguredAdvancedTxTickler(const char *deviceName);
    VniSts deleteDeviceConfiguredNmVersion(const char *deviceName);
    VniSts deleteDeviceSupportsEncryption(const char *deviceName);

    // Locking and version numbers
    VniSts lockVniDir(boolean write);

    void unlockVniDir();

    VniSts getDirectoryVersionNumber(VniDirVersionNum &vniDirVersion);

    VniSts getServerIpcSpace(int &ipcSpace);
    VniSts setServerIpcSpace(int ipcSpace);

protected:
	VniServerControl();
	virtual ~VniServerControl();
    virtual void releaseObj();
};


// Returns temporary error message string.  String will be overwritten
// on each call, so client should copy it if needed longer term.
VNICLIENT_DECL LPCSTR VniErrorMessage(VniSts sts);

class VNICLIENT_DECL VniLock : public RmoLock
{
};


class VNICLIENT_DECL VniCommon
{
public:
    virtual VniSts setMonitorEnableType(VniMonitorEnableType enableType) = 0;

	    // Return the enable type for the msg point. 
    virtual VniMonitorEnableType getMonitorEnableType() const = 0;

};


#endif // !defined(AFX_VNISERVERCONTROL_H__DA69AF4C_14E2_11D3_80B2_00105A202B29__INCLUDED_)
