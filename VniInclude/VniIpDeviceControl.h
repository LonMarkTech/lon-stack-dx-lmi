//
// VniIpDeviceControl.h
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

// Definitions used to manipulate an IP device to allow the ConfigServer to 
// commission it, used by the LNS IP Configuration Utility.  This object is 
// designed to be "stand-alone", in requires minimal VNI include files, and 
// manages the VniServerControl itself.  


#if !defined(VniIpDeviceControl__INCLUDED_)
#define VniIpDeviceControl__INCLUDED_
#include "VniSts.h"
#ifdef VNICLIENT_EXPORTS
#define VNI_IP_DEVICE_CLIENT_DECL __declspec(dllexport) 
#else
#define VNI_IP_DEVICE_CLIENT_DECL __declspec(dllimport) 
#endif

class VNI_IP_DEVICE_CLIENT_DECL VniIpDeviceControl
{
public:
	VniIpDeviceControl();
	virtual ~VniIpDeviceControl();

    VniSts startConfigServerCheck(const char *deviceName, ULONG csIpAddr = 0, WORD csIpPort = 0);
    VniSts stopConfigServerCheck();
	virtual void configServerTestPassed(ULONG csIpAddr, WORD csPort) {};
    VniSts dumpVniIpChannelXmlConfig(const char *ipDeviceName);

private:
	class VniServerControl	*m_pControl;
	class VniIpDevice		*m_pIpDevice;
};

#undef VNI_IP_DEVICE_CLIENT_DECL
#endif
