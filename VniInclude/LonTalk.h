//
// LonTalk.h
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

#ifndef LonTalk_h
#define LonTalk_h

#define LENGTH(a) (sizeof(a)/sizeof(a[0]))

#define LT_THREAD_PRIORITY  6
    
#define LT_PRIORITY_BUFFER  0
#define LT_NON_PRIORITY_BUFFER  1
#define LT_PRIORITY_TYPES  2
    
#define LT_NUM_REGS  7

#define LT_MAX_TIMEOUT  60000

#define LT_VERSION  0x80

#define MAX_APP_NAME_LEN 8

#define MAX_APDU_SIZE 229
#define MAX_NPDU_SIZE 246
#define MAX_LPDU_SIZE 249
// Obsolete padding for LONC interface on Pentagon processor
//#define LONC_OVERHEAD   4	// Extra space required in front of LPDU by LONC (rounded to nearest 4)
//#define LONC_PKT_SIZE (MAX_LPDU_SIZE + LONC_OVERHEAD)

// Use this instead of obsolete LONC_PKT_SIZE
#define MAX_LINK_PKT_SIZE MAX_LPDU_SIZE

#define LONC_CRC_LENGTH 2

#define MAX_NV_LENGTH 31

#define MAX_LEGACY_ADDRESS_TABLE_ENTRIES 15

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#define max(a,b) (((a) > (b)) ? (a) : (b))
#endif

// System image version number upon which this image is based.
#define LT_SYSTEM_VERSION  11
    
// -1 represents fact that an index has been "escaped"
#define LT_ESCAPE_INDEX 255 
#define LT_MAX_INDEX  255

#define LT_ALL_ECS_INDEX 0xffff

#define LT_NM_ESCAPE_CODE		0x7d

// Manufacturer codes (as assigned by LonMark)
#define ECHELON_LONMARK_MANUFACTURER_CODE			1
// We no longer support compiling for Toshiba
#define TOSHIBA_LONMARK_MANUFACTURER_CODE			76

#define LT_MANUFACTURER	ECHELON_LONMARK_MANUFACTURER_CODE

// Define here the model number reported by the query status command (and the
// minor model number available via read memory).
// We no longer support Toshiba's Pentagon processor
/*
#ifdef __PENTAGON
#define LT_MODEL_NUMBER			0x81
#define LT_MINOR_MODEL_NUMBER	0x0
#else
*/
#define LT_MODEL_NUMBER			0x80
#define LT_MINOR_MODEL_NUMBER	0x0
// #endif

typedef enum 
{
    LT_NO_ERROR					= 0x00,     // @NO_TLB@ 
    LT_INVALID_PARAMETER		= 0x01,
    LT_NOT_QUALIFIED			= 0x02,
    LT_MESSAGE_BLOCKED			= 0x03,
    LT_MESSAGE_DEFERRED			= 0x04,
    LT_APP_MESSAGE				= 0x05,
	LT_FLEX_DOMAIN				= 0x06,
	LT_NO_MESSAGE				= 0x07,
	LT_APP_NAME_TOO_LONG		= 0x08,
	LT_INVALID_STATE			= 0x09,
	LT_NO_RESOURCES				= 0x0A,
	LT_DUPLICATE_OBJECT			= 0x0B,
	LT_NOT_IMPLEMENTED			= 0x0C,
	LT_END_OF_ENUMERATION		= 0x0D,
	LT_OWNER_DOES_NOT_EXIST     = 0x0E,
	LT_INVALID_INDEX			= 0x0F,
	LT_CANT_OPEN_PORT			= 0x10,
	LT_NOT_FOUND				= 0x11,
	LT_NO_WINSOCK_DLL			= 0x12,
	LT_CANT_OPEN_IP_LINK		= 0x13,		// error opening IP Link
	LT_CANT_START_SNMP			= 0x14,		// error starting SNMP in master start
	LT_NO_LINK					= 0x15,		// no link at master object
	LT_INVALID_IPADDRESS		= 0x16,		// invalid IP address supplied to master
    LT_LOCAL_MSG_FAILURE        = 0x17,
    LT_STALE_NV_INDEX           = 0x18,     // NV fetch/poll response/completion event for NV that no longer exists...
	LT_INVALID_ADDRESS			= 0x19,		// bad dm/sb/nd address

    // Error values > = 0x80 are logged in the error log.
    LT_ERROR_LOG_MASK			= 0x80,
    LT_NV_LENGTH_MISMATCH		= 0x82,
    LT_NV_MSG_TOO_SHORT			= 0x83,
    LT_EEPROM_WRITE_FAILURE		= 0x84,
    LT_BAD_ADDRESS_TYPE			= 0x85,
    LT_INVALID_DOMAIN			= 0x8A,
    LT_INVALID_ADDR_TABLE_INDEX = 0x8D,
    LT_NV_UPDATE_ON_OUTPUT_NV	= 0x8F,
    LT_UNKNOWN_PDU				= 0x92,
    LT_INVALID_NV_INDEX			= 0x93,     // Incoming NV fetch or NM req specifies invalid NV index. See also LT_STALE_NV_INDEX 
    LT_BAD_ERROR_NO				= 0x95,
    LT_NET_BUF_TOO_SMALL		= 0x97,
    LT_CNFG_CS_ERROR			= 0x99,
	LT_XCVR_REG_OP_FAILURE		= 0x9B,
	LT_SUBNET_PARTITION			= 0x9F,
    LT_AUTHENTICATION_MISMATCH  = 0xA0,
} LtErrorType;

