//
//  oneside.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-15.
//
//

#include "OneSidedPlatform.h"
#include "cocos2d.h"
#include "Constant.h"
#include "BattleController.h"
#include "json.h"
#include "CommonUtils.h"
const float VELOCITY = 3.f;
USING_NS_CC;
OneSidedPlatform::OneSidedPlatform(){}
OneSidedPlatform * OneSidedPlatform::Create()
{
    OneSidedPlatform * platform = new OneSidedPlatform();
    platform->init();
    return platform;
}
bool OneSidedPlatform::init()
{
    //读取数据
    std::string filePath = FileUtils::getInstance()->fullPathForFilename("mission/mission0.json");
    Json::Value missionData;
    CommonUtils::fileToJSON(filePath, missionData);
    
    //初始化item位置
    Json::Value itemList = missionData["itemList"];
    for (int i=0; i<itemList.size(); i++) {
        Json::Value itemData = itemList[i];
        int itemType = itemData["type"].asInt();
        float x = itemData["x"].asFloat();
        float y = itemData["y"].asFloat();
        b2BodyDef bd;
        bd.position.Set(x, y);
        b2Body* body = m_world->CreateBody(&bd);
        
        b2PolygonShape shape;
        shape.SetAsBox(0.5f, 1.f);
        b2Fixture *cannon = body->CreateFixture(&shape, 0.0f);
        
        //初始化固定道具，那么需要初始化图片
        Item *item = Item::create();
        item->setB2fixture(cannon);
        item->setType(itemType);
        BattleController::shared()->getItemList()->setObject(item, 0);
    }
    
    // Actor
    {
        for(int i=0; i<10; i++){
            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(-12.0f - i, .0f);
            b2Body* body = m_world->CreateBody(&bd);
            
            m_radius = 0.5f;
            b2CircleShape shape;
            shape.m_radius = m_radius;
            b2Fixture * actor_fixture = body->CreateFixture(&shape, 20.0f);
            actor_fixture->SetFriction(0);
            body->SetLinearVelocity(b2Vec2(VELOCITY, 0.0f));
            
            actor * tmp_actor = new actor();
            tmp_actor->setB2fixture(actor_fixture);
            tmp_actor->setTurnLeft(false);
            m_characters[i] = tmp_actor;
        }
        //m_state = e_unknown;
    }
    
    //edge
    {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody(&bd);

        log("file Path=%s", filePath.c_str());
        Json::Value edgeList = missionData["edgeList"];
        log("edgeList size =%d", edgeList.size());
        for (int i=0; i<edgeList.size(); i++) {
            Json::Value vecStart = edgeList[i]["start"];
            b2Vec2 startV = b2Vec2(vecStart["x"].asFloat(), vecStart["y"].asFloat());
            
            Json::Value vecEnd = edgeList[i]["end"];
            b2Vec2 endV = b2Vec2(vecEnd["x"].asFloat(), vecEnd["y"].asFloat());
            b2EdgeShape shape;
            
            shape.Set(startV, endV);
            b2Fixture * edgeFixture = ground->CreateFixture(&shape, 0.0f);
            CCLOG("b2EdgeShapepos x=%f ,y=%f", edgeFixture->GetBody()->GetPosition().x, edgeFixture->GetBody()->GetPosition().y);
            if (startV.x == endV.x) {
                m_wallList[m_wallList.size()] = edgeFixture;
            }else if(startV.y == endV.y) {
                m_platformList[m_platformList.size()] = edgeFixture;
            }else if(startV.y < endV.y){
                m_upSlopeList[m_upSlopeList.size()] = edgeFixture;
            }else if(startV.y > endV.y){
                m_downSlopeList[m_downSlopeList.size()] = edgeFixture;
            }
        }
    }
    return true;
}


