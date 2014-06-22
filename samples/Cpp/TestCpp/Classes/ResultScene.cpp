//
//  ResultScene.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-22.
//
//

#include "ResultScene.h"
#include "CommonUtils.h"
#include "MenuScene.h"
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
    
    ControlButton * playButton = ControlButton::create();
    playButton->setPosition(Point(200,200));
    playButton->setBackgroundSpriteForState(Scale9Sprite::create("Images/btn-play-normal.png"), Control::State::NORMAL);
    playButton->setBackgroundSpriteForState(Scale9Sprite::create("Images/btn-play-selected.png"), Control::State::HIGH_LIGHTED);
    playButton->setAdjustBackgroundImage(false);
    playButton->addTargetWithActionForControlEvents(this, cccontrol_selector(ResultScene::goToMenu), Control::EventType::TOUCH_UP_INSIDE);
    this->addChild(playButton);
}

void ResultScene::goToMenu(Object* sender, Control::EventType event){
    CCDirector::getInstance()->replaceScene(MenuScene::scene());
}