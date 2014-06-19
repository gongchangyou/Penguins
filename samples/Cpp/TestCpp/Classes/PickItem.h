//
//  PickItem.h
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-15.
//
//

#ifndef __PickItem__
#define __PickItem__
#include "cocos2d.h"
USING_NS_CC;

class PickItem : public Object{

public:
    PickItem();
    virtual ~PickItem();
    virtual bool init();
    CREATE_FUNC(PickItem);
private:
    CC_SYNTHESIZE(int, m_type, Type);
    CC_SYNTHESIZE(Point, m_pos, Pos);
};

#endif /* defined(__PickItem__) */
