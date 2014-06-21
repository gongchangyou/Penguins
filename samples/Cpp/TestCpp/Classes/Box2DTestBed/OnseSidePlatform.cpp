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
    // Ground
    {
        b2BodyDef bd;
        b2Body* ground = m_world->CreateBody(&bd);
        
        b2EdgeShape shape;
        shape.Set(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
        b2Fixture * groundFixture = ground->CreateFixture(&shape, 0.0f);
    }
    
    // Platform
    {
        b2BodyDef bd;
        bd.position.Set(0.0f, 10.0f);
        b2Body* body = m_world->CreateBody(&bd);
        
        b2PolygonShape shape;
        shape.SetAsBox(3.0f, 0.5f);
        m_platform = body->CreateFixture(&shape, 0.0f);
        m_bottom = 10.0f - 0.5f;
        m_top = 10.0f + 0.5f;
    }
    // Wall
    {
        b2BodyDef bd;
        bd.position.Set(10.0f, 0.0f);
        b2Body* body = m_world->CreateBody(&bd);
        
        b2PolygonShape shape;
        shape.SetAsBox(3.0f, 3.f);
        m_rock = body->CreateFixture(&shape, 0.0f);
    }
    // cannon
    {
        b2BodyDef bd;
        bd.position.Set(-8.0f, 0.0f);
        b2Body* body = m_world->CreateBody(&bd);
        
        b2PolygonShape shape;
        shape.SetAsBox(0.5f, 1.f);
        b2Fixture *cannon = body->CreateFixture(&shape, 0.0f);
        
        //初始化固定道具，那么需要初始化图片
        Item *item = Item::create();
        item->setB2fixture(cannon);
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
            body->SetLinearVelocity(b2Vec2(3.0f, 0.0f));
            
            actor * tmp_actor = new actor();
            tmp_actor->setCharacter(actor_fixture);
            m_characters[i] = tmp_actor;
        }
        //m_state = e_unknown;
    }
    
    return true;
}


Point OneSidedPlatform::getItemFinalPos(Point itemPos){
    if (itemPos.y < 0) {
        return Point(itemPos.x, 0);
    }else if(itemPos.y > m_platform->GetBody()->GetPosition().y &&
             itemPos.x <= m_platform->GetBody()->GetPosition().x + 1.5 &&
             itemPos.x >= m_platform->GetBody()->GetPosition().x - 1.5
             ){
        return Point(itemPos.x, m_platform->GetBody()->GetPosition().y);
    }else{
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
    
    
    bool aIsCharacter = false;
    bool bIsCharacter = false;
    for (int i =0; i<m_characters.size(); i++) {
        if(fixtureA == m_characters[i]->getCharacter()){
            aIsCharacter = true;
        }
        if(fixtureB == m_characters[i]->getCharacter()){
            bIsCharacter = true;
        }
    }
    if (aIsCharacter && bIsCharacter) {
        contact->SetEnabled(false);
    }
    //		if (fixtureA != m_platform && fixtureA != m_character)
    //		{
    //			return;
    //		}else{
    //            //log("on platform");
    //        }
    //
    //		if (fixtureB != m_platform && fixtureB != m_character)
    //		{
    //			return;
    //		}
    
    //#if 1
    //		b2Vec2 position = m_character->GetBody()->GetPosition();
    //
    //		if (position.y < m_top + m_radius - 3.0f * b2_linearSlop)
    //		{
    //			contact->SetEnabled(false);
    //		}
    //#else
    //        b2Vec2 v = m_character->GetBody()->GetLinearVelocity();
    //        if (v.y > 0.0f)
    //		{
    //            contact->SetEnabled(false);
    //        }
    //#endif
}

void OneSidedPlatform::Step(Settings* settings)
{
    Test::Step(settings);
    
    for (int i =0; i<m_characters.size(); i++) {
        if (m_characters[i]->getFlyFlg()) {
            m_characters[i]->setTimes(m_characters[i]->getTimes() + 1);
            if(m_characters[i]->getTimes() >= 30){
                m_characters[i]->getCharacter()->GetBody()->SetLinearVelocity(b2Vec2(3.0f,15.f));
                m_characters[i]->setFlyFlg( false );
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
    
    if (fixtureA == m_platform)
    {
        //log("a is platform");
        fixtureB->GetBody()->SetLinearVelocity(b2Vec2(3.0f, 0.0f));
    }
    
    if (fixtureB == m_platform)
    {
        //log("b is platform");
    }
    
    if (fixtureA == m_rock) {
        for (int i =0; i<m_characters.size(); i++) {
            if (fixtureB == m_characters[i]->getCharacter()) {
                m_characters[i]->getCharacter()->GetBody()->SetLinearVelocity(b2Vec2(-3.0f,0.f));
            }
        }
        
        //log("a is rock");
    }
    
    DictElement * ele;
    __Dictionary * itemList = BattleController::shared()->getItemList();
    CCDICT_FOREACH(itemList, ele){
        Item * item = dynamic_cast<Item*>(ele->getObject());
        b2Fixture *itemBox2d = item->getB2fixture();
        
        if (fixtureA == itemBox2d) {
            contact->SetEnabled(false);
            for (int i =0; i<m_characters.size(); i++) {
                if (fixtureB == m_characters[i]->getCharacter()) {
                    m_characters[i]->setFlyFlg(true);
                    m_characters[i]->setTimes(0);
                }
            }
        }
    }
    
    if (fixtureB == m_rock) {
        //log("b is rock");
    }
}