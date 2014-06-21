//
//  BattleController.h
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-21.
//
//

#ifndef __cocos2d_samples__BattleController__
#define __cocos2d_samples__BattleController__

#include "cocos2d.h"
USING_NS_CC;
class BattleController : public Object{
public:
    static BattleController   *shared();
    BattleController();
    virtual ~BattleController();
    bool init();
private:
    CC_SYNTHESIZE_RETAIN(__Dictionary*, m_itemList, ItemList);
};

#endif /* defined(__cocos2d_samples__BattleController__) */
