//
//  NonCopyable.h
//  muduo
//
//  Created by 陈希 on 15/7/24.
//  Copyright (c) 2015年 陈希. All rights reserved.
//

#ifndef muduo_NonCopyable_h
#define muduo_NonCopyable_h

class NonCopyable
{
public:
    NonCopyable() = default;
    ~NonCopyable() {}
    
private:
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable& operator=(const NonCopyable&) = delete;
};

#endif
