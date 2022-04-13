//
// VniSts.h
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

#if !defined(VniSts__INCLUDED_)
#define VniSts__INCLUDED_
#include <windows.h>
#include "rmosts.h"

// Note that RMO, LTA, and DAS all use base errors starting at 0xb000.  This keeps
// the lower 16 bits of the error distinct from NSS errors, having both the reserved
// bit set and a scope of 3.  This facilitates DAS translating NSS errors to and from HRESULTS, 
// without concern of coliding with VNI generated codes. See NS_SRSTS.h for details.
#define LT_ERROR_CODE_BASE                 (RMO_ERROR_CODE_BASE + MAX_RMO_ERRORS)
#define MAX_LT_ERRORS                       0x100
#define MAKE_LT_ERROR(ltError)              MAKE_HRESULT(SEVERITY_ERROR, FACILITY_VNI, (ltError)+LT_ERROR_CODE_BASE)
#define IsLtaSts(sts) IS_XXX_STS(sts, LT_ERROR_CODE_BASE, MAX_LT_ERRORS)
#define LT_STS_CODE(hr) (HRESULT_CODE(hr) - LT_ERROR_CODE_BASE)
    

#define VNI_ERROR_CODE_BASE         (LT_ERROR_CODE_BASE + MAX_LT_ERRORS)
#define MAX_VNI_ERRORS              0x100

typedef  HRESULT VniSts;
#define IsVniSts(sts) IS_XXX_STS(sts, VNI_ERROR_CODE_BASE, MAX_VNI_ERRORS)
#define VNI_STS_CODE(hr) (HRESULT_CODE(hr) - VNI_ERROR_CODE_BASE)
#define VNI_E_GOOD                          NO_ERROR             // 0
// *************************** SUCCESS CODES *************************** 
//
//  Values are 32 bit values layed out as follows:
//
//   3 3 2 2 2 2 2 2 2 2 2 2 1 1 1 1 1 1 1 1 1 1
//   1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0 9 8 7 6 5 4 3 2 1 0
//  +-+-+-+-+-+---------------------+-------------------------------+
//  |S|R|C|N|r|    Facility         |               Code            |
//  +-+-+-+-+-+---------------------+-------------------------------+
//
//  where
//
//      S - Severity - indicates success/fail
//
//          0 - Success
//          1 - Fail (COERROR)
//
//      R - reserved portion of the facility code, corresponds to NT's
//              second severity bit.
//
//      C - reserved portion of the facility code, corresponds to NT's
//              C field.
//
//      N - reserved portion of the facility code. Used to indicate a
//              mapped NT status value.
//
//      r - reserved portion of the facility code. Reserved for internal
//              use. Used to indicate HRESULT values that are not status
//              values, but are instead message ids for display strings.
//
//      Facility - is the facility code
//
//      Code - is the facility's status code
//
//
// Define the facility codes
//
#define FACILITY_VNI                     0x4


//
// Define the severity codes
//
#define VNI_SEVERITY_WARNING             0x2
#define VNI_SEVERITY_SUCCESS             0x0
#define VNI_SEVERITY_INFORMATIONAL       0x1
#define VNI_SEVERITY_ERROR               0x3


//
// MessageId: VNI_S_NO_MORE_VNIS
//
// MessageText:
//
//  There are no more VNIs to be found
//
#define VNI_S_NO_MORE_VNIS               ((VniSts)0x0000B400L)

// *************************** ERROR CODES *************************** 
//
// MessageId: VNI_E_UNKNOWN_REG_ERROR
//
// MessageText:
//
//  Unknown registry error
//
#define VNI_E_UNKNOWN_REG_ERROR          ((VniSts)0xC004B400L)

//
// MessageId: VNI_E_REG_BAD_PARMS
//
// MessageText:
//
//  Bad parameter (registry sub-system)
//
#define VNI_E_REG_BAD_PARMS              ((VniSts)0xC004B401L)

//
// MessageId: VNI_E_REG_OUT_OF_MEMORY
//
// MessageText:
//
//  Out of memory (registry sub-system)
//
#define VNI_E_REG_OUT_OF_MEMORY          ((VniSts)0xC004B402L)

//
// MessageId: VNI_E_REG_CANT_FIND_OBJECT
//
// MessageText:
//
//  Cant find object (registry sub-system)
//
#define VNI_E_REG_CANT_FIND_OBJECT       ((VniSts)0xC004B403L)

//
// MessageId: VNI_E_REG_RESOURCE_PROBLEM
//
// MessageText:
//
//  Resource problem (registry sub-system)
//
#define VNI_E_REG_RESOURCE_PROBLEM       ((VniSts)0xC004B404L)

//
// MessageId: VNI_E_REG_OUT_OF_RANGE
//
// MessageText:
//
//  Parameter out of range (registry sub-system)
//
#define VNI_E_REG_OUT_OF_RANGE           ((VniSts)0xC004B405L)

