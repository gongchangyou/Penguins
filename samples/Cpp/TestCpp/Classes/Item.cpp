//
//  Item.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-21.
//
//

#include "Item.h"

Item::Item(){
    m_pic = NULL;
    m_b2fixture = NULL;
    m_type = 0;
    m_tmpBox2dPos = Point::ZERO;
}

Item::~Item(){
    CC_SAFE_RELEASE(m_pic);
}

bool Item::init(){
    return true;
}
