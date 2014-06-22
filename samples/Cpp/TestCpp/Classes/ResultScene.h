//
//  ResultScene.h
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-22.
//
//

#ifndef __cocos2d_samples__ResultScene__
#define __cocos2d_samples__ResultScene__

#include "cocos2d.h"
#include "cocos-ext.h"
USING_NS_CC_EXT;
USING_NS_CC;
class ResultScene : public Scene{
    
public:
    CREATE_FUNC(ResultScene);
    
    ResultScene();
    virtual ~ResultScene();
    virtual bool init();
    virtual void onEnter();
private:
    CC_SYNTHESIZE(int, m_seconds, Seconds);
    void goToMenu(Object* sender, Control::EventType event);
};
#endif /* defined(__cocos2d_samples__ResultScene__) */
