#include "Box2dView.h"
#include "GLES-Render.h"
#include "Test.h"
#include "renderer/CCRenderer.h"
#include "Constant.h"
#include "VisibleRect.h"
#define kAccelerometerFrequency 30
#define FRAMES_BETWEEN_PRESSES_FOR_DOUBLE_CLICK 10

extern int g_totalEntries;

Settings settings;

enum 
{
    kTagBox2DNode,
};

//------------------------------------------------------------------
//
// Box2DView
//
//------------------------------------------------------------------
Box2DView::Box2DView(void)
{
}

Box2DView* Box2DView::viewWithEntryID(int entryId)
{
    Box2DView* pView = new Box2DView();
    pView->initWithEntryID(entryId);
    pView->autorelease();

    return pView;
}

bool Box2DView::initWithEntryID(int entryId)
{    
    schedule( schedule_selector(Box2DView::tick) );
    m_test = dynamic_cast<OneSidedPlatform*>(OneSidedPlatform::Create());
    
    
//    // Adds Touch Event Listener
//    auto listener = EventListenerTouchOneByOne::create();
//    listener->setSwallowTouches(true);
//    
//    listener->onTouchBegan = CC_CALLBACK_2(Box2DView::onTouchBegan, this);
//    listener->onTouchMoved = CC_CALLBACK_2(Box2DView::onTouchMoved, this);
//    listener->onTouchEnded = CC_CALLBACK_2(Box2DView::onTouchEnded, this);
//    
//    _eventDispatcher->addEventListenerWithFixedPriority(listener, -10);
//    _touchListener = listener;
    
    return true;
}

std::string Box2DView::title() const
{
    return std::string(m_entry->name);
}

void Box2DView::tick(float dt)
{
    m_test->Step(&settings);
}

void Box2DView::draw()
{
    Layer::draw();

    _customCmd.init(_globalZOrder);
    _customCmd.func = CC_CALLBACK_0(Box2DView::onDraw, this);
    Director::getInstance()->getRenderer()->addCommand(&_customCmd);
}

void Box2DView::onDraw()
{
    kmMat4 oldMat;
    kmGLGetMatrix(KM_GL_MODELVIEW, &oldMat);
    kmGLLoadMatrix(&_modelViewTransform);
    GL::enableVertexAttribs( cocos2d::GL::VERTEX_ATTRIB_FLAG_POSITION );
    m_test->m_world->DrawDebugData();
    CHECK_GL_ERROR_DEBUG();
    
    kmGLLoadMatrix(&oldMat);
}

Box2DView::~Box2DView()
{
    // Removes Touch Event Listener
    _eventDispatcher->removeEventListener(_touchListener);
    delete m_test;
}
//
//void Box2DView::registerWithTouchDispatcher()
//{
//    // higher priority than dragging
//    auto director = Director::getInstance();
//    director->getTouchDispatcher()->addTargetedDelegate(this, -10, true);
//}

bool Box2DView::onTouchBegan(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();    

    auto nodePosition = convertToNodeSpace( touchLocation );
    log("Box2DView::onTouchBegan, pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x, nodePosition.y);

    return m_test->MouseDown(b2Vec2(nodePosition.x,nodePosition.y));    
}

void Box2DView::onTouchMoved(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();    
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    log("Box2DView::onTouchMoved, pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x, nodePosition.y);
    
    m_test->MouseMove(b2Vec2(nodePosition.x,nodePosition.y));        
}

void Box2DView::onTouchEnded(Touch* touch, Event* event)
{
    auto touchLocation = touch->getLocation();    
    auto nodePosition = convertToNodeSpace( touchLocation );
    
    log("Box2DView::onTouchEnded, pos: %f,%f -> %f,%f", touchLocation.x, touchLocation.y, nodePosition.x, nodePosition.y);
    
    m_test->MouseUp(b2Vec2(nodePosition.x,nodePosition.y));
}

void Box2DView::setItem(Object *item){
    m_test->setItem(item);
}

// void Box2DView::accelerometer(UIAccelerometer* accelerometer, Acceleration* acceleration)
// {
//     //// Only run for valid values
//     //if (acceleration.y!=0 && acceleration.x!=0)
//     //{
//     //    if (test) test->SetGravity((float)-acceleration.y,(float)acceleration.x);
//     //}
// } 

