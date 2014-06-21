//
//  BattleController.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-21.
//
//

#include "BattleController.h"
static BattleController *m_pInstance = NULL;

BattleController *BattleController::shared(){
    
    if (m_pInstance == NULL)
    {
        m_pInstance = new BattleController();
        m_pInstance->init();
    }
    return m_pInstance;
}

bool BattleController::init(){
    this->setItemList(__Dictionary::create());
    return true;
}

BattleController::BattleController(){
    m_itemList = NULL;
}

BattleController::~BattleController(){
    CC_SAFE_RELEASE(m_itemList);
}