//
//  BattleScene.cpp
//  CrossKaiser
//
//  Created by 龚畅优 on 13-8-27.
//
//

#include "BattleScene.h"

#include "Constant.h"
BattleScene::BattleScene(){
    m_item = NULL;
}

BattleScene::~BattleScene(){
    _eventDispatcher->removeEventListener(_touchListener);
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
    
    m_view = Box2DView::viewWithEntryID( 7 );
    addChild(m_view);
    m_view->setScale(BOX2D_SCALE);
    m_view->setAnchorPoint( Point(0,0) );
    m_view->setPosition( Point(visibleOrigin.x+visibleSize.width/2, visibleOrigin.y+BOX2D_GROUND_Y) );
    log("x=%f, width=%f, y =%f, height=%f",visibleOrigin.x,visibleSize.width/2, visibleOrigin.y,BOX2D_GROUND_Y);
    
    //赋值m_contactListener
    m_contactListener = m_view->getContactListener();
    
    //添加游戏道具
    Sprite *arrow = Sprite::create("Images/arrows.png");
    arrow->setPosition(Point(240, 50));
    this->addChild(arrow, 0, TAG_ITEM);
    
    // Adds Touch Event Listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    
    listener->onTouchBegan = CC_CALLBACK_2(BattleScene::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(BattleScene::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(BattleScene::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithFixedPriority(listener, -10);
    _touchListener = listener;
    
    
    return true;
    
}

bool BattleScene::onTouchBegan(Touch* touch, Event* event)
{
    log("x=%f, y=%f", touch->getLocation().x,touch->getLocation().y);
    if(this->getChildByTag(TAG_ITEM)->getBoundingBox().containsPoint(touch->getLocation())){
        log("contains");
        m_item = dynamic_cast<Sprite*>( this->getChildByTag(TAG_ITEM) );
    }
    return true;
}

void BattleScene::onTouchMoved(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    log("Box2DView::onTouchMoved, pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x, nodePosition.y);
    if ( m_item ){
        m_item->setPosition(touchLocation);
        
        //给个阴影展示最终落到何处
        Point itemBox2DPos = CommonUtils::convertWinToBox2D(m_item->getPosition());
        log("itemBox2DPos x = %f, y=%f", itemBox2DPos.x, itemBox2DPos.y);
        Point itemFinalPos = m_contactListener->getItemFinalPos(itemBox2DPos);
        log("itemFinalPos x = %f, y=%f", itemFinalPos.x, itemFinalPos.y);
    }
}

void BattleScene::onTouchEnded(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    log("Box2DView::onTouchEnded, pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x, nodePosition.y);
    if ( m_item ){
        Point itemBox2DPos = CommonUtils::convertWinToBox2D(m_item->getPosition());
        Point itemFinalPos = m_contactListener->getItemFinalPos(itemBox2DPos);
        log("itemFinalPos x = %f, y=%f", itemFinalPos.x, itemFinalPos.y);
        Point endPos = CommonUtils::convertBox2DToWin(itemFinalPos);
        log("endPos x = %f, y=%f", endPos.x, endPos.y);
        //Point finalPos = endPos + Point(0, m_item->getContentSize().height);
        
        float tmpTime = .1f;
        m_item->runAction(MoveTo::create(tmpTime, endPos));
        m_item = NULL;
        
        //告诉box2d 在finalPos处 CommonUtils::convertWinToBox2D 添加炮台或者别的item
        PickItem *pickItem = PickItem::create();
        pickItem->setType(ITEM_CANNON);
        pickItem->setPos(itemFinalPos);
        Action * action =  __CCCallFuncO::create(m_view, callfuncO_selector(Box2DView::setItem), pickItem);
        this->runAction(Sequence::create(DelayTime::create(tmpTime), action, NULL));
        
        
    }
    
}