//
// MessageId: VNI_E_REG_NO_MORE_ENTRIES
//
// MessageText:
//
//  No more entries (registry sub-system)
//
#define VNI_E_REG_NO_MORE_ENTRIES        ((VniSts)0xC004B406L)

//
// MessageId: VNI_E_REG_CANT_OPEN_REGISTRY
//
// MessageText:
//
//  Cannot open registry
//
#define VNI_E_REG_CANT_OPEN_REGISTRY     ((VniSts)0xC004B407L)

//
// MessageId: VNI_E_REG_UNKNOWN_REGISTRY_ERROR
//
// MessageText:
//
//  Unknown error (registry sub-system)
//
#define VNI_E_REG_UNKNOWN_REGISTRY_ERROR ((VniSts)0xC004B408L)

//
// MessageId: VNI_E_VNI_DOES_NOT_EXIST
//
// MessageText:
//
//  Specified VNI does not exist
//
#define VNI_E_VNI_DOES_NOT_EXIST         ((VniSts)0xC004B409L)

//
// MessageId: VNI_E_CANT_CREATE_VNI_PROCESS
//
// MessageText:
//
//  Cannot create the remote process
//
#define VNI_E_CANT_CREATE_VNI_PROCESS    ((VniSts)0xC004B40AL)

//
// MessageId: VNI_E_CANT_LOCK_VNI_SERVER_DIR
//
// MessageText:
//
//  Cannot lock the server directory
//
#define VNI_E_CANT_LOCK_VNI_SERVER_DIR   ((VniSts)0xC004B40BL)

//
// MessageId: VNI_E_OPEN_OBJECT_CONFLICT
//
// MessageText:
//
//  Tried to use two different objects to represent the same VNI object
//
#define VNI_E_OPEN_OBJECT_CONFLICT       ((VniSts)0xC004B40CL)

//
// MessageId: VNI_E_OPEN_FAILURE
//
// MessageText:
//
//  VNI open failure
//
#define VNI_E_OPEN_FAILURE               ((VniSts)0xC004B40DL)

//
// MessageId: VNI_E_NO_MORE_MONITOR_POINTS
//
// MessageText:
//
//  No more monitor points found
//
#define VNI_E_NO_MORE_MONITOR_POINTS     ((VniSts)0xC004B40EL)

//
// MessageId: VNI_E_NO_MORE_MONITOR_SETS
//
// MessageText:
//
//  No more monitor sets found
//
#define VNI_E_NO_MORE_MONITOR_SETS       ((VniSts)0xC004B40FL)

//
// MessageId: VNI_E_STACK_NOT_OPEN
//
// MessageText:
//
//  VniStack is not open
//
#define VNI_E_STACK_NOT_OPEN             ((VniSts)0xC004B410L)

//
// MessageId: VNI_E_NOT_IMPLEMENTED
//
// MessageText:
//
//  Not implemented
//
#define VNI_E_NOT_IMPLEMENTED            ((VniSts)0xC004B411L)

//
// MessageId: VNI_E_MONITOR_SET_NOT_FOUND
//
// MessageText:
//
//  Monitor set not found
//
#define VNI_E_MONITOR_SET_NOT_FOUND      ((VniSts)0xC004B412L)

//
// MessageId: VNI_E_NV_POINT_NOT_FOUND
//
// MessageText:
//
//  NvPoint not found
//
#define VNI_E_NV_POINT_NOT_FOUND         ((VniSts)0xC004B413L)

//
// MessageId: VNI_E_MSG_POINT_NOT_FOUND
//
// MessageText:
//
//  MsgPoint not found
//
#define VNI_E_MSG_POINT_NOT_FOUND        ((VniSts)0xC004B414L)

//
// MessageId: VNI_E_MUST_SPECIFY_MSG_TAG
//
// MessageText:
//
//  Must specify message tag
//
#define VNI_E_MUST_SPECIFY_MSG_TAG       ((VniSts)0xC004B415L)

//
// MessageId: VNI_E_MSG_TAG_NOT_FOUND
//
// MessageText:
//
//  Message tag not found
//
#define VNI_E_MSG_TAG_NOT_FOUND          ((VniSts)0xC004B416L)

//
// MessageId: VNI_E_OUT_OF_TEMPORARY_MONITOR_POINTS
//
// MessageText:
//
//  Monitor set ran out of temporary monitor points due to capacity constraints
//
#define VNI_E_OUT_OF_TEMPORARY_MONITOR_POINTS ((VniSts)0xC004B417L)

//
// MessageId: VNI_E_OUT_OF_TEMPORARY_MONITOR_SETS
//
// MessageText:
//
//  VniStack ran out of temporary monitor sets due to capacity constraints
//
#define VNI_E_OUT_OF_TEMPORARY_MONITOR_SETS ((VniSts)0xC004B418L)

//
// MessageId: VNI_E_ADD_POINT_TO_PERSISTENT_MONITOR_SET
//
// MessageText:
//
//  Attempted to add a point to a persistent Monitor Set
//
#define VNI_E_ADD_POINT_TO_PERSISTENT_MONITOR_SET ((VniSts)0xC004B419L)

