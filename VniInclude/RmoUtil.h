//
// RmoUtil.h:   Interfaces of RMO utility classes
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

#if !defined(RmoUtil__INCLUDED_)
#define RmoUtil__INCLUDED_

#include "RmoBase.h"

/* This class represets a global RMO lock, based on a critical section. */
class RMO_DECL RmoGlobalLock
{
public:
    RmoGlobalLock();
    ~RmoGlobalLock();
    static void lock();
    static void unlock();
    static boolean isLocked() { return m_lockCount != 0; }
private:
    static int m_lockCount;
    static boolean m_rmoGlobalLockInitialized;    
};

// This class is used to lock RMO global lock, simply by
// defining an object, as a local variable for example.  When the object
// goes out of scope, the lock is released.
class RMO_DECL RmoLock
{
public:
    RmoLock(boolean initiallyLocked = TRUE) 
    {
        m_useCount = 0;
        if (initiallyLocked)
        {
            lock();
        }
    }
    void lock()
    {
        RmoGlobalLock::lock();
        m_useCount++;
    }
    void unlock()
    {
        if (m_useCount != 0)
        {
            RmoGlobalLock::unlock();
            --m_useCount;
        }
    }
    ~RmoLock()
    {
        while (m_useCount--)
        {
            RmoGlobalLock::unlock();
        }
    }
private:
    int m_useCount;
};

// This class is used to keep track of the usage of a resource. A class that is
// derived from this base class will have an initial use count of 0.  Calls to 
// addRef increment the count, calls to release decrement the count.  When the
// count becomes zero, the private pure virtual function "releaseObj" is called to
// tell the object to release itself.
class RMO_DECL RmoRefBaseClass
{
public:
    RmoRefBaseClass() { m_useCount = 0; }
    ~RmoRefBaseClass() {};

    virtual int addRef();
    virtual int release();

protected:
    virtual void releaseObj() = 0;

private:
    int m_useCount;
};


class RMO_DECL RmoPtrBase
{
public:
    RmoPtrBase(RmoRefBaseClass *p);
    ~RmoPtrBase();
    RmoRefBaseClass *get() const;
    void set(RmoRefBaseClass *p);
private:
    RmoRefBaseClass *m_p;
};

template <class T> class RmoPtr {
    RmoPtrBase m_ptrBase;
public:
    RmoPtr(T* p = NULL) : m_ptrBase(p) {}
    ~RmoPtr(void) {};

    operator T*(void) const { return static_cast<T *>(m_ptrBase.get()); }
    T& operator*(void) const { return static_cast<T &>(*m_ptrBase.get()); }
    T* operator->(void) const{ return static_cast<T *>(m_ptrBase.get()); }
    RmoPtr& operator=(RmoPtr<T> &p_) 
        {return operator=((T *) p_);}

    RmoPtr& operator=(T* p_) 
    {
        m_ptrBase.set(p_);
        return *this;
    }
};

#endif
