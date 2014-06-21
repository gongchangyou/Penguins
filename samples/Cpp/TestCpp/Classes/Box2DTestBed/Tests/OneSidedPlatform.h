/*
* Copyright (c) 2008-2009 Erin Catto http://www.box2d.org
*
* This software is provided 'as-is', without any express or implied
* warranty.  In no event will the authors be held liable for any damages
* arising from the use of this software.
* Permission is granted to anyone to use this software for any purpose,
* including commercial applications, and to alter it and redistribute it
* freely, subject to the following restrictions:
* 1. The origin of this software must not be misrepresented; you must not
* claim that you wrote the original software. If you use this software
* in a product, an acknowledgment in the product documentation would be
* appreciated but is not required.
* 2. Altered source versions must be plainly marked as such, and must not be
* misrepresented as being the original software.
* 3. This notice may not be removed or altered from any source distribution.
*/

#ifndef ONE_SIDED_PLATFORM_H
#define ONE_SIDED_PLATFORM_H
#include "cocos2d.h"
#include "Test.h"
#include "Item.h"
USING_NS_CC;

class actor:public Object{
private:
    CC_SYNTHESIZE(b2Fixture*, m_b2fixture, B2fixture);
    CC_SYNTHESIZE(int, m_times, Times);
    CC_SYNTHESIZE(int, m_actionType, ActionType);
    CC_SYNTHESIZE(bool, m_turnLeft, TurnLeft);
    CC_SYNTHESIZE(bool, m_flyFlg, FlyFlg);
public:
    actor(){
        m_times = 0;
        m_actionType = 0;
        m_turnLeft  = false;
        m_flyFlg = false;
    }
    ~actor(){}
};

class OneSidedPlatform : public Test
{
public:

	enum State
	{
		e_unknown,
		e_above,
		e_below
	};

	OneSidedPlatform();
	
	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);

	void Step(Settings* settings);
    
    void BeginContact(b2Contact* contact);
    
	static OneSidedPlatform* Create();
	
    bool init();
    void fly(b2Fixture * m_character){
        m_character->GetBody()->SetLinearVelocity(b2Vec2(3.0f,15.f));
    }
	float32 m_radius, m_top, m_bottom;
	State m_state;
    
    std::map<int, actor*> m_characters;
    void delItem(Item *item);
    
    
public:
    Point getItemFinalPos(Point itemPos);
    void setItem(Object *item);
    std::map<int, b2Fixture *>m_wallList;
    std::map<int, b2Fixture *>m_platformList;
    std::map<int, b2Fixture *>m_upSlopeList;
    std::map<int, b2Fixture *>m_downSlopeList;
};



#endif
