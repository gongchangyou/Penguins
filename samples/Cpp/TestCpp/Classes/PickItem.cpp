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
    m_pos = Point::ZERO;
}
PickItem::~PickItem(){

}

bool PickItem::init(){
    return true;
}