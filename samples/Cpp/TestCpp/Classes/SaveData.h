//
//  SaveData.h
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-22.
//
//

#ifndef __cocos2d_samples__SaveData__
#define __cocos2d_samples__SaveData__

#include "cocos2d.h"
USING_NS_CC;

class SaveData : public Object{
public:
    static SaveData * getInstance();
    bool init();
    SaveData();
    ~SaveData();
    int getMaxMissionId();
    void  setMaxMissionId(int missionId);
};

#endif /* defined(__cocos2d_samples__SaveData__) */
