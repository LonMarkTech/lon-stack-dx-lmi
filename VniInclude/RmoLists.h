//
// RmoLists.h:  RMO linked list facility
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

#if !defined(RmoLists__INCLUDED_)
#define RmoLists__INCLUDED_

#include "RmoBase.h"
#include "RmoUtil.h"

class RMO_DECL LinkedListHeaderBase
{
    friend  class LinkedListBase;
public:
    LinkedListHeaderBase() {m_pHead = NULL; m_pTail = NULL;};
    virtual ~LinkedListHeaderBase();

    void *first() const;
    void *last() const;

private:
    LinkedListBase *m_pHead;
    LinkedListBase *m_pTail;
};

class RMO_DECL LinkedListBase
{
private:
    LinkedListHeaderBase *m_pLinkedListHeader;
    LinkedListBase       *m_pNext;
    LinkedListBase       *m_pPrev;
    void                 *m_pObject;
public:

    LinkedListBase() { m_pLinkedListHeader = NULL; m_pNext = NULL; m_pPrev = NULL; m_pObject = NULL;}
    virtual ~LinkedListBase() { removeFromList(); };
    void addToHead(void *m_pObject, LinkedListHeaderBase *pLinkedListHeader);
    void addToTail(void *pObject, LinkedListHeaderBase *pLinkedListHeader);
#ifdef _DEBUG
    void *next() const;
    void *prev() const;
#else
    void *next() const { return ((m_pNext == NULL) ? NULL : m_pNext->m_pObject); }
    void *prev() const { return ((m_pPrev == NULL) ? NULL : m_pPrev->m_pObject); }
#endif
    void *first() const { return ((m_pLinkedListHeader == NULL) ? NULL : m_pLinkedListHeader->first()); }
    void *last() const { return ((m_pLinkedListHeader == NULL) ? NULL : m_pLinkedListHeader->last()); }
    void removeFromList();
    void *getItem() const { return m_pObject; }
};

inline void *LinkedListHeaderBase::first() const { return ((m_pHead == NULL) ? NULL : m_pHead->getItem()); }
inline void *LinkedListHeaderBase::last() const { return ((m_pTail == NULL) ? NULL : m_pTail->getItem()); }

/* Forward reference */
template <class TObjType> class LinkedList; 

template <class TObjType> class LinkedListHeader : public LinkedListHeaderBase
{
public:
    LinkedListHeader<TObjType>() {};
    virtual ~LinkedListHeader<TObjType>() {};

    TObjType *first() const {return static_cast<TObjType *>(LinkedListHeaderBase::first());};
    TObjType *last() const {return static_cast<TObjType *>(LinkedListHeaderBase::last());};

};

template <class TObjType> class LinkedList : public LinkedListBase
{
public:
    LinkedList<TObjType>() {};
    virtual ~LinkedList<TObjType>() {};

    TObjType *next() const { return static_cast<TObjType *>(LinkedListBase::next());}
    TObjType *prev() const {return static_cast<TObjType *>(LinkedListBase::prev());}
    TObjType *first() const {return static_cast<TObjType *>(LinkedListBase::first());}
    TObjType *last() const {return static_cast<TObjType *>(LinkedListBase::last());}

    TObjType *getItem() const {return static_cast<TObjType *>(LinkedListBase::getItem());}
};

template <class TObjType> class SafeLinkedListHeader : public LinkedListHeaderBase
{
public:
    SafeLinkedListHeader<TObjType>() {};
    virtual ~SafeLinkedListHeader<TObjType>() {};

    boolean first(RmoPtr<TObjType> &obj) const 
    { 
        RmoLock lock;
        obj = static_cast<TObjType *>(LinkedListHeaderBase::first());
        return obj != NULL;
    };

    boolean last() const 
    { 
        RmoLock lock;
        obj = static_cast<TObjType *>(LinkedListHeaderBase::last());
        return obj != NULL;
    };

};

template <class TObjType> class SafeLinkedList : public LinkedListBase
{
public:
    SafeLinkedList<TObjType>() {};
    virtual ~SafeLinkedList<TObjType>() {};

    boolean next(RmoPtr<TObjType> &obj) const 
    { 
        RmoLock lock;
        obj = static_cast<TObjType *>(LinkedListBase::next());
        return obj != NULL;
    }
    boolean prev(RmoPtr<TObjType> &obj) const 
    { 
        RmoLock lock;
        obj = static_cast<TObjType *>(LinkedListBase::prev());
        return obj != NULL;
    }
    boolean first(RmoPtr<TObjType> &obj) const 
    { 
        RmoLock lock;
        obj = static_cast<TObjType *>(LinkedListBase::first());
        return obj != NULL;
    }
    boolean last(RmoPtr<TObjType> &obj) const 
    { 
        RmoLock lock;
        obj = static_cast<TObjType *>(LinkedListBase::last());
        return obj != NULL;
    }

    boolean getItem(RmoPtr<TObjType> &obj) const 
    {
        RmoLock lock;
        obj = static_cast<TObjType *>(LinkedListBase::getItem());
        return obj != NULL;
    }
};

#endif



