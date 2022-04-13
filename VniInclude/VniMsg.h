//
// VniMsg.h
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

// Interfaces for VniMsgTag VniMsgOut, VniRespIn, VniMsgIn, VniRespOut

#if !defined(VNIMSG__INCLUDED_)
#define VNIMSG__INCLUDED_
#include "vniStack.h"
#include "VniMiscLt.h"

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniMsgTag
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////

class VniClientMsgTag;
template class VNICLIENT_DECL VniObjBase<class VniClientMsgTag>;
class VNICLIENT_DECL VniMsgTag : public VniObjBase<class VniClientMsgTag>
{
public:
    VniMsgTag *newObj();

protected:
    virtual void releaseObj();
    VniMsgTag();
    ~VniMsgTag();
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniMsgOut
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class VniClientMsgOut;
template class VNICLIENT_DECL VniObjBase<class VniClientMsgOut>;
class VNICLIENT_DECL VniMsgOut : public VniObjBase<class VniClientMsgOut>
{
public:
    static VniMsgOut *newObj(boolean bPriority);

    void setImplicitAddress(class VniMsgPoint *pVniMsgPoint);
    class VniMsgPoint *pVniMsgPoint();
    void setTag(VniMsgTag& tag);;
    VniMsgTag *getMsgTag();

    boolean            getOverrideSet();
    void setOverride(VniMsgOverride* pOverride);

    void setCode(int code);
	void setLength(int length);
    void setData(int offset, int data);
    void setData(byte* data, int offset, int length);
    void setCodeAndData(byte data[], int len);

    int getCode();
	byte* getData();
    int getLength();
    void getData(byte* pData, int offset, int length);
	byte getData(int offset);

    void setAuthenticated(boolean value);
	void setZeroSync(boolean value);
	void setAttenuate(boolean value);
    void setPath(int path);
    void setServiceType(LtServiceType value);		
    VniOutgoingAddress& getAddr();

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

        // Encryption values are initially FALSE, and can be set at any layer, but can never
        // be cleared again.
    void setDownlinkEncryption();
    boolean getDownlinkEncryption();
    void setUplinkEncryption();
    boolean getUplinkEncryption();

protected:
    VniMsgOut(boolean bPriority);
    virtual ~VniMsgOut();
    virtual void releaseObj();
    
private:
    VniOutgoingAddress m_outgoingAddress;
};


//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniIncomingAddress
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class VniClientIncomingAddress;
template class VNICLIENT_DECL VniObjBase<class VniClientIncomingAddress>;
class VNICLIENT_DECL VniIncomingAddress : public VniObjBase<class VniClientIncomingAddress>
{
public:
	LtAddressFormat getAddressFormat();

        // Source domain/subnet/node
    VniDomainConfiguration& getDomainConfiguration();   
        // The following retrieve dest fields.

	VniSubnetNode& getSubnetNode();               
	int getSubnet();                                    
	int getNode();
	int getGroup();
	int getMember();

    VniMsgInAddress &getMsgInAddress();

private:
//    friend class VniClientStack;
    friend class VniClientNvPoint;
    VniIncomingAddress(LtBlob &blob);
    ~VniIncomingAddress();

    void releaseObj();

    VniMsgInAddress m_msgInAddress;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniRespIn
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class VniClientRespIn;
template class VNICLIENT_DECL VniObjBase<class VniClientRespIn>;
class VNICLIENT_DECL VniRespIn : public VniObjBase<class VniClientRespIn>
{
public:
    int getCode();
	byte* getData();
    int getLength();
    void getData(byte* pData, int offset, int length);
	byte getData(int offset);

    VniMsgInAddress& getAddress();

    void releaseObj();

private:
    friend class VniClientStack;
    VniRespIn(class LtBlob &blob);
    ~VniRespIn();

    VniMsgInAddress m_msgInAddress;
};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniMsgIn
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class VniClientMsgIn;
template class VNICLIENT_DECL VniObjBase<class VniClientMsgIn>;
class VNICLIENT_DECL VniMsgIn : public VniObjBase<class VniClientMsgIn>
{
public:
    int getCode();
	byte* getData();
    int getLength();
    void getData(byte* pData, int offset, int length);
	byte getData(int offset);
    boolean getPriority();
    boolean getAuthenticated();
    int getPath();
	LtServiceType getServiceType();

    VniMsgInAddress& getAddress();

    void releaseObj();

private:
    friend VniClientStack;
    friend class VniClientMsgPoint;
    VniMsgIn(class LtBlob &blob, class VniMsgPoint *pMsgPoint, VniMsgInXrefId msgInXrefId);
    virtual ~VniMsgIn();

    VniMsgInAddress m_msgInAddress;

};

//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//                      VniRespOut
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
class VniClientRespOut;
template class VNICLIENT_DECL VniObjBase<class VniClientRespOut>;
class VNICLIENT_DECL VniRespOut : public VniObjBase<class VniClientRespOut>
{
public:

    static VniRespOut *newObj(VniMsgIn* pRequest);

    boolean getPriority();

    void setNullResponse();
    void setRespondOnFlexDomain();
    void setCode(int code);
	void setLength(int length);
    void setData(int offset, int data);
    void setData(byte* data, int offset, int length);
    void setCodeAndData(byte data[], int len);

    int getCode();
	byte* getData();
    int getLength();
    void getData(byte* pData, int offset, int length);
	byte getData(int offset);

        // Encryption values are initially FALSE, and can be set at any layer, but can never
        // be cleared again.
    void setDownlinkEncryption();
    boolean getDownlinkEncryption();
    void setUplinkEncryption();
    boolean getUplinkEncryption();

protected:
    VniRespOut(VniMsgIn* pRequest); 
    virtual ~VniRespOut();
    virtual void releaseObj();

};
#endif
