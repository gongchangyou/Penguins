//
//  BattleScene.cpp
//  CrossKaiser
//
//  Created by 龚畅优 on 13-8-27.
//
//

#include "BattleScene.h"
#include "BattleController.h"
#include "Constant.h"
#include "json.h"
#include "ResultScene.h"
#include "SaveData.h"
#include "UserInfo.h"
const Rect ITEM_RECT = Rect(160,0, 160,40);

BattleScene::BattleScene(){
    m_itemSprite = NULL;
    m_seconds = 0;
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

void BattleScene::onEnter(){
    Layer::onEnter();
    __NotificationCenter::getInstance()->addObserver(this, callfuncO_selector(BattleScene::endMission), NC_END_MISSION_TAG, NULL);
    schedule(schedule_selector(BattleScene::updateEvent),1.f);
}
void BattleScene::onExit(){
    Layer::onExit();
    unschedule(schedule_selector(BattleScene::updateEvent));
    __NotificationCenter::getInstance()->removeObserver(this, NC_END_MISSION_TAG);
}

bool BattleScene::init(){
    if (!Layer::init()) {
        return false;
    }
    
    BattleController::shared()->reset();
    
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
    
    //先渲染场上的道具
    __Dictionary *itemList = BattleController::shared()->getItemList();
    DictElement *ele;
    CCDICT_FOREACH(itemList, ele){
        Item *item = dynamic_cast<Item*>(ele->getObject());
        b2Fixture *itemBox2d = item->getB2fixture();
        Point winPos = CommonUtils::convertBox2DToWin(Point(itemBox2d->GetBody()->GetPosition().x, itemBox2d->GetBody()->GetPosition().y));
        Sprite *arrow = Sprite::create("Images/arrows.png");
        arrow->setPosition(winPos);
        this->addChild(arrow);
        item->setPic(arrow);
    }
    
    //往controller里面添加 注意key 如果场上已经有了道具，这个起始点不是从0开始
    //读取数据
    __String *file = __String::createWithFormat("mission/mission%d.json", UserInfo::getInstance()->getCurMissionId());
    std::string filePath = FileUtils::getInstance()->fullPathForFilename(file->getCString());
    Json::Value missionData;
    CommonUtils::fileToJSON(filePath, missionData);
    
    //初始化item位置
    Json::Value useItemList = missionData["useItemList"];
    for (int i=0; i<useItemList.size(); i++) {
        Item * item = Item::create();
        int type = useItemList[i]["type"].asInt();
        item->setType(type);
        Sprite * itemSprite;
        switch (type) {
            case ITEM_CANNON:
                itemSprite = Sprite::create("Images/arrows.png");
                break;
                
            default:
                break;
        }
        
        Point originPoint = Point(160 + 30 *i, 20);
        itemSprite->setPosition(originPoint);
        item->setOriginPoint(originPoint);
        this->addChild(itemSprite);
        item->setPic(itemSprite);
        BattleController::shared()->getItemList()->setObject(item, itemList->count());
    }
    
    
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
    
    DictElement * obj;
    __Dictionary * itemList = BattleController::shared()->getItemList();
    CCDICT_FOREACH(itemList, obj){
        Item * item = dynamic_cast<Item*>(obj->getObject());
        if (item->getIsLocked()) {
            continue;
        }
        if (item->getPic()->getBoundingBox().containsPoint(touch->getLocation())) {
            m_itemSprite = item->getPic();
            m_item = item;
            
            //通知box2d删除b2Fixture
            if (item->getB2fixture()) {
                //这两行顺序不能乱 delItem里面会用到b2Fixture
                m_contactListener->delItem(item);
                item->setB2fixture(NULL);
            }
        }
    }
    return true;
}

void BattleScene::onTouchMoved(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    log("Box2DView::onTouchMoved, pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x, nodePosition.y);
    if ( m_itemSprite ){
        m_itemSprite->setPosition(touchLocation);
        
        //给个阴影展示最终落到何处
        Point itemBox2DPos = CommonUtils::convertWinToBox2D(m_itemSprite->getPosition());
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
    if ( m_itemSprite ){
        if (ITEM_RECT.containsPoint(m_itemSprite->getPosition())) {
            __Dictionary *itemList = BattleController::shared()->getItemList();
            DictElement *ele;
            CCDICT_FOREACH(itemList, ele){
                Item *item = dynamic_cast<Item*>(ele->getObject());
                Point originPoint = item->getOriginPoint();
                if (item->getPic() == m_itemSprite) {
                    m_itemSprite->setPosition(originPoint);
                    return;
                }
            }
        }
        
        Point itemBox2DPos = CommonUtils::convertWinToBox2D(m_itemSprite->getPosition());
        Point itemFinalPos = m_contactListener->getItemFinalPos(itemBox2DPos);
        log("itemFinalPos x = %f, y=%f", itemFinalPos.x, itemFinalPos.y);
        Point endPos = CommonUtils::convertBox2DToWin(itemFinalPos);
        log("endPos x = %f, y=%f", endPos.x, endPos.y);
        //Point finalPos = endPos + Point(0, m_itemSprite->getContentSize().height);
        
        float tmpTime = .1f;
        m_itemSprite->runAction(MoveTo::create(tmpTime, endPos));
        m_itemSprite = NULL;
        
        //告诉box2d 在finalPos处 CommonUtils::convertWinToBox2D 添加炮台或者别的item
        m_item->setTmpBox2dPos(itemFinalPos);
        Action * action =  __CCCallFuncO::create(m_view, callfuncO_selector(Box2DView::setItem), m_item);
        this->runAction(Sequence::create(DelayTime::create(tmpTime), action, NULL));
        
    }
}

void BattleScene::endMission(Object *obj){
    log("battlescene change scene %d", m_seconds);
    
    //更新saveData
    SaveData::getInstance()->setMaxMissionId(UserInfo::getInstance()->getCurMissionId());
    
    ResultScene *resultScene = ResultScene::create();
    resultScene->setSeconds(m_seconds);
    CCDirector::getInstance()->replaceScene(resultScene);
}

void BattleScene::updateEvent(float delta){
    log("delta %f", delta);
    m_seconds ++;
}