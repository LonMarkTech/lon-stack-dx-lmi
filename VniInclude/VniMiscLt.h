//
// VniMiscLt.h
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

// Interface Definitions used to interface with the LON Stack. 

#ifndef __VNI_VNI_MISC_LT_INCLUDED__
#define __VNI_VNI_MISC_LT_INCLUDED__
#include "VniLtDefs.h"
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniMsgOverrideOptions
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtMsgOverrideOptions;
template class VNICLIENT_DECL VniSimpleObjBase<LtMsgOverrideOptions>;
class VNICLIENT_DECL VniMsgOverrideOptions : public VniSimpleObjBase<LtMsgOverrideOptions>
{
public:
	VniMsgOverrideOptions();
	VniMsgOverrideOptions(byte options);
    VniMsgOverrideOptions( const VniMsgOverrideOptions& ovOpt);
    ~VniMsgOverrideOptions();
	VniMsgOverrideOptions& operator=( const VniMsgOverrideOptions& ovOpt);

	byte getOptions();
	boolean hasOverrides();

	boolean overrideService();
	boolean overridePriority();
	boolean overrideTxTimer();
	boolean overrideRptTimer();
	boolean overrideRetryCount();
	boolean overrideAuthKey();

    void setImplementor(LtMsgOverrideOptions *pImplementor);
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniMsgOverride
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtMsgOverride;
template class VNICLIENT_DECL VniSimpleObjBase<LtMsgOverride>;
class VNICLIENT_DECL VniMsgOverride : public VniSimpleObjBase<LtMsgOverride>
{
public:
	VniMsgOverride();
	VniMsgOverride(VniMsgOverrideOptions options,
				  LtServiceType service,
				  boolean priority,
				  int txTimer,
				  int rptTimer,
				  byte retryCount,
				  byte *authkey=NULL,
                  boolean keyIsOMA = FALSE);
	VniMsgOverride( const VniMsgOverride& ov);
	~VniMsgOverride();
	VniMsgOverride& operator=( const VniMsgOverride& ov);

	boolean hasOverrides();

	VniMsgOverrideOptions& getOptions();
	LtServiceType getService();
	boolean getPriority();
	int getTxTimer();
	int getRptTimer();
	byte getRetryCount();
	byte *getAuthKey();
    boolean getKeyIsOma();
	void setDefaults(VniMsgOverride* pMsgOverride);

    void setImplementor(LtMsgOverride *pImplementor);
    void update(const LtMsgOverride *pLtMsgOverride);

private:
    VniMsgOverrideOptions *m_pOptions;
    void updateOptions();
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniAddressConfiguration
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtAddressConfiguration;
template class VNICLIENT_DECL VniSimpleObjBase<LtAddressConfiguration>;
class VNICLIENT_DECL VniAddressConfiguration : public VniSimpleObjBase<LtAddressConfiguration>
{
public:
    VniAddressConfiguration();
    VniAddressConfiguration(int index);
    VniAddressConfiguration(int type, int domainIndex, int id);
    virtual ~VniAddressConfiguration();
    boolean equals(VniAddressConfiguration& cmp);
    void copy(VniAddressConfiguration& ac);
	VniAddressConfiguration& operator=( const VniAddressConfiguration& ac);

    int getGroup();
    boolean isBound();
	boolean isBoundExternally();
	boolean inUse();
    void setGroup(int group);
    int getAddressType();
    void setAddressType(int type);
    int getTxTimer();
    void setTxTimer(int value);
    int getRptTimer();
    void setRptTimer(int value);
    int getRcvTimer();
    void setRcvTimer(int value);

    int getRetry();
    void setRetry(int value);

    int getMaxResponses();
    void setMaxResponses(int value);

    int toLonTalk(byte* pData, int version);
    VniSts fromLonTalk(byte data[], int& len, int version);

	int getIndex();
    void setIndex(int index);
	int getSubnet();
    void setSubnet(int subnet);
	int getDomainIndex();
    void setDomainIndex(int domainIndex);

	// Group/subnet fields
	int getDestId();
    void setDestId(int destId);

	// Group only fields
	int getSize();
    void setSize(int size);
	int getMember();
    void setMember(int member);
	int getRestrictions();
    void setRestrictions(int restrictions);

