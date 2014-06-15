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
USING_NS_CC;

class BattleScene : public Layer{
public:
    CREATE_FUNC(BattleScene);
    virtual bool init(){
        if (!Layer::init()) {
            return false;
        }
        
        auto director = Director::getInstance();
        Point visibleOrigin = director->getVisibleOrigin();
        Size visibleSize = director->getVisibleSize();
        
        Box2DView* view = Box2DView::viewWithEntryID( 7 );
        addChild(view);
        view->setScale(15);
        view->setAnchorPoint( Point(0,0) );
        view->setPosition( Point(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+visibleSize.height/3) );
        return true;

    };
    
    static Scene* scene(){
        Scene* scene = Scene::create();
        BattleScene *layer = BattleScene::create();
        scene->addChild(layer);
        return scene;
    };
    
    BattleScene(){};
    virtual ~BattleScene(){};
    
};
#endif /* defined(__CrossKaiser__BattleScene__) */
