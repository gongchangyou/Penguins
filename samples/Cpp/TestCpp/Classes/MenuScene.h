//
//  MenuScene.h
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-22.
//
//

#ifndef __cocos2d_samples__MenuScene__
#define __cocos2d_samples__MenuScene__
#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
class MenuScene : public Layer{
public:
    CREATE_FUNC(MenuScene);
    
    MenuScene();
    virtual ~MenuScene();
    static Scene* scene();
    virtual bool init();
    
    void goToMission(Object* sender, Control::EventType event);
};

#endif /* defined(__cocos2d_samples__MenuScene__) */