	// Broadcast only fields
	int getBacklog();
    void setBacklog(int backlog);

    void setImplementor(LtAddressConfiguration *pImplementor, boolean allocated);
private:
    boolean m_allocated;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniSubnetNode
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtSubnetNode;
template class VNICLIENT_DECL VniSimpleObjBase<LtSubnetNode>;
class VNICLIENT_DECL VniSubnetNode : public VniSimpleObjBase<LtSubnetNode>
{
public:
    VniSubnetNode();          
	VniSubnetNode(int nSubnet, int nNode);
	VniSubnetNode& operator=( const VniSubnetNode& subnetNode);
    ~VniSubnetNode();

	void set(int nSubnet, int nNode);
	void set(VniSubnetNode& sn);
	void setSubnet(int nSubnet);
	byte getNode();
	byte getSubnet();

	boolean operator ==(VniSubnetNode& subnetNode);
	boolean inUse();

    void setImplementor(LtSubnetNode *pImplementor, boolean allocated);

private:
    boolean m_allocated;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniDomain
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtDomain;
template class VNICLIENT_DECL VniSimpleObjBase<LtDomain>;
class VNICLIENT_DECL VniDomain : public VniSimpleObjBase<LtDomain>
{
public:
    VniDomain();
    VniDomain(byte* pData, int nLen);
	VniDomain& operator=( const VniDomain& dc);
    ~VniDomain();

	void set(byte* pData, int nLen);
	void set(VniDomain& domain);
    void setDomain(int length, byte* data);
    void setZeroLength();
    int getData(int i);
    int getAll(byte* data);
    int get(byte* data);
    boolean inUse();
    boolean isValid();

    int getLength();
    void getData(byte* data);
    boolean operator ==(VniDomain& domain);

    void setImplementor(LtDomain *pImplementor, boolean allocated);

private:
    boolean m_allocated;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniDomainConfiguration
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtDomainConfiguration;
template class VNICLIENT_DECL VniSimpleObjBase<LtDomainConfiguration>;
class VNICLIENT_DECL VniDomainConfiguration : public VniSimpleObjBase<LtDomainConfiguration>
{
public:
    VniDomainConfiguration();
    VniDomainConfiguration(int index);
    VniDomainConfiguration(int index, VniDomain& domain, byte* pKey, 
                          int subnet, int node, boolean useOma = FALSE);
    VniDomainConfiguration(VniDomain& domain, byte* pKey, int subnet, int node, boolean useOma = FALSE);
    virtual ~VniDomainConfiguration();

	VniDomainConfiguration& operator=( const VniDomainConfiguration& dc);

    void setAddress(int subnet, int node);
    int getIndex();
    void setIndex(int index);
    boolean equals(VniDomainConfiguration& match, boolean includeAddress);
    boolean equals(VniSubnetNode& sn);

    VniDomain& getDomain();
	VniSubnetNode& getSubnetNode();
    int getSubnet();
    int getNode();
    boolean isFlexDomain();
	byte* getKey();
    boolean getUseOma();
	void setKey(byte* pKey);
	static boolean isFlexDomain(int index);
    VniSts fromLonTalk(byte data[], int& len, VniLtLonTalkDomainStyle style);
    int toLonTalk(byte* data, VniLtLonTalkDomainStyle style);

    void setImplementor(LtDomainConfiguration *pImplementor, boolean allocated);

private:
    boolean m_allocated;

private:
    VniSubnetNode m_subnetNode;
    VniDomain     m_domain;

};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniOutgoingAddress
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtOutgoingAddress;
template class VNICLIENT_DECL VniSimpleObjBase<LtOutgoingAddress>;
class VNICLIENT_DECL VniOutgoingAddress: public VniSimpleObjBase<LtOutgoingAddress>
{
public:
    VniOutgoingAddress();
    VniOutgoingAddress(VniOutgoingAddress& address);
    virtual ~VniOutgoingAddress();
	VniOutgoingAddress& operator=( const VniOutgoingAddress& addr);

    void copy(VniAddressConfiguration& ac);
    void setDomainConfiguration(VniDomainConfiguration& domain);
    VniDomainConfiguration& getDomainConfiguration();   

