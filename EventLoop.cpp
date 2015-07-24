//
//  EventLoop.cpp
//  muduo
//
//  Created by 陈希 on 15/7/24.
//  Copyright (c) 2015年 陈希. All rights reserved.
//

#include "EventLoop.h"
#include "poll.h"
#include <cassert>

__thread EventLoop* t_loopInThisThread = 0;

EventLoop::EventLoop()
: _looping(false),
  _threadId(CurrentThread::tid())
{
    if (t_loopInThisThread) {
        
    }
    else
    {
        t_loopInThisThread = this;
    }
}

EventLoop::~EventLoop()
{
    assert(!_looping);
    t_loopInThisThread = NULL;
}

EventLoop* EventLoop::getEventLoopOfCurrentThread()
{
    return t_loopInThisThread;
}

void EventLoop::loop()
{
    assert(!_looping);
    assertInLoopThread();
    _looping = true;
    
    ::poll(NULL, 0, 5*1000);
    
    _looping = false;
}