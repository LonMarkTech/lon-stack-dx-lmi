//
// VniDefs.h:  Common VNI definitions
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

#if !defined(VniDefs__INCLUDED_)
#define VniDefs__INCLUDED_

#ifdef VNIBASE_EXPORTS
#define VNIBASE_DECL __declspec(dllexport) 
#else
#define VNIBASE_DECL __declspec(dllimport) 
#endif

#ifdef VNICLIENT_EXPORTS
#define VNICLIENT_DECL __declspec(dllexport) 
#else
#define VNICLIENT_DECL __declspec(dllimport) 
#endif


#include "VniSts.h"

typedef enum VniClassType
{
    VniClassId_ObjectControl,
    VniClassId_VniStack,
    VniClassId_VniMonitorSet,
    VniClassId_VniNvPoint,
    VniClassId_VniMsgPoint,
	VniClassId_VniIpDevice,
    VniClassId_VniProtocolAnalyzer,
    VniClassId_VniL5Mip,
    VniClassId_VniLicensetControl,
} VniClassType;

// This enum is used to determine when a value should be read from the network and
// when to read it from the VNI servers local copy.

typedef enum 
{
    ALWAYS_USE_LOCAL_VALUE,     // Read the value from the VNI server.
    ALWAYS_USE_REMOTE,          // Fetch the variable from the network.
    USE_REMOTE_UNLESS_MP_IS_LOCAL, // Fetch from network unless the monitor point corresponds to
                                // a local value.
    USE_LOCAL_IF_KNOWN,         // Read the value form the VNI server it its is currently "known".
                                // Otherwise, fetch it from the network.
    USE_LOCAL_IF_NOT_STALE,     // Read the from the server it is no older than specified,
} NetAccessCondition;

// This enum is when an NV update arrives to specify the type of update
typedef enum NvUpdateType 
{
    NV_UPDATE_BY_FETCH_RESPONSE,    // Response to a FETCH arrived.
    NV_UPDATE_BY_ANOTHERS_FETCH,    // NV fetched by someone else.
    NV_UPDATE_BY_LOCAL_VALUE_READ,  // The local value of the NV has been read. 
    NV_UPDATE_BY_UPDATE_NV,         // NV was sent to the VNI.
    NV_UPDATE_BY_CLIENT,            // NV was updated by the client.
} NvUpdateType;

typedef enum VniMonitorEnableType
{
    MONITOR_DISABLED,
    POLLING_DISABLED,   /* But updates are enabled, but polling is not */
    MONITOR_ENABLED,    /* Polling (if defined) and updates are enabled. */
} VniMonitorEnableType;

#define TEMPORARY_MONITOR_SET_BIT  0x8000

typedef unsigned long  VniMsgTagId;
typedef unsigned long  VniMsgInXrefId;
typedef unsigned long  VniNvTagId;

#define VNI_NULL_NV_TAG_ID 0

typedef enum 
{
    VNI_TRACE_NONE            = 0,
	VNI_TRACE_ERRORS          = 0x0001,
	VNI_TRACE_NV_UPDATES      = 0x0002,
	VNI_TRACE_NV_NOTIFICTAION = 0x0004,
	VNI_TRACE_NV_FAILURES     = 0x0008,
	VNI_TRACE_NV_SUCCESS      = 0x0010,
	VNI_TRACE_MESSAGE_IN	  = 0x0020,		// Message in and response out.	
	VNI_TRACE_MESSAGE_OUT	  = 0x0040,		// Message out and response in.
	VNI_TRACE_MSG_FAILURES	  = 0x0080,		// In and out.
	VNI_TRACE_MSG_SUCCESS	  = 0x0100,		// In and out.

    VNI_TRACE_OBJECT_CREATION = 0x0200,
    VNI_TRACE_VXL             = 0x0400,
    VNI_TRACE_NV_FETCH        = 0x0800, 
    VNI_TRACE_SET_NV          = 0x1000,
    VNI_TRACE_NV_POLLING_CONTROL = 0x2000,
    VNI_TRACE_NV_FAILED_DEVICE_CONTROL = 0x4000,
    VNI_TRACE_SIM_L5MIP_MESSAGES = 0x8000,

    VNI_TRACE_LICENSE_URGENT  = 0x10000,
    VNI_TRACE_LICENSE_VERBOSE = 0x20000,

    VNI_TRACE_NVPOINT_OPTIONS = 0x40000,

	VNI_TRACE_REPORT          = 0x80000000,     // Always report 	
} VniTraceType;

typedef enum 
{
	VNI_TRACE_CLIENT_ALL,
	VNI_TRACE_CLIENT_BY_PROCESS_ID,
	VNI_TRACE_CLIENT_BY_CLIENT_ID,
} VniTraceClientType;

    // TBD - should be moved to LTA...
#define LT_MP_USE_LOCAL_VALUE (1 << 3)

    // Options that cannot be set or reset by client.
#define LT_MP_CONFIG_OPTIONS (LT_MP_USE_BOUND_UPDATES | LT_MP_USE_LOCAL_VALUE)

#define VNI_DEFAULT_NUM_MONITOR_NV_ENTRIES    10000
#define VNI_DEFAULT_NUM_MONITOR_POINT_ENTRIES 10000
#define VNI_DEFAULT_NUM_MONITOR_SET_ENTRIES   5000  
#define VNI_DEFAULT_NUM_MESSAGE_TAGS          0
#define VNI_DEFAULT_MAX_PRIVATE_NVS           8000
#define VNI_DEFAULT_NUM_RECIEVE_TRANSACTIONS  32768 // One for every subnet/node in a domain (255*127) + plus some.
#define VNI_DEFAULT_NUM_TRANSMIT_TRANSACTIONS 32768 // "
#define VNI_DEFAULT_TRANSACTION_ID_LIFETIME   24576 /* Maximum neuron recieve timer */
#define VNI_DEFAULT_MESSAGE_EVENT_MAXIMUM      250
#define VNI_DEFAULT_MESSAGE_OUT_MAXIMUM        250
#define VNI_DEFAULT_PRIORITY_MESSAGE_OUT_MAXIMUM  250

typedef unsigned long VniDirVersionNum;
typedef unsigned char VniProgramId[8];
typedef unsigned char VniUniqueId[6];

typedef enum
{
    NI_EVENT_LDVX_CONNECTION_CLOSED,
    NI_EVENT_LDVX_CONNECTING,
    NI_EVENT_LDVX_CONNECTION_ESTABLISHED,
    NI_EVENT_LDVX_CONNECTION_FAILED,
    NI_EVENT_LDVX_CONNECTION_DETACHED,
    NI_EVENT_LDVX_CONNECTION_ATTACHED,
} NetworkInterfaceEventType;

#endif
