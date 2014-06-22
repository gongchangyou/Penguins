//
//  UserInfo.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-22.
//
//

#include "UserInfo.h"

const std::string MAX_MISSION_ID_KEY = "max_mission_id_key";

UserInfo* m_UserInfoInstance = NULL;

UserInfo::UserInfo(){
    m_curMissionId = 0;
}
UserInfo::~UserInfo(){}
UserInfo * UserInfo::getInstance(){
    if (!m_UserInfoInstance) {
        m_UserInfoInstance = new UserInfo();
    }
    return m_UserInfoInstance;
}

bool UserInfo::init(){
    return true;
}
