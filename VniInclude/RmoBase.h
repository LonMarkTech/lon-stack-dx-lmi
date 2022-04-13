//
// RmoBase.h:  Basic RMO definitions
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

#if !defined(RmoBase__INCLUDED_)
#define RmoBase__INCLUDED_
#include "RmoSts.h"

#ifdef RMO_EXPORTS
#define RMO_DECL __declspec(dllexport) 
#else
#define RMO_DECL __declspec(dllimport) 
#endif

typedef unsigned int RmoClientId;
#define NO_RMO_CLIENT_ID ((RmoClientId)(0xFFFFFFFFU))

typedef unsigned int RmoMethodReturnId;
#define RMO_METHOD_RETURN_ID_NONE 0

typedef BYTE RmoClassType;      
#define NO_RMO_CLASS_TYPE ((RmoClassType)-1)
typedef WORD RmoMethodType;

/* The following methods are gobal */
enum 
{
    MT_OBJECT_DESTRUCTOR = 0,
    MT_REMOTE_OBJECT_OBJECT_REGISTRATION,

    MT_METHOD_BASE = 100,
    /* Class specific methods are defined starting at MT_METHOD_BASE */
};

#define NO_PROCESS_ID 0

typedef unsigned long int Msec;

#endif