Point OneSidedPlatform::getItemFinalPos(Point itemPos){
    //先看看在edge范围中么
    std::map<int, b2EdgeShape*>edgeList;
    bool inEdge =false;
    for (int i=0; i<m_platformList.size(); i++) {
        b2Fixture * platform = m_platformList[i];
        b2EdgeShape * edgeShape = dynamic_cast<b2EdgeShape*>(platform->GetShape());
        if (itemPos.x >= edgeShape->m_vertex1.x && itemPos.x <= edgeShape->m_vertex2.x) {
            inEdge = true;
            edgeList[edgeList.size()] = edgeShape;
        }
    }
    
    for (int i=0; i<m_upSlopeList.size(); i++) {
        b2Fixture * upSlope = m_upSlopeList[i];
        b2EdgeShape * edgeShape = dynamic_cast<b2EdgeShape*>(upSlope->GetShape());
        if (itemPos.x >= edgeShape->m_vertex1.x && itemPos.x <= edgeShape->m_vertex2.x) {
            inEdge = true;
            edgeList[edgeList.size()] = edgeShape;
        }
    }
    
    for (int i=0; i<m_downSlopeList.size(); i++) {
        b2Fixture * downSlope = m_downSlopeList[i];
        b2EdgeShape * edgeShape = dynamic_cast<b2EdgeShape*>(downSlope->GetShape());
        if (itemPos.x >= edgeShape->m_vertex1.x && itemPos.x <= edgeShape->m_vertex2.x) {
            inEdge = true;
            edgeList[edgeList.size()] = edgeShape;
        }
    }
    
    if (inEdge) {
        
        if (edgeList.size() == 1) {//快速出结果 减少迭代
            b2EdgeShape * edgeShape = edgeList[0];
            float x1 = edgeShape->m_vertex1.x;
            float y1 =edgeShape->m_vertex1.y;
            float x2 = edgeShape->m_vertex2.x;
            float y2 =edgeShape->m_vertex2.y;
            float y = (itemPos.x-x1)*(y2-y1)/(x2-x1) + y1;
            return Point(itemPos.x, y);
        }else{
            //看看有没有在item下面的edge
            float x = itemPos.x;
            float y = itemPos.y;
            std::map<int, b2EdgeShape*>upList;
            std::map<int, b2EdgeShape*>downList;
            for (int i=0; i<edgeList.size(); i++) {
                b2EdgeShape * edgeShape = edgeList[i];
                float x1 = edgeShape->m_vertex1.x;
                float y1 =edgeShape->m_vertex1.y;
                float x2 = edgeShape->m_vertex2.x;
                float y2 =edgeShape->m_vertex2.y;
                if((y-y1)*(x2-x1) - (y2-y1)*(x-x1) >=0 ){//在上面
                    upList[upList.size()] = edgeShape;
                }else{
                    downList[downList.size()] = edgeShape;
                }
            }
            if (upList.size() > 0) {
                float tmpY = 0;
                for (int i=0; i<upList.size(); i++) {
                    b2EdgeShape * edgeShape = upList[0];
                    float x1 = edgeShape->m_vertex1.x;
                    float y1 =edgeShape->m_vertex1.y;
                    float x2 = edgeShape->m_vertex2.x;
                    float y2 =edgeShape->m_vertex2.y;
                    float y = (itemPos.x-x1)*(y2-y1)/(x2-x1) + y1;
                    if (y>tmpY) {
                        tmpY = y;
                    }
                }
                return Point(itemPos.x, tmpY);
            }else{
                float tmpY = 100;
                for (int i=0; i<downList.size(); i++) {
                    b2EdgeShape * edgeShape = downList[i];
                    float x1 = edgeShape->m_vertex1.x;
                    float y1 =edgeShape->m_vertex1.y;
                    float x2 = edgeShape->m_vertex2.x;
                    float y2 =edgeShape->m_vertex2.y;
                    float y = (itemPos.x-x1)*(y2-y1)/(x2-x1) + y1;
                    log("%f= (%f-%f)*(%f-%f)/(%f-%f) + %f",y,itemPos.x,x1,y2,y1,x2,x1,y1 );
                    if (y<tmpY) {
                        tmpY = y;
                    }
                }
                return Point(itemPos.x, tmpY);
            }
            
        }
    }else{//ground
        return Point(itemPos.x, 0);
    }
}

