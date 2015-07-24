//
//  Thread.cpp
//  muduo
//
//  Created by 陈希 on 15/7/24.
//  Copyright (c) 2015年 陈希. All rights reserved.
//

#include "Thread.h"
#include "CurrentThread.h"
#include <type_traits>
#include <sys/syscall.h>
#include <unistd.h>
#include <exception>
#include <cstdlib>
#include <cassert>

namespace CurrentThread {
    __thread int t_cachedTid = 0;
    __thread char t_tidString[32];
    __thread int t_tidStringLength = 6;
    __thread const char* t_threadName = "unknown";
    const bool sameType = std::is_same<int, pid_t>::value;
    static_assert(sameType, "");
}

namespace detail{
    pid_t gettid()
    {
        return static_cast<pid_t>(::syscall(SYS_gettid));
    }
    
    void afterFork()
    {
        CurrentThread::t_cachedTid = 0;
        CurrentThread::t_threadName = "main";
        CurrentThread::tid();
    }
    
    class ThreadNameInitializer
    {
    public:
        ThreadNameInitializer()
        {
            CurrentThread::t_threadName = "main";
            CurrentThread::tid();
            pthread_atfork(NULL, NULL, &afterFork);
        }
    };
    
    ThreadNameInitializer init;
    
    struct ThreadData
    {
        typedef Thread::ThreadFunc ThreadFunc;
        ThreadFunc _func;
        std::string _name;
        std::weak_ptr<pid_t> _wkTid;
        
        ThreadData(const ThreadFunc& func,
                   const std::string& name,
                   const std::shared_ptr<pid_t>& tid)
        : _func(func),
          _name(name),
          _wkTid(tid)
        { }
        
        void runInThread()
        {
            pid_t tid = CurrentThread::tid();
            
            std::shared_ptr<pid_t> ptid = _wkTid.lock();
            if (ptid)
            {
                *ptid = tid;
                ptid.reset();
            }
            
            CurrentThread::t_threadName = _name.empty() ? "Thread" : _name.c_str();
            try
            {
                _func();
                CurrentThread::t_threadName = "finished";
            }
            catch (const std::exception& ex)
            {
                CurrentThread::t_threadName = "crashed";
                fprintf(stderr, "exception caught in Thread %s\n", _name.c_str());
                fprintf(stderr, "reason: %s\n", ex.what());
                abort();
            }
            catch (...)
            {
                CurrentThread::t_threadName = "crashed";
                fprintf(stderr, "unknown exception caught in Thread %s\n", _name.c_str());
                throw;
            }
        }
    };
    
    void* startThread(void* obj)
    {
        ThreadData* data = static_cast<ThreadData*>(obj);
        data->runInThread();
        delete data;
        return NULL;
    }
}

void CurrentThread::cacheTid()
{
    if (t_cachedTid == 0) {
        t_cachedTid = detail::gettid();
        t_tidStringLength = snprintf(t_tidString, sizeof t_tidString, "%5d ", t_cachedTid);
    }
}

bool CurrentThread::isMainTHread()
{
    return tid() == ::getpid();
}

void CurrentThread::sleepUsec(int64_t usec)
{
    struct timespec ts = {0, 0};
    ts.tv_sec = static_cast<time_t>(usec / 1000000);
    ts.tv_nsec = static_cast<long>(usec % 1000000 * 1000);
    ::nanosleep(&ts, NULL);
}

std::atomic_int Thread::_numCreated;

Thread::Thread(const ThreadFunc& func, const std::string& n)
: _started(false),
  _joined(false),
  _pthreadId(0),
  _tid(new pid_t(0)),
  _func(func),
  _name(n)
{
    setDefaultName();
}

Thread::~Thread()
{
    if (_started && !_joined) {
        pthread_detach(_pthreadId);
    }
}

void Thread::setDefaultName()
{
    int num = ++_numCreated;
    if (_name.empty()) {
        char buf[32];
        snprintf(buf, sizeof buf, "Thread%d", num);
        _name = buf;
    }
}

void Thread::start()
{
    assert(!_started);
    _started = true;
    detail::ThreadData* data = new detail::ThreadData(_func, _name, _tid);
    if (pthread_create(&_pthreadId, NULL, &detail::startThread, data)) {
        _started = false;
        delete data;
    }
}

int Thread::join()
{
    assert(_started);
    assert(!_joined);
    _joined = true;
    return pthread_join(_pthreadId, NULL);
}