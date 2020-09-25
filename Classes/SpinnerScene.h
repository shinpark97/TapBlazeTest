

#ifndef __SPINNER_SCENE_H__
#define __SPINNER_SCENE_H__

#include <cocos/scripting/deprecated/CCDeprecated.h>
#include "cocos2d.h"

class Spinner : public cocos2d::Scene
{
    cocos2d::Node* wheel;
    cocos2d::Node* wheelComponents;
    cocos2d::Sprite** items;
    cocos2d::MenuItemImage* spinButton;
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    


    void rotate(Ref* pSender);
    void positionItems(const char* description[]);
    int randomPrizeSelector();
    void randomPrizeSelectorTest(int repeat);
    void showResult(int prize);
    

    CREATE_FUNC(Spinner);
};

#endif
