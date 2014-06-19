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
USING_NS_CC;

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
    PickItem *item = dynamic_cast<PickItem*>(obj);
    switch (item->getType()) {
        case ITEM_CANNON:
        {
            Point pos = item->getPos();
            b2BodyDef bd;
            bd.position.Set(pos.x, pos.y);
            b2Body* body = m_world->CreateBody(&bd);
            
            b2PolygonShape shape;
            shape.SetAsBox(0.5f, 1.f);
            m_cannon = body->CreateFixture(&shape, 0.0f);
        }
            break;
            
        default:
            break;
    }
    
}