
#include <iostream>
#include "SpinnerScene.h"

USING_NS_CC;

Scene* Spinner::createScene()
{
    return Spinner::create();
}

// on "init" you need to initialize your instance
bool Spinner::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
    /*Beginning of Initialization*/
    wheel = Sprite::create("wheel_sections_8.png");
    auto item0 =Sprite::create("heart.png");
    auto item1 = Sprite::create("brush.png");
    auto item2 = Sprite::create("gem.png");
    auto item3 = Sprite::create("hammer.png");
    auto item4 = Sprite::create("coin.png");
    auto item5 = Sprite::create("brush.png");
    auto item6 = Sprite::create("gem.png");
    auto item7 =  Sprite::create("hammer.png");
    auto wheelBorder = Sprite::create("wheel_border.png");
    auto wheelArrow = Sprite::create("wheel_arrow.png");
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    items =new Sprite*[8] {item0,item1,item2,item3,item4,item5,item6,item7};
    const char* description[] = {"30min","x3","x35","x3","x750","x1","x75","x1"};
    spinButton = MenuItemImage::create("spin_button.png","spin_button.png",CC_CALLBACK_1(Spinner::rotate, this));

    /*End of Initialization*/
    wheelArrow->setPositionNormalized(Vec2(0.5,0.96));
    wheelBorder->setPosition(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);



    auto wheelSize = wheel->getContentSize();

    wheel->setPosition(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y);

    this->addChild(wheel,1);
    this->addChild(wheelBorder,2);
    wheelBorder->addChild(wheelArrow,1);

    positionItems(description);
    //scale to fit the screen size
    wheel->setScale(0.75);
    wheelBorder->setScale(0.75);


    auto spinLabel = Label::createWithTTF("Play On", "fonts/arial.ttf", 24);

    spinButton->addChild(spinLabel);
    spinLabel -> setPositionNormalized(Vec2(0.5,0.5));
        float x = origin.x + visibleSize.width - spinButton->getContentSize().width/2;
        float y = origin.y + spinButton->getContentSize().height/2;
        spinButton->setPosition(Vec2(x,y));


    // create menu, it's an autorelease object
    auto menu = Menu::create(spinButton, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////


    wheelComponents = Node::create();
    wheelComponents->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));


    return true;
}

void Spinner::positionItems( const char* description[]){
    auto wheelSize = wheel->getContentSize();
    auto distanceFromTheOrigin = Vec2(wheelSize.width/2,6*wheelSize.height/7);
    auto wheelOrigin = Vec2(wheelSize.width/2,wheelSize.height/2);
    for (int i = 0; i<8;++i){
        this->items[i]->setPosition(distanceFromTheOrigin.rotateByAngle(wheelOrigin,CC_DEGREES_TO_RADIANS(-22.5-(45*i))));
        items[i]->setRotation(22.5+(i*45));
        items[i]->setScale(0.75);
        wheel->addChild(items[i],1);
        auto itemDescription = Label::create();
        itemDescription->setTextColor(Color4B::BLACK);
        itemDescription->setString(description[i]);
        itemDescription->setPositionNormalized(Vec2(1,0));
        itemDescription->setBMFontSize(25);
        items[i]->addChild(itemDescription);
    }
}

void Spinner::rotate(Ref* pSender){
    float fff = wheel->getRotation();
    float fffff = fmod(wheel->getRotation(),360);
    int prize = randomPrizeSelector();
    log("Sector %d",prize+1);
    float prizeLocation = 10+22.5+(7-prize)*45; //offset by 10 for the ending animation, 22.5 to locate the arrow in the center, 360/8 = 45, 7-i because it spins backwards,
    int additionalRotate = fmod(fffff+360-prizeLocation,360)>180?0:180; //offset to make sure the wheel does not spin backward

    auto preRotate = RotateBy::create(0.3f,-20.0); //pull by 20 degrees before spinning
    auto rotate = RotateBy::create(1.5f,20+360*4+additionalRotate); //spin 4 times + the 20 degrees I pulled
    auto findLocation = RotateTo::create(0.3f,prizeLocation); //rotateTo instead of rotateBy since it needs to end in a specific angle
    auto reverse = RotateBy::create(0.2f,-10.0);  //pull 10 degrees to make the stop look smoother
    auto result = CallFunc::create([this,prize](){Spinner::showResult(prize);});
    auto disable = CallFunc::create([this](){this->spinButton->setEnabled(false);this->spinButton->setVisible(false);}); //hide and disable button when it's in action
    auto enable = CallFunc::create([this](){this->spinButton->setEnabled(true);this->spinButton->setVisible(true);}); // enable it after the action so the user can spin again


    auto seq = Sequence::create(disable,preRotate,rotate,findLocation,reverse,result,enable, nullptr);


    wheel->runAction(seq);
    randomPrizeSelectorTest(1000); //generates 1000 results to check if the probability assignment is correct (unit testing)


    return;
}

void Spinner::showResult(int prize){
    log(prize);
    auto scale = ScaleBy::create(1,1.3);
    auto scaleSequence = Sequence::create(scale, scale->reverse(),nullptr);
    this->items[prize]->runAction(scaleSequence);
    return;

}

void Spinner::randomPrizeSelectorTest(int repeat){
    int recorder[8] = {0,0,0,0,0,0,0,0};
    int prize;
    for(int i=0;i<repeat;++i){
        prize = randomPrizeSelector();
        recorder[prize]+=1;

    }
    for (int i=0;i<8;++i){
        log("Sector %d: %d occurances.",i+1,recorder[i]);
    }
}

int Spinner::randomPrizeSelector(){
    int choice = cocos2d::RandomHelper::random_int(1,100);
    int chance []= {20,10,10,10,5,20,5,20};
    int index = 0;
    while(choice>chance[index]){
        choice-=chance[index];
        ++index;
    }
    return index;

}
