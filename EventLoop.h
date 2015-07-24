//
//  EventLoop.h
//  muduo
//
//  Created by 陈希 on 15/7/24.
//  Copyright (c) 2015年 陈希. All rights reserved.
//

#ifndef __muduo__EventLoop__
#define __muduo__EventLoop__

#include <unistd.h>
#include "NonCopyable.h"
#include "CurrentThread.h"

class EventLoop : NonCopyable
{
public:
    
    EventLoop();
    ~EventLoop();
    
    void loop();
    
    void assertInLoopThread()
    {
        if (!isInLoopThread())
        {
            abortNotInLoopThread();
        }
    }
    
    bool isInLoopThread() const { return _threadId == CurrentThread::tid(); }
    
    static EventLoop* getEventLoopOfCurrentThread();
private:
    
    void abortNotInLoopThread() {};

    bool _looping;
    const pid_t _threadId;
};

#endif /* defined(__muduo__EventLoop__) */
