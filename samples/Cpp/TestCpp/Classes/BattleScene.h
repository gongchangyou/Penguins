//
//  BattleScene.h
//  CrossKaiser
//
//  Created by 龚畅优 on 13-8-27.
//
//

#ifndef __CrossKaiser__BattleScene__
#define __CrossKaiser__BattleScene__

#include "cocos2d.h"
#include "Constant.h"
#include "CommonUtils.h"
#include "Box2dView.h"
#include "Item.h"
USING_NS_CC;
const int TAG_ITEM = 100;
class BattleScene : public Layer{
public:
    CREATE_FUNC(BattleScene);
    
    BattleScene();
    virtual ~BattleScene();
    
    virtual bool init();
    
    static Scene* scene();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    void endMission(Object *obj);
    
    virtual void onEnter();
    virtual void onExit();
    
    void updateEvent(float delta);
private:
    EventListenerTouchOneByOne * _touchListener;
    Sprite * m_itemSprite;
    Item * m_item;
    Box2DView * m_view;
    OneSidedPlatform * m_contactListener;
    int m_seconds;
    
};
#endif /* defined(__CrossKaiser__BattleScene__) */
