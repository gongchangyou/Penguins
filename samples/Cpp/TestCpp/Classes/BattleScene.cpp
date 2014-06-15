//
//  BattleScene.cpp
//  CrossKaiser
//
//  Created by 龚畅优 on 13-8-27.
//
//

#include "BattleScene.h"
#include "Box2dView.h"

BattleScene::BattleScene(){

}

BattleScene::~BattleScene(){

}

Scene * BattleScene::scene(){
    Scene* scene = Scene::create();
    BattleScene *layer = BattleScene::create();
    scene->addChild(layer);
    return scene;
}

bool BattleScene::init(){
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
}