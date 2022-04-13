//
// RmoSts.h:  Definitions for RMO status codes
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

#if !defined(RmoSts__INCLUDED_)
#define RmoSts__INCLUDED_
#include <windows.h>

// Note that RMO, LTA, and DAS all use base errors starting at 0xb000.  This keeps
// the lower 16 bits of the error distinct from NSS errors, having both the reserved
// bit set and a scope of 3.  This facilitates DAS translating NSS errors to and from HRESULTS, 
// without concern of coliding with VNI generated codes. See NS_SRSTS.h for details.
#define RMO_ERROR_CODE_BASE         0xb200
#define MAX_RMO_ERRORS              0x100

#define MAKE_ERROR(code,base)   MAKE_HRESULT(SEVERITY_ERROR, FACILITY_RMO,((code)+(base)))
#define MAKE_WARNING(code,base) MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_RMO,((code)+(base)))
#define IS_XXX_STS(sts, base, max) ((SCODE_CODE(sts) >= base) && (SCODE_CODE(sts) < (base+max)))
#define IsRmoSts(sts) IS_XXX_STS(sts, RMO_ERROR_CODE_BASE, MAX_RMO_ERRORS)
#define RMO_STS_CODE(hr)        (HRESULT_CODE(hr) - RMO_ERROR_CODE_BASE)

#define RMO_E_GOOD                          NO_ERROR            // 0

typedef HRESULT                        RmoSts;
//////////////////////////////////////////////////////////////////////
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
#define FACILITY_RMO                     0x4


//
// Define the severity codes
//
#define RMO_SEVERITY_WARNING             0x2
#define RMO_SEVERITY_SUCCESS             0x0
#define RMO_SEVERITY_INFORMATIONAL       0x1
#define RMO_SEVERITY_ERROR               0x3


//
// MessageId: RMO_E_MAX_OBJECTS_ALLOCATED
//
// MessageText:
//
//  Maximum objects allocated.
//
#define RMO_E_MAX_OBJECTS_ALLOCATED      ((RmoSts)0xC004B200L)

//
// MessageId: RMO_E_BAD_OBJECT_ID
//
// MessageText:
//
//  Bad object ID
//
#define RMO_E_BAD_OBJECT_ID              ((RmoSts)0xC004B201L)

//
// MessageId: RMO_E_PROCESS_TIMEOUT
//
// MessageText:
//
//  Process timeout
//
#define RMO_E_PROCESS_TIMEOUT            ((RmoSts)0xC004B202L)

//
// MessageId: RMO_E_NO_SERVER_OBJECT
//
// MessageText:
//
//  Remote object does not exist
//
#define RMO_E_NO_SERVER_OBJECT           ((RmoSts)0xC004B203L)

//
// MessageId: RMO_E_NO_IPC_MSG_AVAIL
//
// MessageText:
//
//  No IPC message available
//
#define RMO_E_NO_IPC_MSG_AVAIL           ((RmoSts)0xC004B204L)

//
// MessageId: RMO_E_IPC_UNKNOWN_ERROR
//
// MessageText:
//
//  Unknown IPC  error
//
#define RMO_E_IPC_UNKNOWN_ERROR          ((RmoSts)0xC004B205L)

//
// MessageId: RMO_E_IPC_MSG_ERROR
//
// MessageText:
//
//  IPC messaging error
//
#define RMO_E_IPC_MSG_ERROR              ((RmoSts)0xC004B206L)

//
// MessageId: RMO_E_IPC_RESOURCE_PROBLEM
//
// MessageText:
//
//  IPC resource problem
//
#define RMO_E_IPC_RESOURCE_PROBLEM       ((RmoSts)0xC004B207L)

//
// MessageId: RMO_E_IPC_OUT_OF_MEMORY
//
// MessageText:
//
//  IPC subsystem is out of memory
//
#define RMO_E_IPC_OUT_OF_MEMORY          ((RmoSts)0xC004B208L)

//
// MessageId: RMO_E_IPC_OUT_OF_RANGE
//
// MessageText:
//
//  IPC parameter is out of range
//
#define RMO_E_IPC_OUT_OF_RANGE           ((RmoSts)0xC004B209L)

//
// MessageId: RMO_E_IPC_CANT_FIND_OBJECT
//
// MessageText:
//
//  IPC subsystem cannot find specified object
//
#define RMO_E_IPC_CANT_FIND_OBJECT       ((RmoSts)0xC004B20AL)

//
// MessageId: RMO_E_IPC_LOCK_FAILURE
//
// MessageText:
//
//  IPC subsystem lock failure
//
#define RMO_E_IPC_LOCK_FAILURE           ((RmoSts)0xC004B20BL)

//
// MessageId: RMO_E_CANT_LINK_TO_OBJECT
//
// MessageText:
//
//  Cannot link to remote object
//
#define RMO_E_CANT_LINK_TO_OBJECT        ((RmoSts)0xC004B20CL)


#endif //RmoSts__INCLUDED_
