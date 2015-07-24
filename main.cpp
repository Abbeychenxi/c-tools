//
//  main.cpp
//  muduo
//
//  Created by 陈希 on 15/7/24.
//  Copyright (c) 2015年 陈希. All rights reserved.
//

#include <iostream>
#include "CurrentThread.h"
#include "EventLoop.h"
#include "Thread.h"

using namespace std;

EventLoop* g_loop;
void threadFunc()
{
//    printf("threadFunc(): pid= %d, tid= %d\n", getpid(), CurrentThread::tid());
//    EventLoop loop;
//    loop.loop();
    g_loop->loop();
}

int main(int argc, const char * argv[]) {
    // insert code here...
    
//    printf("main(): pid = %d, tid = %d\n", getpid(), CurrentThread::tid());
//    
//    EventLoop loop;
//    
//    Thread thread(threadFunc);
//    thread.start();
//    
//    loop.loop();
//    pthread_exit(NULL);
    
    EventLoop loop;
    g_loop = &loop;
    Thread t(threadFunc);
    t.start();
    t.join();
    
    return 0;
}