	void setLocal();
	void set(int type, int domain, int retry, int timer, ULONGLONG destId, int misc);
    void setSubnetNode(int domain, int subnet, int node,
                              int timer, int retry);
    void setGroup(int domain, int group, int groupSize,
                         int timer, int retry);
    void setBroadcast(int domain, int subnet, int backlog,
                             int timer, int retry);
    void setUniqueId(int domain, int subnet, VniUniqueId& id,
                            int timer, int retry);
    void setGroupSize(boolean inGroup);
	void setSubnetNode(int sn, int nd);

	VniUniqueId& getDestinationUniqueId();

	int getSubnetNodeIndex();
	void setSubnetNodeIndex(int index);

	int getSubnet();
	int getDestId();

	int getTxDuration(LtServiceType st);

    int getAddressType();
    void setAddressType(int type);
    int getTxTimer();
    void setTxTimer(int value);
    int getRptTimer();
    void setRptTimer(int value);
    int getRcvTimer();
    void setRcvTimer(int value);
    int getRetry();
    void setRetry(int value);
    int getMaxResponses();
    void setMaxResponses(int value);

    void setImplementor(LtOutgoingAddress *pImplementor, boolean allocated);

private:
    boolean m_allocated;

private:
    VniAddressConfiguration m_ac;
    VniDomainConfiguration  m_domain;
    void init(LtOutgoingAddress *pImplementor);
    VniUniqueId m_uniqueId;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniMsgInAddress
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtIncomingAddress;
template class VNICLIENT_DECL VniSimpleObjBase<LtIncomingAddress>;
class VNICLIENT_DECL VniMsgInAddress: public VniSimpleObjBase<LtIncomingAddress> 
{
public:
    VniMsgInAddress(LtIncomingAddress *pLtIncomingAddress);
    virtual ~VniMsgInAddress();

	LtAddressFormat getAddressFormat();
        // Source domain/subnet/node
    VniDomainConfiguration& getDomainConfiguration();   
        // The following retrieve dest fields.

	VniSubnetNode& getSubnetNode();               
	int getSubnet();                                    
	int getNode();
	int getGroup();
	int getMember();

