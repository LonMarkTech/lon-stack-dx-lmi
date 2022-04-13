//
// VniInterfaces.h Common definitions VNI clients interfaces. 
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

#if !defined(_VniInterfaces__INCLUDED__)
#define _VniInterfaces__INCLUDED__
typedef unsigned char		boolean;

#include "RmoUtil.h"

//****************************************************************************
// All exposed VNI client objects are based on VniObjBase or VniSimpleObjBase.  
// All of them are empty interfaces, containin a poiter to the "real" object.
///
// VniObjBase is a reference counted object. If you wish to derive an object 
// using one of these as a base class, you may allocate your derived object, 
// and you must override the "releaseObj" function. This way the object will be 
// both allocated and deleted at the same level.  
// If you wish to use one of these objects directly, you cannot allocate it directly,
// rather you must call the appropriate "newObj" routine and the object will
// be allocated and released under the VniClient DLL.  Note that some objects
// can only be used as base classes, as they have pure virtual functions.  These
// classes have no "newObj" methods.
//     
//****************************************************************************

template <class TObjType> class VniSimpleObjBase 
{
public:
    VniSimpleObjBase<TObjType>() { m_pImplementor = NULL; }
    virtual ~VniSimpleObjBase<TObjType>() {};
    TObjType const *getImplementor() const { return m_pImplementor; }
    TObjType *getImplementor() { return m_pImplementor; }

protected:
    void setImplementor(TObjType *pImplementor) 
    { 
        m_pImplementor = pImplementor; 
    }

private:
    TObjType *m_pImplementor;
};


template <class TObjType> class VniObjBase : public RmoRefBaseClass
{
public:
    VniObjBase<TObjType>() { m_pImplementor = NULL; }
    virtual ~VniObjBase<TObjType>() {};
    TObjType const *getImplementor() const { return m_pImplementor; }
    TObjType *getImplementor() { return m_pImplementor; }

protected:
    void setImplementor(TObjType *pImplementor) 
    { 
        m_pImplementor = pImplementor; 
    }

private:
    TObjType *m_pImplementor;
};


#endif
