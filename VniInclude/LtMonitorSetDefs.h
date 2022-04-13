//
// LtMonitorSetDefs.h
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

#ifndef _LTMONITORSETDEFS_H
#define _LTMONITORSETDEFS_H

typedef unsigned short LtMsIndex;
typedef unsigned short LtMpIndex;
typedef unsigned short LtMcIndex;
typedef unsigned short LtMtNvIndex;

#define LT_MC_VALID_DESCRIPTION_ONLY	true
#define LT_MC_ALL						false

// LtMpUpdateOptions option types
#define LT_MP_USE_BOUND_UPDATES (1 << 0)
#define LT_MP_SUPPRESS_POLLING_IF_BOUND (1 << 1)
#define LT_MP_POLL_RESET	     (1 << 2)
#define LT_MP_USE_LOCAL_VALUE	(1 << 3)
#define LT_MP_GENERATE_INITIAL_FETCH (1 << 4)
typedef unsigned char LtMpUpdateOptions;
typedef unsigned long LtMpPollInterval;

// LtMpNotifyOptions option types
#define LT_MP_NOTIFY_CHANGE_ONLY (1 << 0)
typedef unsigned char  LtMpNotifyOptions;
typedef unsigned long  LtMpNotifyInterval;

typedef unsigned char  LtMpFilterOptions;
#define LT_MSG_FILTER_BY_CODE       (1 << 0)
#define LT_MSG_FILTER_BY_ADDRESS    (1 << 1)
#define LT_MSG_FILTER_ACCEPT_NM_ESC (1 << 2)	// Normally NM escape messages are 
												// not forwarded.  This bit enables
												// forwarding them.
#define LT_MSG_FILTER_OUTPUT_ONLY   (1 << 3)    // This is an output only message point.  
                                                // Don't ever route incomming messages with it.
#endif
