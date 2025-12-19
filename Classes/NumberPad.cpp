
#include "NumberPad.h"

USING_NS_CC;

NumberPad* NumberPad::create()
{
    NumberPad *ret = new (std::nothrow) NumberPad();
    if (ret && ret->init())
    {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool NumberPad::init()
{
    if (!Node::init()) {
        return false;
    }

    auto menu = Menu::create();
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);
    float buttonSize = 90.0f;
    float padding = 22.0f;

    // Create number buttons 1-9 in a 3x3 grid
    for (int i = 0; i < 9; ++i) {
        int num = i + 1;
        Vec2 position(
            (i % 3) * (buttonSize + padding),
            -(i / 3) * (buttonSize + padding)
        );
        createButton(std::to_string(num), num, position, menu);
    }

    return true;
}

void NumberPad::createButton(const std::string& title, int tag, const Vec2& position, Menu* menu)
{
    auto label = Label::createWithTTF(title, "fonts/arial.ttf", 56);
    auto button = MenuItemLabel::create(label, [this, tag](Ref* sender){
        if (onButtonPressed) {
            onButtonPressed(tag);
        }
    });
    button->setPosition(position);
    menu->addChild(button);
}