    void setImplementor(LtIncomingAddress *pImplementor, boolean allocated);

private:
    boolean m_allocated;

private:
    VniDomainConfiguration m_sourceDomain;
    VniSubnetNode          m_destNode;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniNetworkVariableConfiguration
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtNetworkVariableConfiguration;
template class VNICLIENT_DECL VniSimpleObjBase<LtNetworkVariableConfiguration>;
class VNICLIENT_DECL VniNetworkVariableConfiguration: 
    public VniSimpleObjBase<LtNetworkVariableConfiguration>
{
public:
    VniNetworkVariableConfiguration();
	VniNetworkVariableConfiguration(int nIndex, int nType, int x);
    virtual ~VniNetworkVariableConfiguration();

	boolean hasAddress();
    boolean hasOutputAddress();
	int getPrimary();
	boolean isAlias();

    VniOutgoingAddress* getAddress();

    LtServiceType getServiceType();
	void setServiceType(LtServiceType serviceType);

	boolean getIsAlias();
    void setIsAlias(boolean isAlias);
	boolean getPriority();
    void setPriority(boolean priority);
	boolean getTurnaround();
    void setTurnaround(boolean turnaround);
	boolean getAuthenticated();
    void setAuthenticated(boolean authenticated);
	boolean getOutput();
    void setOutput(boolean output);
	boolean getReadByIndex();
    void setReadByIndex(boolean readByIndex);
	boolean getWriteByIndex();
    void setWriteByIndex(boolean writeByIndex);
	boolean getRemoteNmAuth();
    void setRemoteNmAuth(boolean remoteNmAuth);
	boolean getSourceSelectionOnly();
    void setSourceSelectionOnly(boolean sourceSelectionOnly);
	int getNvUpdateSelection();
    void setNvUpdateSelection(int nvUpdateSelection);
	int getNvResponseSelection();
    void setNvResponseSelection(int nvResponseSelection);
	int getNvRequestSelection();
    void setNvRequestSelection(int nvRequestSelection);
	int getIndex();
    void setIndex(int index);
	int getAddressTableIndex();
    void setAddressTableIndex(int addressTableIndex);
	int getNvIndex();
    void setNvIndex(int nvIndex);
	int getSelector();
    void setSelector(int selector);
	int getPrimaryIndex();
    void setPrimaryIndex(int primaryIndex);

    int toLonTalk(byte* data, boolean bExact = false, int nVersion = 2);
    int fromLonTalk(byte data[], int length, int nVersion = 2);

    void setImplementor(LtNetworkVariableConfiguration *pImplementor);

private:
    class VniOutgoingAddress              m_outgoingAddress;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniStatus
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtStatus;
template class VNICLIENT_DECL VniSimpleObjBase<LtStatus>;
class VNICLIENT_DECL VniStatus: public VniSimpleObjBase<LtStatus>
{
public:
    VniStatus();
    virtual ~VniStatus();
	VniStatus& operator=( const VniStatus& status);

    /**
     * Number of transmission errors since last clear.  Includes CRC errors and
     * packets with preamble but no data.
     */
    int getTransmissionErrors();      
    
    /**
     * Number of transmit timeouts.  This counts the number of times acknowledged
     * or request response service was attempted without receiving all required 
     * acks/responses.
     */
    int getTransmitTimeouts();
    
    /**
     * Number of instances of receive transaction record exhaustion.  Each incoming
     * messages (other than unackd) is tracked for duplicate detection purposes using
     * a receive transaction record. 
     */
    int getReceiveTransactionFulls();
    
    /**
     * Number of lost messages.  This includes message lost at the network layer due
     * to an insufficient number of buffers.  This generally indicates that messages
     * addressed to the node are arriving faster than the application can handle
     * them.  If the condition is bursty in nature, then additional buffers may solve
     * the problem.
     */
    int getLostMessages();
    
    /**
     * Number of missed messages.  This includes messages lost at the MAC layer due
     * to an insufficient number of buffers or insufficiently large buffers.  This
     * generally means that messages are arriving at the device faster than the 
     * protocol stack can process them.  If the condition is due to traffic bursts,
     * then additional buffers may solve the problem.
     */
    int getMissedMessages();
    
    /**
     * Reset cause.  The reset cause has the following form:
     * <p><pre>
     * XXXXXXX1     - power-up reset
     * XXXXXX10     - external reset
     * XXXX1100     - watchdog timer reset
     * XXX10100     - internal/software reset
     * </pre>
     */
    int getResetCause();
    
    /** 
     * State.  The device state as follows:
     * <p><pre>
     * BXXXOSSS
     * where
     *  B is 1 for bypass offline, 0 otherwise.
     *  O is 1 for offline, 0 for online.
     *  SSS is 
     *      2   unconfigured
     *      3   applicationless
     *      4   configured
     *      6   hard-offline
     * </pre>
     */
    int getState();
    
    /** 
     * Version. The device firmware version number.  128 is used by non-Neuron
     * implementations. 
     */
    int getVersion();
    
    /**
     * Error log.  The most recently logged error (see Neuron Chip Data Book for
     * a description of error codes).
     */
    int getError();
    
    /**
     * Model number.  The microprocessor model number.  128 is used by non-Neuron
     * implementations.
     */
    int getModel();
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniLtMisc
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class VNICLIENT_DECL VniLtMisc {
public:
    
    static int toTimer(int base, int t);

    static int toTxTimer(int t);

    static int toRcvTimer(int t);

    static int fromTxTimer(int t);

    static int fromRcvTimer(int t);

    static void validateVniLtDefs(void);  // Verify definitions defined in Vni and LT matchup.

};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniCommParams
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class LtCommParams;
template class VNICLIENT_DECL VniSimpleObjBase<LtCommParams>;
class VNICLIENT_DECL VniCommParams: public VniSimpleObjBase<LtCommParams>
{
public:
    VniCommParams();
    ~VniCommParams();
    static getCommParamsSize() { return 16; };
    byte get(int index);
    void set(byte value, int index);
	boolean isCompatible(VniCommParams& cps);
	int setNewPriority(VniCommParams& cps);
};

#endif
