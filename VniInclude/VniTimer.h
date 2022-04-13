//
// VniTimer.h
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

// Interface for the Vni timer classes 
// VniTimeStamp, VniAlarm and VniTimerControl

#if !defined(VNITIMER__INCLUDED_)
#define VNITIMER__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "VniDefs.h"
#include "VniUtil.h"

// #define DEBUG_VNI_TIMERS

//////////////////////////////////////////////////////////////////////
// VniTimeStamp
// A simple time stamp, with access to elapsed time.
//////////////////////////////////////////////////////////////////////
class VNIBASE_DECL VniTimeStamp
{
public:
    VniTimeStamp() { set(); }
    virtual ~VniTimeStamp() {};
    void set(void) { m_timeSet = currentTime(); }
    void set(Msec time) { m_timeSet = time; }
    static Msec currentTime();
    Msec elapsedTime(void) const { return (elapsedTime(m_timeSet, currentTime())); }
    Msec elapsedTime(Msec currentTime) const { return (elapsedTime(m_timeSet, currentTime)); }
    static Msec elapsedTime(Msec timerStarted, Msec currentTime) { return (currentTime - timerStarted); }
    Msec getTimeSet() const { return m_timeSet; }


private:
    Msec m_timeSet;

#ifdef DEBUG_VNI_TIMERS
public:
    static void    printTime();
    static boolean traceDetails() { return m_traceDetails; }
private:
    static boolean m_traceDetails;
#endif
};

//////////////////////////////////////////////////////////////////////
// VniAlarm
// An alarm, which calls a pure virtual method on expiration.
//////////////////////////////////////////////////////////////////////

class VNIBASE_DECL VniAlarm 
{
public:
    VniAlarm(class VniAlarmControl *pControl);
    VniAlarm(Msec duration, class VniAlarmControl *pControl);
    VniAlarm();

    virtual ~VniAlarm();
    
    /* Set the timer to the desired duration. */
    void set(Msec duration);

    /* Remove the timer from the time delay queue */
    void clear();

    Msec timeRemaining();
    Msec getDuration();
    Msec getExpiration();
    Msec elapsedTime(void) const;
    Msec elapsedTime(Msec currentTime);
    Msec getTimeSet() const;

    virtual void timeOut() = 0;

    virtual void lockTimer() {};
    virtual void unlockTimer() {};

protected:
    void registerAlarmControl(class VniAlarmControl *pControl);

private:
    class VniAlarmImpl *m_pImplementor;

public:
    class VniAlarmImpl *getImplementor();
    class VniAlarmImpl const *getImplementor() const;
};

//////////////////////////////////////////////////////////////////////
// VniAlarmControl
// This object controls the alarms.  The app needs to call 
// processTimeout periodically, and should find out how soon to
// call it again using getWaitTimer(). The alarm system can signal
// the app to call processTimeout if it provides an event handle using
// setNotificationHandle. 
//////////////////////////////////////////////////////////////////////
const int VniAlarmControl_TIMER_QUEUE_QUANTUM = 50; // 50 milleseconds.
const int VniAlarmControl_MAX_TIMER_QUEUES  = 200;  // From 0 - 10 seconds in 50 msec increments.

class VNIBASE_DECL VniAlarmControl
{
public:
    VniAlarmControl();
    ~VniAlarmControl();

    void processTimeout();
    Msec getWaitTimer();   
    void setNotificationHandle(HANDLE eventHandle);

private:
    class VniAlarmControlImpl *m_pImplementor;

public:
    class VniAlarmControlImpl *getImplementor();
    class VniAlarmControlImpl const *getImplementor() const;
};


#endif // VNITIMER__INCLUDED_


