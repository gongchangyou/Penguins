//
//  SaveData.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-22.
//
//

#include "SaveData.h"

const char * MAX_MISSION_ID_KEY = "max_mission_id_key";

SaveData* m_instance = NULL;

SaveData::SaveData(){}
SaveData::~SaveData(){}
SaveData * SaveData::getInstance(){
    if (!m_instance) {
        m_instance = new SaveData();
    }
    
    return m_instance;
}

bool SaveData::init(){
    return true;
}
int SaveData::getMaxMissionId(){
    return UserDefault::getInstance()->getIntegerForKey(MAX_MISSION_ID_KEY);
}

void SaveData::setMaxMissionId(int missionId){
    if (missionId > this->getMaxMissionId()) {
        UserDefault::getInstance()->setIntegerForKey(MAX_MISSION_ID_KEY, missionId);
    }
}