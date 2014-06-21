//
//  Item.h
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-21.
//
//

#ifndef __cocos2d_samples__Item__
#define __cocos2d_samples__Item__
#include <Box2D/Box2D.h>
#include "cocos2d.h"
#include "Constant.h"
USING_NS_CC;

class Item : public Object{
public:
    Item();
    virtual ~Item();
    CREATE_FUNC(Item);
    bool init();
private:
    CC_SYNTHESIZE_RETAIN(Sprite*, m_pic, Pic);
    CC_SYNTHESIZE(b2Fixture*, m_b2fixture, B2fixture);
    CC_SYNTHESIZE(int, m_type, Type);
    
    CC_SYNTHESIZE(Point, m_tmpBox2dPos, TmpBox2dPos);
};

#endif /* defined(__cocos2d_samples__Item__) */
