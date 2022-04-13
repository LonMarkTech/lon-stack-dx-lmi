//
// VniProtocolAnalyzerControl.h
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

// Definitions used to open a protocol analyzer. This object is designed to be 
// "stand-alone", in that it requires minimal VNI include files, and manages
// the VniServerControl itself.  

#if !defined(VniProtocolAnalyzerControl__INCLUDED_)
#define VniProtocolAnalyzerControl__INCLUDED_
#include  <sys\timeb.h>

#ifdef VNICLIENT_EXPORTS
#define VNI_PA_CLIENT_DECL __declspec(dllexport) 
#else
#define VNI_PA_CLIENT_DECL __declspec(dllimport) 
#endif

#ifdef _MSC_VER
#pragma pack(push,8)
#endif
typedef struct _tagTimeStampedPkt {
	DWORD 	highResTimeStamp;
	_timeb 	currentTime;
	BYTE 	packetData[257];
} TimeStampedPkt;
#ifdef _MSC_VER
#pragma pack(pop)
#endif

class VNI_PA_CLIENT_DECL VniProtocolAnalyzerControl
{
public:
	VniProtocolAnalyzerControl();
	virtual ~VniProtocolAnalyzerControl();

    // This open method is obsolete.  Calling this method is identical to calling the
    // second open method with useTimeStampedPkt set to FALSE.
    HRESULT open(const char *deviceName, int password = 0x0854abc93);
    HRESULT close();

	void releasePacket(BYTE *pData);

    // Implement this callback to recieve incomming packet data, excluding the 2 byte
    // layer 2 header.  Bad packets will not be recieved.
    // You must call open(FALSE, ....) or use the deprecated open method above 
    // to recieve packets with this callback.
    // Your method must call releasePacket to release the packet when you are done with it.
	virtual void packetArrived(DWORD timeStamp, int packetNumber, int length, BYTE *pData) 
	{
		releasePacket(pData);
	};

    void sendPacket(BYTE *pData, int len);


	//*************************************************************************************
    // The interfaces below this line are new for LNS 3.20.60. They support an extended 
    // packet format (TimeStampedPkt) which includes both a time stamp and the full layer 2
    // SICB.  The first two bytes of the SICB the packet type and the packet length. When the
    // TimeStampedPkt format is used, bad packets will be reported, whereas they will be ommitted
    // if the old format is used (since the old format does not include the packet type information.
	//*************************************************************************************

    // If useTimeStampedPkt is TRUE, the packetArrivedEx method will be called as new packets arrive.
    // If useTimeStampedPkt is FALSE, the old packetArrived method will be called as new packets arrive,
    // and bad packets will be discarded.   
    HRESULT open(boolean useTimeStampedPkt, const char *deviceName, int password = 0x0854abc93);

	void releasePacket(TimeStampedPkt *pPacket);

    // Implement this callback to recieve incomming packets with the TimeStampedPkt format.
    // You must call open(TRUE, ....) to recieve packets with this callback.
    // Your method must call releasePacket to release the packet when you are done with it.
	virtual void packetArrivedEx(TimeStampedPkt *pPacket, int length, int packetNumber)
	{
		releasePacket(pPacket);
	}

private:
	class VniServerControl	  *m_pControl;
	class VniProtocolAnalyzer *m_pProtocolAnalyzer;
};
// Returns temporary error message string.  String will be overwritten
// on each call, so client should copy it if needed longer term.
VNI_PA_CLIENT_DECL LPCSTR VniErrorMessage(HRESULT sts);

// These are some of the valid values that appear as the first byte of a packet when using the
// new interface packetArrivedEx.  This corresponds to the SICB command byte.
#define L2_PKT_TYPE_TIMEOUT				0x00
#define L2_PKT_TYPE_CRC					0x01
#define L2_PKT_TYPE_PACKET_TOO_LONG		0x02
#define L2_PKT_TYPE_PREAMBLE_TOO_LONG	0x03
#define L2_PKT_TYPE_PREAMBLE_TOO_SHORT	0x04
#define L2_PKT_TYPE_PACKET_TOO_SHORT	0x05
#define L2_PKT_TYPE_LOCAL_NM_RESP		0x16	// Response to local NM command
#define L2_PKT_TYPE_INCOMMING			0x1a	// traditional incoming L2 packets
#define L2_PKT_TYPE_MODE1_INCOMMING		0x1b	// mode 1 incoming L2 packets.  
#define L2_PKT_TYPE_FREQUENCY_REPORT	0x40	// Frequency report
#define L2_PKT_TYPE_RESET				0x50	// Reset

#undef VNI_PA_CLIENT_DECL
#endif
