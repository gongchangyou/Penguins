//
//  CommonUtils.h
//  cocos2d_samples
//
//  Created by 龚畅优 on 14-6-15.
//
//

#ifndef __CommonUtils__
#define __CommonUtils__
#include "cocos2d.h"
#include "Constant.h"
USING_NS_CC;
namespace CommonUtils{
    Point convertBox2DToWin(Point box2dPoint);
    Point convertWinToBox2D(Point winPoint);
}
using namespace CommonUtils;
#endif /* defined(__cocos2d_samples__CommonUtils__) */
