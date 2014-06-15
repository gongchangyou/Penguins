//
//  oneside.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-15.
//
//

#include "OneSidedPlatform.h"
#include "cocos2d.h"
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
