//
// VniLtDefs.h
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

// Definitions of literals, etc, stolen from the LON Stack.  They must have the 
// same values.
//
// For #defines, just use the same name.  The compiler will catch it if the value
// differs (since there are some files that include both the LT and VNI defs).
// For enums or anything else that must be replicated, add an assert to verify
// that they are equal in VniLtMisc::validateVniLtDefs

#ifndef __VNI_LT_DEFS_INCLUDED__
#define __VNI_LT_DEFS_INCLUDED__

#include "vnidefs.h"
#include "VniInterfaces.h"
#include "LONTalk.h"
#include "LtMonitorSetDefs.h"

// NV definition flags - useable by applications.  First 7 flags values match
// SI data definitions.  Last 3 flags also are related to SI data definitions.
#define NV_SD_CONFIG_CLASS			0x00000001	// Config class NV - keep persistently
#define NV_SD_AUTH_CONFIG			0x00000002	// Authentication attribute configurable
#define NV_SD_PRIORITY_CONFIG		0x00000004	// Priority attribute configurable
#define NV_SD_SERVICE_CONFIG		0x00000008	// Service type configurable
#define NV_SD_OFFLINE				0x00000010	// Only change when offline
#define NV_SD_POLLED				0x00000020	// Polled output or polling input
#define NV_SD_SYNC					0x00000040	// Sync NV

#define NV_SD_CHANGEABLE			0x00000080	// Causes NV arrays to be exploded in SI data.
#define	NV_SD_PRIORITY				0x00000100	// Default to priority
#define NV_SD_AUTHENTICATED			0x00000200	// Default to authenticated
#define NV_SD_ACKD					0x00000400	// Default to ackd service
#define NV_SD_UNACKD_RPT			0x00000800	// Default to unackd rpt service
#define NV_SD_UNACKD				0x00001000	// Default to unackd service

#define NV_SD_AUTH_NONCONFIG		0x00002000	// Authentication attribute non-configurable
#define NV_SD_PRIORITY_NONCONFIG	0x00004000	// Priority attribute non-configurable
#define NV_SD_SERVICE_NONCONFIG		0x00008000	// Service type non-configurable

#define NV_SD_TEMPORARY				0x00010000	// NV is temporary and no persistence should be kept.
#define NV_SD_SOURCE_SELECTION		0x00020000	// Use source selection (for private NVs)
#define NV_SD_READ_BY_SELECTOR		0x00040000  // NV is to be read by selector
#define NV_SD_WRITE_BY_INDEX		0x00080000	// NV is to be written by index
#define NV_SD_NM_AUTH				0x00100000	// Target node has NM auth turned on
#define NV_SD_CHANGEABLE_LENGTH		0x00200000	// NV length may be changed

// Flag values for internal use only

#define NV_SD_NONCONFIG_SHIFT		12			// Delta of config mask and nonconfig masks
#define NV_SD_NONCONFIG_MASK		(NV_SD_AUTH_CONFIG|NV_SD_PRIORITY_CONFIG|NV_SD_SERVICE_CONFIG)
#define NV_SD_FLAGS_MASK			(NV_SD_CONFIG_CLASS|NV_SD_OFFLINE|NV_SD_POLLED|NV_SD_SYNC)
#define NV_SD_PUBLIC_MASK			0x0000FFFF

#define NV_SD_PRIVATE				0x00400000	// Private NV flag
#define NV_SD_MASTER_DELETE			0x00800000	// Indicates owner of master
#define NV_SD_DESCRIPTION_CHANGE	0x01000000	// Indicates non-static description
#define NV_SD_FORKED				0x02000000	// Forked NV array elements
#define NV_SD_MAX_RATE				0x04000000	// Has max rate estimate
#define NV_SD_RATE					0x08000000	// Has rate estimate
#define NV_SD_DESCRIPTION			0x10000000	// Has description
#define NV_SD_NAME					0x20000000	// Has name
#define NV_SD_DYNAMIC				0x40000000	// Dynamic NV
#define NV_SD_OUTPUT				0x80000000	// Output NV

// NV type (SNVT unless one of values below)
typedef unsigned char LtNvType;

typedef enum
{
	VNI_LT_CORRUPTION				= 0x00,		// Image checksum invalid.
	VNI_LT_PROGRAM_ID_CHANGE		= 0x01,		// Program ID changed
	VNI_LT_SIGNATURE_MISMATCH		= 0x02,		// Image signature mismtach.  Could be corruption
											// or change to the persistent data format.
	VNI_LT_PROGRAM_ATTRIBUTE_CHANGE = 0x03,		// Number of NVs, aliases, address or domain
											// entries changed.
	VNI_LT_PERSISTENT_WRITE_FAILURE = 0x04,		// Could not write the persistence file.
	VNI_LT_NO_PERSISTENCE			= 0x05,		// No persistence found.
	VNI_LT_RESET_DURING_UPDATE		= 0x06,		// Reset or power cycle occurred while
											// configuration changes were in progress.
	VNI_LT_VERSION_NOT_SUPPORTED	= 0x07,		// Version number not supported

	VNI_LT_PERSISTENCE_OK			= -1
} VniPersistenceLossReason;

#define LT_OVRD_NONE		0x00

#define LT_OVRD_SERVICE		0x01
#define LT_OVRD_PRIORITY	0x02
#define LT_OVRD_TX_TIMER	0x04
#define LT_OVRD_RPT_TIMER	0x08
#define LT_OVRD_RETRY_COUNT	0x10
#define LT_OVRD_AUTH_KEY	0x20

typedef enum
{   
    VNI_LT_CLASSIC_DOMAIN_STYLE,    // Standard domain definition, with 48 bit key.
    VNI_LT_KEYLESS_DOMAIN_STYLE,    // LONTalk Domain without a key.
    VNI_LT_OMA_DOMAIN_STYLE,        // LONTalk Domain with 96 bit key.
} VniLtLonTalkDomainStyle;

#define LT_UNBOUND_INDEX		0xffff
#define LT_UNUSED_ALIAS			0xffff
#define LT_UNUSED_INDEX			0xffff
#define LT_UNUSED_SELECTOR		0xffff
#define LT_MAX_BOUND_SELECTOR	0x2fff
#define LT_MAX_SELECTOR			0x3fff

#define LT_SELECTION_UNCONDITIONAL		0x00
#define LT_SELECTION_BYSOURCE			0x01
#define LT_SELECTION_NEVER				0x02

#define LT_DOMAIN_LENGTH 6
#define LT_CLASSIC_DOMAIN_KEY_LENGTH 6
#define LT_OMA_DOMAIN_KEY_LENGTH 12

typedef unsigned char       byte;
#endif
