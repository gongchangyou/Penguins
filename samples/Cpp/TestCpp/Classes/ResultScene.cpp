//
//  ResultScene.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-22.
//
//

#include "ResultScene.h"
#include "CommonUtils.h"

ResultScene::ResultScene(){
    m_seconds = 0;
}
ResultScene::~ResultScene(){}

bool ResultScene::init(){
    if (!Scene::init()) {
        return false;
    }
    
    return true;
}

void ResultScene::onEnter(){
    Scene::onEnter();
    LabelTTF *label = LabelTTF::create(CommonUtils::IntToString(m_seconds), "", 18);
    label->setPosition(Point(100,100));
    this->addChild(label);
}