//
// VniUtil.h
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

// Interfaces for VNI utility classes

#if !defined(VNI_UTIL__INCLUDED_)
#define VNI_UTIL__INCLUDED_

#include "VniDefs.h"
#include "RmoUtil.h"
#include "RmoLists.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


//////////////////////////////////////////////////////////////////////
// VniStr
// Used to store a character string.
//////////////////////////////////////////////////////////////////////

class VNIBASE_DECL VniStr
{
public:
    VniStr() { m_str = NULL; };
    VniStr(const char *str);
    virtual ~VniStr();
    void set(const char *str);
    void setSubstr(const char *str, unsigned int len);
    void cat(const char *str);
    int getLen(void);
    const char *getStr(void) { return m_str; }
    const char *operator&() { return m_str; }
    operator const char *()  { return m_str; }
    void releaseString(void);
private:
    char *m_str;
};

//////////////////////////////////////////////////////////////////////
// VniPtrArrayBase 
// An array of pointers.  Grows dynamically.  
// Should use template VniPtrArray<ObjectType>
//////////////////////////////////////////////////////////////////////
class VNIBASE_DECL VniPtrArrayBase
{
public:
    VniPtrArrayBase(int capacity, int minAllocationUnits);
    virtual ~VniPtrArrayBase();
    void set(int index, void *pData);
    void *get(int index);
    int numElements() { return m_numElements; }
    int capacity() { return m_capacity; }
private:
    void *&getRef(int index);
    int m_numElements;
    int m_capacity;
    int m_minAllocationUnits;
    void **m_pointerArray;

};

//////////////////////////////////////////////////////////////////////
// VniPtrArray<ObjType> 
// An array of pointers to objects of type ObjType. 
//////////////////////////////////////////////////////////////////////
template <class ObjType> class VniPtrArray : public VniPtrArrayBase
{
public:
    VniPtrArray<ObjType>(int capacity, int allocationUnits) :
        VniPtrArrayBase(capacity, allocationUnits) {};

    virtual ~VniPtrArray<ObjType>() {}
    void set(int index, ObjType* pData) { VniPtrArrayBase::set(index, pData); }
    ObjType* get(int index) { return (static_cast<ObjType *>(VniPtrArrayBase::get(index))); }
//  ObjType* &operator[](int index) {return getRef(index); }
};

//////////////////////////////////////////////////////////////////////
// NvValue
// A network variable value 
//////////////////////////////////////////////////////////////////////
class VNIBASE_DECL NvValue : public RmoRefBaseClass
{
public:
        // If copy is TRUE, a buffer will be allocated and the data will
        // be copied into it.  The buffer will be freed upon destruction.
        // Otherwise the object contains a pointer to the user supplied data.
    NvValue(const void *pData, int length, boolean copy = TRUE);
    NvValue();
    virtual ~NvValue();
    void getNvValue(void *pData, int length) const; 
    int  getLen() const { return m_length; }
    const void *getNvValue() const { return m_pData; }
    void setNvValue(const void *pData, int length, boolean copy = TRUE);
    BOOLEAN compare(NvValue &value);
    void set(NvValue &value, boolean copy = TRUE);
    void package(class LtBlob *pBlob);
    static NvValue *allocate(const void *pData, int length, boolean copy = TRUE);
    static NvValue *allocate();

protected:
    void releaseObj();

private:
    boolean     m_allocated;
    int         m_length;
    const void *m_pData;
};

//////////////////////////////////////////////////////////////////////
// VniPackage
// Used to package LTA objects
//////////////////////////////////////////////////////////////////////
class VNIBASE_DECL VniPackage
{
public:
    VniPackage(class LtBlob *pBlob) { m_pBlob = pBlob; };
    void package(class LtMsgIn *pMsgIn);
    void package(class LtRespIn *pRespIn);
private:
    class LtBlob *m_pBlob;
};


//////////////////////////////////////////////////////////////////////
// VniResourceList
// Used to maintain an allocation list of available resources.
//////////////////////////////////////////////////////////////////////

template class VNIBASE_DECL LinkedListHeader<class VniResourceListElement>;

class VNIBASE_DECL VniResourceList
{
public:
    VniResourceList(int first, int last) { setPool(first, last); }
    VniResourceList() {};
    virtual ~VniResourceList();
    void setPool(int first, int last);
    BOOLEAN allocateId(int &id);
    void freeId(int id);
    
private:
    LinkedListHeader<class VniResourceListElement> m_resourceElementHead;
};

class VniResourceListElement : public LinkedList<VniResourceListElement>
{
friend class VniResourceList;
public:
    VniResourceListElement(int first, int last, LinkedListHeader<VniResourceListElement> &head); 
    virtual ~VniResourceListElement() {};
    int allocateId();
    VniResourceListElement *freeId(int id);
    BOOLEAN join(VniResourceListElement *pJoined);
private:
    int m_first;
    int m_last;
};


//////////////////////////////////////////////////////////////////////
// VniDescriptionData
// Description data for monitor sets and points.
//////////////////////////////////////////////////////////////////////

class VNIBASE_DECL VniDescriptionData
{
public:
    VniDescriptionData();
    VniDescriptionData(const void *description, int descLen, boolean copyDesc = TRUE, boolean allocated = FALSE)
    {
        setDescription(description, descLen, copyDesc, allocated);
    }
    virtual ~VniDescriptionData();
    const void *description() const { return m_description; }
    int         descLen()     const { return m_descLen; }
    // If copyDesc is true, memory is allocated for the descriptor and copied, otherwise the
    // the VniDescription just points to the data specified.  The allocated parameter
    // is only when copyDesc is FALSE, and indicates that the description data has been allocated
    // and should be freed by the VniDescrition when it is done with it.
    void setDescription(const void *description, int descLen, boolean copyDesc = TRUE, boolean allocated = FALSE);

private:
    const void             *m_description;
    boolean                 m_descriptionAllocated;
    int                     m_descLen;
};

class VNIBASE_DECL VniDescription 
{
public:
    VniDescription() {};
    VniDescription(const void *description, int descLen, boolean copyDesc = TRUE, boolean allocated = FALSE)
    {
        setDescription(description, descLen, copyDesc, allocated);
    }
    virtual ~VniDescription() {};
    const void *description() const { return m_descriptionData.description(); }
    int         descLen()     const { return m_descriptionData.descLen(); }

    virtual VniSts setDescription(const void *description, int descLen, boolean copyDesc = TRUE, boolean allocated = FALSE)
    {
        setDescriptionData(description, descLen, copyDesc, allocated);
        return(VNI_E_GOOD);
    }

protected:
    void setDescriptionData(const void *description, int descLen, boolean copyDesc, boolean allocated)
    {
        m_descriptionData.setDescription(description, descLen, copyDesc, allocated);
    }

private:
    VniDescriptionData m_descriptionData;
};


//////////////////////////////////////////////////////////////////////
// The following functions are used to convert LT objects to strings.
//////////////////////////////////////////////////////////////////////

extern VNIBASE_DECL void vniLtIncomingAddressToStr(char *sourceAddr, char *destAddr, class LtIncomingAddress &addr);
extern VNIBASE_DECL void vniLtAddressConfigurationToStr(char *addrString, class LtAddressConfiguration &addr, boolean includeRcvTimer);
extern VNIBASE_DECL void vniLtOutgoingAddressToStr(char *addrString, class LtOutgoingAddress &addr);

#endif // !defined(VNI_UTIL__INCLUDED_)
