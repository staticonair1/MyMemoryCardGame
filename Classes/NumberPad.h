
#ifndef __NUMBER_PAD_H__
#define __NUMBER_PAD_H__

#include "cocos2d.h"
#include <functional>

class NumberPad : public cocos2d::Node
{
public:
    static NumberPad* create();
    virtual bool init() override;

    // A callback function for when a button is pressed.
    // The integer argument is the number (1-9), or 0 for delete.
    std::function<void(int)> onButtonPressed;

private:
    void createButton(const std::string& title, int tag, const cocos2d::Vec2& position, cocos2d::Menu* menu);
};

#endif // __NUMBER_PAD_H__