typedef enum
{
	LT_TPDU				= 0,
	LT_SPDU				= 1,
	LT_AUTHPDU			= 2,
	LT_APDU				= 3,
} LtPduType;

typedef enum
{
	LT_DEFAULT_PATH,
	LT_NORMAL_PATH,
	LT_ALT_PATH,
} LtAlternatePath;

// Node states
typedef enum
{
	LT_UNCONFIGURED		= 2,
	LT_APPLICATIONLESS  = 3,
	LT_CONFIGURED		= 4,
	LT_HARD_OFFLINE		= 6,
} LtNodeState;

// Address Formats are based on those found in the LonTalk packet.
// These are used for incoming addresses.  The first 4 match
// the address formats used in the LonTalk packet.
typedef enum
{
	LT_AF_BROADCAST			= 0,
	LT_AF_GROUP				= 1,
	LT_AF_SUBNET_NODE		= 2,
	LT_AF_UNIQUE_ID			= 3,
	LT_AF_GROUP_ACK			= 4,
	LT_AF_TURNAROUND		= 5,
} LtAddressFormat;

// Address Types are based on those found in the address table.
// These are used for outgoing addresses.
typedef enum
{
	LT_AT_UNBOUND			= 0,
	LT_AT_SUBNET_NODE		= 1,
	LT_AT_UNIQUE_ID			= 2,
	LT_AT_BROADCAST			= 3,
	LT_AT_BROADCAST_GROUP	= 0x23,
	LT_AT_GROUP				= 0x80,
	LT_AT_GROUP_ACK			= 0x81,
	LT_AT_TURNAROUND_ONLY	= 0x82,
	LT_AT_LOCAL				= 0xff,
} LtAddressType;

typedef enum
{
	// First 3 values match LonTalk protocol values
	// for NV configuration.
	LT_ACKD = 0,
	LT_UNACKD_RPT = 1,
	LT_UNACKD = 2,
	LT_REQUEST,
	LT_RESPONSE,
	LT_REMINDER_ACKD,
	LT_REMINDER_REQUEST,
	LT_REMMSG_ACKD,
	LT_REMMSG_REQUEST,
	LT_CHALLENGE,
	LT_REPLY,
	LT_ACK,
    LT_CHALLENGE_OMA,
    LT_REPLY_OMA,

	LT_UNKNOWN,

	LT_SERVICE_TYPES		// Number of service types
} LtServiceType;

#define LT_REMINDER(st) (st>=LT_REMINDER_ACKD && st<=LT_REMMSG_REQUEST)
#define PLAIN_REMINDER(st) (st==LT_REMINDER_ACKD || st==LT_REMINDER_REQUEST)

typedef enum
{
	LT_SERVICE_OFF = 0,
	LT_SERVICE_ON = 1,
	LT_SERVICE_BLINKING = 2,
} LtServiceLedState;

#define LT_CLASSIC_DOMAIN_KEY_LENGTH 6
#define LT_OMA_DOMAIN_KEY_LENGTH 12

#endif
