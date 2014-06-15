#ifndef _BOX2D_VIEW_H_
#define _BOX2D_VIEW_H_

#include "renderer/CCCustomCommand.h"
#include "OneSidedPlatform.h"

struct TestEntry;
class Test;
class Box2DView : public Layer
{
    EventListenerTouchOneByOne* _touchListener;
    TestEntry*    m_entry;
    OneSidedPlatform*        m_test;
    int            m_entryID;
public:
    Box2DView(void);
    virtual ~Box2DView(void);

    bool initWithEntryID(int entryId);
    std::string title() const;
    void tick(float dt);
    void draw();

//    virtual void registerWithTouchDispatcher();
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    //virtual void accelerometer(UIAccelerometer* accelerometer, Acceleration* acceleration);

    static Box2DView* viewWithEntryID(int entryId);
    
    Point getItemFinalPos(Point itemPos);
protected:
    CustomCommand _customCmd;
    void onDraw();
};

#endif
