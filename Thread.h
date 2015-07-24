//
//  Thread.h
//  muduo
//
//  Created by 陈希 on 15/7/24.
//  Copyright (c) 2015年 陈希. All rights reserved.
//

#ifndef __muduo__Thread__
#define __muduo__Thread__

#include <pthread.h>
#include "NonCopyable.h"
#include <functional>
#include <string>
#include <memory>
#include <sys/types.h>
#include <atomic>

class Thread : NonCopyable
{
public:
    typedef std::function<void ()> ThreadFunc;
    
    explicit Thread(const ThreadFunc&, const std::string& name = std::string());
    ~Thread();
    
    void start();
    int join();
    
    bool started() const { return _started; }
    
private:
    void setDefaultName();
    
    bool _started;
    bool _joined;
    pthread_t _pthreadId;
    std::shared_ptr<pid_t> _tid;
    ThreadFunc _func;
    std::string _name;
    
    static std::atomic_int _numCreated;
};

#endif /* defined(__muduo__Thread__) */
