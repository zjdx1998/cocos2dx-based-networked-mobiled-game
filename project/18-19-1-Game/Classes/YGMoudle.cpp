/*
    Description : 摇杆类设计(joystick design)
    Author: 张建东(Zhang Jiandong)
    Date : 2018/8/21
*/
#include "YGMoudle.h"

USING_NS_CC;

YGMoudle * YGMoudle::create(GComponent * mainView)
{
    YGMoudle *pRet = new(std::nothrow) YGMoudle();
    if (pRet && pRet->init(mainView))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool YGMoudle::init(GComponent * mainView)
{
    m_btnPtrButton = mainView->getChild("joystick")->as<GButton>();
    m_btnPtrButton->setChangeStateOnClick(false);

    m_objPtrThumb = m_btnPtrButton->getChild("thumb");
    m_objPtrTouchArea = mainView->getChild("joystick_touch");
    m_objPtrCenter = mainView->getChild("joystick_center");

    m_fInitX = m_objPtrCenter->getX() + m_objPtrCenter->getWidth() / 2;
    m_fInitY = m_objPtrCenter->getY() + m_objPtrCenter->getHeight() / 2;
   // log("(first):::%f , %f", _InitX, _InitY);
    //log("first window:: %f , %f",Director::getInstance()->getRunningScene()->getPosition().x,
//        Director::getInstance()->getRunningScene()->getPosition().y);
   // log("%view: %f , %f", mainView->getPosition().x,mainView->getPosition().y);
    touchId = -1;
    m_iRadius = 150;

    m_objPtrTouchArea->addEventListener(UIEventType::TouchBegin, CC_CALLBACK_1(YGMoudle::onTouchBegin, this));
    m_objPtrTouchArea->addEventListener(UIEventType::TouchMove, CC_CALLBACK_1(YGMoudle::onTouchMove, this));
    m_objPtrTouchArea->addEventListener(UIEventType::TouchEnd, CC_CALLBACK_1(YGMoudle::onTouchEnd, this));


    return true;
}

void YGMoudle::onTouchBegin(EventContext * context)
{
    if (touchId == -1)//First touch
    {
        InputEvent* evt = (InputEvent*)context->getInput();
        touchId = evt->getTouchId();

        m_btnPtrButton->displayObject()->stopActionByTag(1);
        auto screenPos = Director::getInstance()->getRunningScene()->getPosition();
        log("the touchPos : %f , %f and the screenPos %f , %f",
            evt->getPosition().x,evt->getPosition().y,screenPos.x,screenPos.y);
        Vec2 pt = UIRoot->globalToLocal(evt->getPosition()-screenPos);
        pt = evt->getPosition();
        float bx = pt.x;
        float by = pt.y;
    //    log("button test : %f , %f", bx, by);
        m_btnPtrButton->setSelected(true);

        if (bx < 0)
            bx = 0;
        else if (bx > m_objPtrTouchArea->getWidth())
            bx = m_objPtrTouchArea->getWidth();

        if (by > UIRoot->getHeight())
            by = UIRoot->getHeight();
        else if (by < m_objPtrTouchArea->getY())
            by = m_objPtrTouchArea->getY();

        m_flastStageX = bx;
        m_flastStageY = by;
        m_fstartStageX = bx;
        m_fstartStageY = by;

        m_objPtrCenter->setVisible(true);
        m_objPtrCenter->setPosition(bx - m_objPtrCenter->getWidth() / 2, by - m_objPtrCenter->getHeight() / 2);
        m_btnPtrButton->setPosition(bx - m_btnPtrButton->getWidth() / 2, by - m_btnPtrButton->getHeight() / 2);
        //_button->setPosition(_center->getX(), _center->getY());
        
        float deltaX = bx - m_fInitX;
        float deltaY = by - m_fInitY;
        float degrees = atan2(deltaY, deltaX) * 180 / M_PI;
        m_objPtrThumb->setRotation(degrees + 90);

        context->captureTouch();
    }
}

void YGMoudle::onTouchMove(EventContext * context)
{
    InputEvent* evt = (InputEvent*)context->getInput();
    if (touchId != -1 && evt->getTouchId() == touchId)
    {
        Vec2 pt = UIRoot->globalToLocal(evt->getPosition());
        pt = evt->getPosition();
        float bx = pt.x;
        float by = pt.y;
        float moveX = bx - m_flastStageX;
        float moveY = by - m_flastStageY;
        m_flastStageX = bx;
        m_flastStageY = by;
        float buttonX = m_btnPtrButton->getX() + moveX;
        float buttonY = m_btnPtrButton->getY() + moveY;

        float offsetX = buttonX + m_btnPtrButton->getWidth() / 2 - m_fstartStageX;
        float offsetY = buttonY + m_btnPtrButton->getHeight() / 2 - m_fstartStageY;

        float rad = atan2(offsetY, offsetX);
        float degree = rad * 180 / M_PI;
        m_objPtrThumb->setRotation(degree + 90);

        float maxX = m_iRadius * cos(rad);
        float maxY = m_iRadius * sin(rad);
        if (std::abs(offsetX) > std::abs(maxX))
            offsetX = maxX;
        if (std::abs(offsetY) > std::abs(maxY))
            offsetY = maxY;

        buttonX = m_fstartStageX + offsetX;
        buttonY = m_fstartStageY + offsetY;
        if (buttonX < 0)
            buttonX = 0;
        if (buttonY > UIRoot->getHeight())
            buttonY = UIRoot->getHeight();
        m_btnPtrButton->setPosition(buttonX - m_btnPtrButton->getWidth() / 2, buttonY - m_btnPtrButton->getHeight() / 2);

        dispatchEvent(MOVE, nullptr, Value(degree));
    }
}

void YGMoudle::onTouchEnd(EventContext * context)
{
    InputEvent* evt = (InputEvent*)context->getInput();
    if (touchId != -1 && evt->getTouchId() == touchId)
    {
        touchId = -1;
        m_objPtrThumb->setRotation(m_objPtrThumb->getRotation() + 180);
        m_objPtrCenter->setVisible(false);

        /*Action* action = Sequence::createWithTwoActions(ActionFloat2::create(0.3f,
            _button->getPosition(),
            Vec2(_InitX - _button->getWidth() / 2, _InitY - _button->getHeight() / 2), CC_CALLBACK_2(GObject::setPosition, _button)),
        CallFunc::create([this]()
        {
            _button->setSelected(false);
            _thumb->setRotation(0);
            _center->setVisible(true);
            _center->setPosition(_InitX - _center->getWidth() / 2, _InitY - _center->getHeight() / 2);
        }));
        action->setTag(1);

        _button->displayObject()->runAction(action);*/
        m_PtrTweener = GTween::to(m_btnPtrButton->getPosition(), Vec2(m_fInitX - m_btnPtrButton->getWidth() / 2, m_fInitY - m_btnPtrButton->getHeight() / 2), 0.3f)
            ->setTarget(m_btnPtrButton, TweenPropType::Position)
            ->onComplete([this]()
        {
            m_btnPtrButton->setSelected(false);
            m_objPtrThumb->setRotation(0);
            m_objPtrCenter->setVisible(true);
            m_objPtrCenter->setPosition(m_fInitX - m_objPtrCenter->getWidth() / 2, m_fInitY - m_objPtrCenter->getHeight() / 2);
        });
        dispatchEvent(END);
    }
}
