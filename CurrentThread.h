//
//  CurrentThread.h
//  muduo
//
//  Created by 陈希 on 15/7/24.
//  Copyright (c) 2015年 陈希. All rights reserved.
//

#ifndef muduo_CurrentThread_h
#define muduo_CurrentThread_h

#include <stdint.h>

namespace CurrentThread {
    extern __thread int t_cachedTid;
    extern __thread char t_tidString[32];
    extern __thread int t_tidStringLength;
    extern __thread const char* t_threadName;
    void cacheTid();
    
    inline int tid()
    {
        if (__builtin_expect(t_cachedTid == 0, 0))
        {
            cacheTid();
        }
        return t_cachedTid;
    }
    
    inline const char* tidString()
    {
        return t_tidString;
    }
    
    inline int tidStringLength()
    {
        return t_tidStringLength;
    }
    
    inline const char* name()
    {
        return t_threadName;
    }
    
    bool isMainTHread();
    
    void sleepUsec(int64_t usec);
}

#endif
