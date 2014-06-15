//
//  PickItem.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-15.
//
//

#include "PickItem.h"

PickItem::PickItem(){
    m_type = 0;
    m_sprite = NULL;
}
PickItem::~PickItem(){
    CC_SAFE_RELEASE_NULL(m_sprite);
}

bool PickItem::init(){
    return true;
}