//
// MessageId: VNI_E_DEL_POINT_FROM_PERSISTENT_MONITOR_SET
//
// MessageText:
//
//  Attempted to delete a point from a persistent Monitor Set
//
#define VNI_E_DEL_POINT_FROM_PERSISTENT_MONITOR_SET ((VniSts)0xC004B41AL)

//
// MessageId: VNI_E_ABOSOLUTE_REG_PATH_NOT_SUPPORTED
//
// MessageText:
//
//  Absolute registry paths not supported.
//
#define VNI_E_ABOSOLUTE_REG_PATH_NOT_SUPPORTED ((VniSts)0xC004B41BL)

//
// MessageId: VNI_E_REG_BAD_COLLECTION_PATH
//
// MessageText:
//
//  Cannot define a collection of VNIs at this level in the registry.
//
#define VNI_E_REG_BAD_COLLECTION_PATH    ((VniSts)0xC004B41CL)

//
// MessageId: VNI_E_OPEN_STACK_NI_MISMATCH
//
// MessageText:
//
//  The VNI is already open with different network interface settings.
//
#define VNI_E_OPEN_STACK_NI_MISMATCH     ((VniSts)0xC004B41DL)

//
// MessageId: VNI_E_TRACE_FILE_CANT_BE_OPENED
//
// MessageText:
//
//  The specified trace file cannot be opened.
//
#define VNI_E_TRACE_FILE_CANT_BE_OPENED  ((VniSts)0xC004B41EL)

//
// MessageId: VNI_E_NO_MORE_MESSAGE_BUFFERS
//
// MessageText:
//
//  The client has more messages outstanding than permitted.
//
#define VNI_E_NO_MORE_MESSAGE_BUFFERS    ((VniSts)0xC004B41FL)

//
// MessageId: VNI_E_IS_NOT_AN_IP_DEVICE
//
// MessageText:
//
//  The specified device is not IP.
//
#define VNI_E_IS_NOT_AN_IP_DEVICE        ((VniSts)0xC004B420L)

//
// MessageId: VNI_E_L5_MIP_IN_USE
//
// MessageText:
//
//  The specified interface is already used by another VNI.
//
#define VNI_E_L5_MIP_IN_USE              ((VniSts)0xC004B421L)

//
// MessageId: VNI_E_XIF_CANNOT_BE_FOUND
//
// MessageText:
//
//  The specified interface file cannot be found.
//
#define VNI_E_XIF_CANNOT_BE_FOUND        ((VniSts)0xC004B422L)

//
// MessageId: VNI_E_XIF_READ_ERROR
//
// MessageText:
//
//  Error reading the interface file.
//
#define VNI_E_XIF_READ_ERROR             ((VniSts)0xC004B423L)

//
// MessageId: VNI_E_ALLOCATION
//
// MessageText:
//
//  Memory allocation failure.
//
#define VNI_E_ALLOCATION                 ((VniSts)0xC004B424L)

//
// MessageId: VNI_E_PROGRAM_INTERFACE_MISMATCH
//
// MessageText:
//
//  The VNI is already open with a different program interface.
//
#define VNI_E_PROGRAM_INTERFACE_MISMATCH ((VniSts)0xC004B425L)

//
// MessageId: VNI_E_REG_CONFIG_FILE_NOT_FOUND
//
// MessageText:
//
//  The registry backup file cannot be found.
//
#define VNI_E_REG_CONFIG_FILE_NOT_FOUND  ((VniSts)0xC004B426L)

//
// MessageId: VNI_E_REG_CANNOT_UPDATE_CONFIG_FILE
//
// MessageText:
//
//  Cannot update the registry backup file.
//
#define VNI_E_REG_CANNOT_UPDATE_CONFIG_FILE ((VniSts)0xC004B427L)

//
// MessageId: VNI_E_THREAD_CREATION_FAILURE
//
// MessageText:
//
//  Cannot create a VNI thread.
//
#define VNI_E_THREAD_CREATION_FAILURE    ((VniSts)0xC004B428L)

//
// MessageId: VNI_E_DUMP_LTIP_XML_CONFIG_FAILED
//
// MessageText:
//
//  Failure to dump LONWorks/IP channel configuration file.
//
#define VNI_E_DUMP_LTIP_XML_CONFIG_FAILED ((VniSts)0xC004B429L)

//
// MessageId: VNI_E_NOT_SUPPORTED_ON_L5_MIP
//
// MessageText:
//
//  Not supported on a layer 5 MIP.
//
#define VNI_E_NOT_SUPPORTED_ON_L5_MIP    ((VniSts)0xC004B42AL)

//
// MessageId: VNI_E_INVALID_MESSAGE_POINT_OPTIONS
//
// MessageText:
//
//  Invalid message point options.
//
#define VNI_E_INVALID_MESSAGE_POINT_OPTIONS ((VniSts)0xC004B42BL)

#endif
