//
//  CommonUtils.cpp
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-15.
//
//

#include "CommonUtils.h"
#include "Constant.h"
#include "json.h"

namespace CommonUtils
{
    Point convertBox2DToWin(Point box2dPoint){
        auto director = Director::getInstance();
        Size visibleSize = director->getVisibleSize();
        
        float x = box2dPoint.x * BOX2D_SCALE + visibleSize.width / 2;
        float y = box2dPoint.y * BOX2D_SCALE + BOX2D_GROUND_Y;
        
        return Point(x, y);
    }

    Point convertWinToBox2D(Point winPoint){
        auto director = Director::getInstance();
        Size visibleSize = director->getVisibleSize();
        
        float x = ( winPoint.x - visibleSize.width / 2 ) / BOX2D_SCALE;
        
        float y = ( winPoint.y - BOX2D_GROUND_Y ) / BOX2D_SCALE;
        
        return Point(x, y);
    }
    
    bool fileToJSON( std::string path, Json::Value &value )
    {
        __String* jsonStr = __String::createWithContentsOfFile(path.c_str());
        
        Json::Reader reader = Json::Reader();
        return reader.parse(jsonStr->getCString(), value);
    }
    
}