void OneSidedPlatform::setItem(Object *obj){
    Item *item = dynamic_cast<Item*>(obj);
    
    switch (item->getType()) {
        case ITEM_CANNON:
        {
            Point pos = item->getTmpBox2dPos();
            b2BodyDef bd;
            bd.position.Set(pos.x, pos.y);
            b2Body* body = m_world->CreateBody(&bd);
            
            b2PolygonShape shape;
            shape.SetAsBox(0.5f, 1.f);
            b2Fixture * b2fixture = body->CreateFixture(&shape, 0.0f);
            item->setB2fixture(b2fixture);
            
            __Array * keyAry = BattleController::shared()->getItemList()->allKeysForObject(item);
            int key = dynamic_cast<__Integer*>(keyAry->getObjectAtIndex(0))->getValue();
            BattleController::shared()->getItemList()->setObject(item, key);

        }
            break;
            
        default:
            break;
    }
    
}

void OneSidedPlatform::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
{
    Test::PreSolve(contact, oldManifold);
    
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    DictElement * ele;
    __Dictionary * itemList = BattleController::shared()->getItemList();
    CCDICT_FOREACH(itemList, ele){
        Item * item = dynamic_cast<Item*>(ele->getObject());
        b2Fixture * itemBox2d = item->getB2fixture();
        if (fixtureA == itemBox2d || fixtureB == itemBox2d) {
            contact->SetEnabled(false);
            break;
        }
    }
    
    for (int i=0; i<m_wallList.size(); i++) {
        b2Fixture *wall = m_wallList[i];
        if (fixtureA == wall) {
            for (int i =0; i<m_characters.size(); i++) {
                if(fixtureB == m_characters[i]->getB2fixture()){
                    b2Vec2 vec = m_characters[i]->getB2fixture()->GetBody()->GetLinearVelocity();
                    log("vec  i=%d x=%f y=%f",i, vec.x, vec.y);
                    bool turnLeft = vec.x > 0;
                    m_characters[i]->getB2fixture()->GetBody()->SetLinearVelocity(b2Vec2(turnLeft ? -VELOCITY : VELOCITY, vec.y));
                    m_characters[i]->setTurnLeft(turnLeft);
                }
            }
        }
    }
    
    bool aIsCharacter = false;
    bool bIsCharacter = false;
    for (int i =0; i<m_characters.size(); i++) {
        if(fixtureA == m_characters[i]->getB2fixture()){
            aIsCharacter = true;
        }
        if(fixtureB == m_characters[i]->getB2fixture()){
            bIsCharacter = true;
        }
    }
    if (aIsCharacter && bIsCharacter) {
        contact->SetEnabled(false);
    }
    
    for (int i =0; i<m_platformList.size(); i++) {
        b2Fixture* slope = m_platformList[i];
        if (fixtureA == slope) {
            for (int i =0; i<m_characters.size(); i++) {
                if (fixtureB == m_characters[i]->getB2fixture()) {
                    if (m_characters[i]->getB2fixture()->GetBody()->GetLinearVelocity().y > 0) {
                        contact->SetEnabled(false);
                    }
                }
            }
        }
    }
    
    
    for (int i =0; i<m_upSlopeList.size(); i++) {
        b2Fixture* slope = m_upSlopeList[i];
        if (fixtureA == slope) {
            for (int i =0; i<m_characters.size(); i++) {
                if (fixtureB == m_characters[i]->getB2fixture()) {
                    if (m_characters[i]->getFlyFlg()) {
                        log("upSlope continue %d",i);
                        contact->SetEnabled(false);
                        continue;
                    }
                    
                    bool turnLeft = m_characters[i]->getTurnLeft();
                    m_characters[i]->getB2fixture()->GetBody()->SetLinearVelocity(b2Vec2(turnLeft ? -VELOCITY : VELOCITY, 0));
                    
                }
            }
        }
    }
    
    for (int i =0; i<m_downSlopeList.size(); i++) {
        b2Fixture* slope = m_downSlopeList[i];
        if (fixtureA == slope) {
            
            for (int i =0; i<m_characters.size(); i++) {
                if (fixtureB == m_characters[i]->getB2fixture()) {
                    if (m_characters[i]->getFlyFlg()) {
                        contact->SetEnabled(false);
                        continue;
                    }
                    log("downslope %d", i );
                    b2Body * penguin = m_characters[i]->getB2fixture()->GetBody();
                    m_characters[i]->getB2fixture()->GetBody()->SetLinearVelocity(b2Vec2(penguin->GetLinearVelocity().x, -2.f));
                    
                }
            }
        }
    }
}

