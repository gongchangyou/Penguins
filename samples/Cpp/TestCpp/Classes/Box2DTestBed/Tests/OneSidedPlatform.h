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
USING_NS_CC;

class actor:public Object{
private:
    CC_SYNTHESIZE(b2Fixture*, m_character, Character);
    CC_SYNTHESIZE(int, m_times, Times);
    CC_SYNTHESIZE(bool, m_flyFlg, FlyFlg);
public:
    actor(){
        m_times = 0;
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

	OneSidedPlatform()
	{
		// Ground
		{
			b2BodyDef bd;
			b2Body* ground = m_world->CreateBody(&bd);

			b2EdgeShape shape;
			shape.Set(b2Vec2(-20.0f, 0.0f), b2Vec2(20.0f, 0.0f));
			b2Fixture * groundFixture = ground->CreateFixture(&shape, 0.0f);
		}

		// Platform
		{
			b2BodyDef bd;
			bd.position.Set(0.0f, 10.0f);
			b2Body* body = m_world->CreateBody(&bd);

			b2PolygonShape shape;
			shape.SetAsBox(3.0f, 0.5f);
			m_platform = body->CreateFixture(&shape, 0.0f);
			m_bottom = 10.0f - 0.5f;
			m_top = 10.0f + 0.5f;
		}
        // Wall
		{
			b2BodyDef bd;
			bd.position.Set(10.0f, 0.0f);
			b2Body* body = m_world->CreateBody(&bd);
            
			b2PolygonShape shape;
			shape.SetAsBox(3.0f, 3.f);
			m_rock = body->CreateFixture(&shape, 0.0f);
		}
        // cannon
		{
			b2BodyDef bd;
			bd.position.Set(-8.0f, 0.0f);
			b2Body* body = m_world->CreateBody(&bd);
            
			b2PolygonShape shape;
			shape.SetAsBox(0.5f, 1.f);
			m_cannon = body->CreateFixture(&shape, 0.0f);
		}
		// Actor
		{
            for(int i=0; i<10; i++){
                b2BodyDef bd;
                bd.type = b2_dynamicBody;
                bd.position.Set(-12.0f - i, .0f);
                b2Body* body = m_world->CreateBody(&bd);

                m_radius = 0.5f;
                b2CircleShape shape;
                shape.m_radius = m_radius;
                b2Fixture * actor_fixture = body->CreateFixture(&shape, 20.0f);
                body->SetLinearVelocity(b2Vec2(3.0f, 0.0f));
                
                actor * tmp_actor = new actor();
                tmp_actor->setCharacter(actor_fixture);
                m_characters[i] = tmp_actor;
            }
			//m_state = e_unknown;
		}
	}

	void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		Test::PreSolve(contact, oldManifold);

		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();

        if (fixtureA == m_cannon || fixtureB == m_cannon) {
            contact->SetEnabled(false);
        }
        
        bool aIsCharacter = false;
        bool bIsCharacter = false;
        for (int i =0; i<m_characters.size(); i++) {
            if(fixtureA == m_characters[i]->getCharacter()){
                aIsCharacter = true;
            }
            if(fixtureB == m_characters[i]->getCharacter()){
                bIsCharacter = true;
            }
        }
        if (aIsCharacter && bIsCharacter) {
            contact->SetEnabled(false);
        }
//		if (fixtureA != m_platform && fixtureA != m_character)
//		{
//			return;
//		}else{
//            //log("on platform");
//        }
//
//		if (fixtureB != m_platform && fixtureB != m_character)
//		{
//			return;
//		}

//#if 1
//		b2Vec2 position = m_character->GetBody()->GetPosition();
//
//		if (position.y < m_top + m_radius - 3.0f * b2_linearSlop)
//		{
//			contact->SetEnabled(false);
//		}
//#else
//        b2Vec2 v = m_character->GetBody()->GetLinearVelocity();
//        if (v.y > 0.0f)
//		{
//            contact->SetEnabled(false);
//        }
//#endif
	}

	void Step(Settings* settings)
	{
		Test::Step(settings);
        
        for (int i =0; i<m_characters.size(); i++) {
            if (m_characters[i]->getFlyFlg()) {
                m_characters[i]->setTimes(m_characters[i]->getTimes() + 1);
                if(m_characters[i]->getTimes() >= 30){
                    m_characters[i]->getCharacter()->GetBody()->SetLinearVelocity(b2Vec2(3.0f,15.f));
                    m_characters[i]->setFlyFlg( false );
                }
            }
        }

		m_debugDraw.DrawString(5, m_textLine, "Press: (c) create a shape, (d) destroy a shape.");
		m_textLine += DRAW_STRING_NEW_LINE;

//        b2Vec2 v = m_character->GetBody()->GetLinearVelocity();
//        m_debugDraw.DrawString(5, m_textLine, "Character Linear Velocity: %f", v.y);
		m_textLine += DRAW_STRING_NEW_LINE;
	}
    
    void BeginContact(b2Contact* contact)
	{
		b2Fixture* fixtureA = contact->GetFixtureA();
		b2Fixture* fixtureB = contact->GetFixtureB();
        
		if (fixtureA == m_platform)
		{
			log("a is platform");
            fixtureB->GetBody()->SetLinearVelocity(b2Vec2(3.0f, 0.0f));
		}
        
		if (fixtureB == m_platform)
		{
			log("b is platform");
		}
        
        if (fixtureA == m_rock) {
            for (int i =0; i<m_characters.size(); i++) {
                if (fixtureB == m_characters[i]->getCharacter()) {
                    m_characters[i]->getCharacter()->GetBody()->SetLinearVelocity(b2Vec2(-3.0f,0.f));
                }
            }
            
            log("a is rock");
        }
        
        if (fixtureA == m_cannon) {
            contact->SetEnabled(false);
            for (int i =0; i<m_characters.size(); i++) {
                if (fixtureB == m_characters[i]->getCharacter()) {
                    m_characters[i]->setFlyFlg(true);
                    m_characters[i]->setTimes(0);
                }
            }
            //播放动画 结束了之后飞起来
//            m_character->GetBody()->SetTransform(m_cannon->GetBody()->GetPosition(), 0.f);
//            m_character->GetBody()->SetLinearVelocity(b2Vec2(3.0f,15.f));
            log("a is m_cannon");
        }
        
        if (fixtureB == m_rock) {
            log("b is rock");
        }
	}
    
	static Test* Create()
	{
		return new OneSidedPlatform;
	}

    void fly(b2Fixture * m_character){
        m_character->GetBody()->SetLinearVelocity(b2Vec2(3.0f,15.f));
    }
	float32 m_radius, m_top, m_bottom;
	State m_state;
	b2Fixture* m_platform;
    b2Fixture* m_rock;
    b2Fixture* m_cannon;
    
    std::map<int, actor*> m_characters;
    
    
};



#endif
