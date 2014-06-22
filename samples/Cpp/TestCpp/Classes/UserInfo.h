//
//  UserInfo.h
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-22.
//
//

#ifndef __cocos2d_samples__UserInfo__
#define __cocos2d_samples__UserInfo__

#include "cocos2d.h"
USING_NS_CC;

class UserInfo : public Object{
public:
    static UserInfo * getInstance();
    bool init();
    UserInfo();
    ~UserInfo();
    
    CC_SYNTHESIZE(int, m_curMissionId, CurMissionId);
};

#endif /* defined(__cocos2d_samples__UserInfo__) */