void OneSidedPlatform::Step(Settings* settings)
{
    Test::Step(settings);
    
    for (int i =0; i<m_characters.size(); i++) {
        if (m_characters[i]->getActionType() > 0) {
            m_characters[i]->setTimes(m_characters[i]->getTimes() + 1);
            switch (m_characters[i]->getActionType()) {
                case ITEM_CANNON:
                    if(m_characters[i]->getTimes() >= 30){
                        m_characters[i]->getB2fixture()->GetBody()->SetLinearVelocity(b2Vec2(3.0f,15.f));
                        m_characters[i]->setActionType( 0 );
                        m_characters[i]->setTimes( 0 );
                        m_characters[i]->setFlyFlg(true);
                    }
                    break;
                    
                default:
                    break;
            }
            
        }
    }
    
    m_debugDraw.DrawString(5, m_textLine, "Press: (c) create a shape, (d) destroy a shape.");
    m_textLine += DRAW_STRING_NEW_LINE;
    
    //        b2Vec2 v = m_character->GetBody()->GetLinearVelocity();
    //        m_debugDraw.DrawString(5, m_textLine, "Character Linear Velocity: %f", v.y);
    m_textLine += DRAW_STRING_NEW_LINE;
}

void OneSidedPlatform::BeginContact(b2Contact* contact)
{
    b2Fixture* fixtureA = contact->GetFixtureA();
    b2Fixture* fixtureB = contact->GetFixtureB();
    
    DictElement * ele;
    __Dictionary * itemList = BattleController::shared()->getItemList();
    CCDICT_FOREACH(itemList, ele){
        Item * item = dynamic_cast<Item*>(ele->getObject());
        b2Fixture *itemBox2d = item->getB2fixture();
        
        if (fixtureA == itemBox2d) {
            contact->SetEnabled(false);
            for (int i =0; i<m_characters.size(); i++) {
                if (fixtureB == m_characters[i]->getB2fixture()) {
                    m_characters[i]->setActionType(item->getType());
                    m_characters[i]->setTimes(0);
                }
            }
        }
    }
    
    for (int i=0; i<m_platformList.size(); i++) {
        b2Fixture *edgeFixture = m_platformList[i];
        if (fixtureA == edgeFixture) {
            for (int i =0; i<m_characters.size(); i++) {
                if (fixtureB == m_characters[i]->getB2fixture()) {
                    m_characters[i]->setFlyFlg(false);
                }
            }
        }
    }
    
    for (int i=0; i<m_upSlopeList.size(); i++) {
        b2Fixture *edgeFixture = m_upSlopeList[i];
        if (fixtureA == edgeFixture) {
            for (int i =0; i<m_characters.size(); i++) {
                if (fixtureB == m_characters[i]->getB2fixture()) {
                    m_characters[i]->setFlyFlg(false);
                }
            }
        }
    }
    
    for (int i=0; i<m_downSlopeList.size(); i++) {
        b2Fixture *edgeFixture = m_downSlopeList[i];
        if (fixtureA == edgeFixture) {
            for (int i =0; i<m_characters.size(); i++) {
                if (fixtureB == m_characters[i]->getB2fixture()) {
                    m_characters[i]->setFlyFlg(false);
                }
            }
        }
    }

}


void OneSidedPlatform::delItem(Item *item){
    m_world->DestroyBody(item->getB2fixture()->GetBody());
}