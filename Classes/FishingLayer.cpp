//
//  FishingLayer.cpp
//  FishJoyMini
//
//  Created by Ken on 14-3-11.
//
//

#include "FishingLayer.h"

// android effect only support ogg
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define SOUND_COIN        "Audio/sound_coin.ogg"
#else
#define SOUND_COIN        "Audio/sound_coin.mp3"
#endif // CC_PLATFOR_ANDROID

// android effect only support ogg
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define SOUND_FIRE        "Audio/sound_fire.ogg"
#else
#define SOUND_FIRE        "Audio/sound_fire.mp3"
#endif // CC_PLATFOR_ANDROID

bool FishingLayer::init(){
    
    musicTurnOff=true;
	Layer::init();
	_bullet=NULL;
    //Set tag of the layer, and get it in pause layer
    setTag(101);
	
	//Add background picture
	auto background = Sprite::create("CocoStudioRes/background.jpg");
	background->setAnchorPoint(Point(0,0));
	background->setPosition(Point(0,0));
	background->setTag(102);
	addChild(background,0);

	//Read the resources of CocoStudio json file, and add it to the scene

	widget = dynamic_cast<Layout*>(cocostudio::GUIReader::getInstance()->widgetFromJsonFile("CocoStudioRes/FishJoyMini_1.json"));
	addChild(widget,2);

	//Read the texture to sprite frame cache
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("CocoStudioRes/cannon-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("GameScene/Item-chaojiwuqi-iphone-hd.plist");
	
	//Get the instantce of ui_box_01_01 and make it rotate forever
	//让那个齿轮使劲不停转啊转
	auto ui_box_01_01 = dynamic_cast<ImageView*>(widget->getChildByName("ui_box_01_01"));
	ui_box_01_01->runAction(RepeatForever::create((RotateBy::create(3, 360, 360))));
	
	//Get the instance of label of score
	scoreLabel = dynamic_cast<TextAtlas*>(widget->getChildByName("Score"));
	score = 0;

	//Preload background effect
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(SOUND_COIN);
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(SOUND_FIRE);

	
	//Get the instance of cannon
	cannon = dynamic_cast<ImageView*>(widget->getChildByName("cannon"));
	
	//Set the touch event enable
	widget->setTouchEnabled(true);
	
	//Add the touch event callback function
	//在widget上点击，表明是在抓鱼啊！！！
	widget->addTouchEventListener(this, toucheventselector(FishingLayer::shootEvent));
	
	//Get the pause buton and its touch event callback function
	auto pauseBtn = dynamic_cast<Button*>(widget->getChildByName("pauseBtn"));
	pauseBtn->addTouchEventListener(this, toucheventselector(FishingLayer::pauseEvent));
    
	////turn on the background music
	//auto turnOnMusicBtn= dynamic_cast<ImageView *>(widget->getChildByName("ImageView_42"));
	//turnOnMusicBtn->addTouchEventListener(this,toucheventselector(FishingLayer::turnOnMusic));
	////turn off the background music
	//auto turnOffMusicBtn= dynamic_cast<Button *>(widget->getChildByName("music"));
	//turnOffMusicBtn->addTouchEventListener(this,toucheventselector(FishingLayer::turnOffMusic));
	
	return true;
}

void FishingLayer::shootEvent(Widget* target,TouchEventType type){
	if(type == TouchEventType::TOUCH_EVENT_BEGAN){
        
		//Change the texture of the cannot at the beginning of touch to make it more real
		cannon->loadTexture("actor_cannon1_72.png", UI_TEX_TYPE_PLIST);
		
	}else if(type == TouchEventType::TOUCH_EVENT_ENDED){
		
		//Change back the texture of the cannon at the end of the touch event
		cannon->loadTexture("actor_cannon1_71.png", UI_TEX_TYPE_PLIST);
		
		//Change the cannon rotation
		//让大炮旋转起来！！
		FishingLayer::setCannonRotation(target,target->getTouchEndPos());
		
		//Shoot the bullet
		//射出子弹！
		bulletEndPosition=target->getTouchEndPos();
		bulletShoot(target->getTouchEndPos());
		
		
	}else if(type == TouchEventType::TOUCH_EVENT_MOVED){
        
		//Change the cannon ratation
		FishingLayer::setCannonRotation(target, target->getTouchMovePos());
	}
}

//射击射击
void FishingLayer::bulletShoot(Point endPosition){
	
	if(_bullet==NULL){
		
		//Init the bullet
		auto bullet = Sprite::createWithSpriteFrameName("weapon_bullet_007.png");
		auto netFish=SpriteBatchNode::create("GameScene/bullet10-hd.png",5);
		//addChild(netFish,1);
		
        net=Sprite::createWithTexture(netFish->getTexture(),Rect(0,0,80,80));
        net1=Sprite::createWithTexture(netFish->getTexture(),Rect(0,0,80,80));
        net2=Sprite::createWithTexture(netFish->getTexture(),Rect(0,0,80,80));
        net3=Sprite::createWithTexture(netFish->getTexture(),Rect(0,0,80,80));
        
        net1->setRotation(90.0f);
        net2->setRotation(180.0f);
        net3->setRotation(270.0f);
		
		float shifting;
        
		//Set the offest of the raotation
		if(cannon->getRotation()<=0){
			
			shifting = 20.0f;
		}else{
			
			shifting = -20.0f;
		}
		
		//Set the anchorpoint, rotation, position of the bullet
		_bullet = bullet;
		_bullet->setAnchorPoint(Point(0.5, 0.5));
		_bullet->setRotation(cannon->getRotation());
		_bullet->setPosition(Point(cannon->getPosition().x-shifting, cannon->getPosition().y+20));
        
		auto scale0=ScaleTo::create(0.5,0.3);
		auto scale1=ScaleTo::create(0.5,0.1);
		auto scale2=ScaleTo::create(0.5,0.35);
		auto scale3=ScaleTo::create(0.5,0.15);
		auto scale4=ScaleTo::create(0.1,0);
		auto scale00=ScaleTo::create(0.5,0.3);
		auto scale01=ScaleTo::create(0.5,0.1);
		auto scale02=ScaleTo::create(0.5,0.35);
		auto scale03=ScaleTo::create(0.5,0.15);
		auto scale04=ScaleTo::create(0.1,0);
		auto scale000=ScaleTo::create(0.5,0.3);
		auto scale001=ScaleTo::create(0.5,0.1);
		auto scale002=ScaleTo::create(0.5,0.35);
		auto scale003=ScaleTo::create(0.5,0.15);
		auto scale004=ScaleTo::create(0.1,0);
		auto scale0000=ScaleTo::create(0.5,0.3);
		auto scale0001=ScaleTo::create(0.5,0.1);
		auto scale0002=ScaleTo::create(0.5,0.35);
		auto scale0003=ScaleTo::create(0.5,0.15);
		auto scale0004=ScaleTo::create(0.1,0);
		net->setAnchorPoint(Point(0, 0));
		net1->setAnchorPoint(Point(0, 0));
		net2->setAnchorPoint(Point(0, 0));
		net3->setAnchorPoint(Point(0, 0));
		
		//Shoot the bullet and release after the action ended
		_bullet->runAction(Sequence::create(MoveTo::create(1, endPosition),
											CallFunc::create(CC_CALLBACK_0(FishingLayer::bulletRelease,this)),
											NULL));
		addChild(_bullet,1);
		net->runAction(Sequence::create(scale0,scale1,scale2,scale3,scale4,NULL));
		net1->runAction(Sequence::create(scale00,scale01,scale02,scale03,scale04,NULL));
		net2->runAction(Sequence::create(scale000,scale001,scale002,scale003,scale004,NULL));
		net3->runAction(Sequence::create(scale0000,scale0001,scale0002,scale0003,scale0004,NULL));
		
		//Play bullet shoot music effect
		CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(SOUND_FIRE);
	}
}

void FishingLayer::bulletRelease(){
    
	//Release the bullet, and set the _bullet to NULL
	if(_bullet!=NULL){
		_bullet->removeFromParent();
		_bullet = NULL;
	}
	net->setPosition(bulletEndPosition);
	net1->setPosition(bulletEndPosition);
	net2->setPosition(bulletEndPosition);
	net3->setPosition(bulletEndPosition);
	addChild(net,1);
	addChild(net1,1);
	addChild(net2,1);
	addChild(net3,1);
}

void FishingLayer::pauseEvent(Widget* target, TouchEventType type){
    
	if(type == TouchEventType::TOUCH_EVENT_ENDED){
		
		//Get the windows size of fishlayer
		auto winSize = Director::getInstance()->getWinSize();
        
		//Pause all the actions and animations
        this->onExit();
        
		//Get the background ant change it to the pause texture
		auto background = (Sprite*)getChildByTag(102);
		background->setTexture("GameScene/bgblur01_01-hd.png");
		background->setScaleX(winSize.width/background->getContentSize().width);
		background->setScaleY(winSize.height/background->getContentSize().height);
		background->setZOrder(2);
        
		//Create the pause layer
		auto pauseLayer = FishingPauseLayer::create();
		this->getParent()->addChild(pauseLayer,3);
	}
}

//设置大炮旋转
void FishingLayer::setCannonRotation(Widget* target, Point targetPos){
	
	//Calulatethe rotation of cannon
	auto radian=atan2(targetPos.y-21.6f ,targetPos.x-480.0f);
	auto inclination= radian*180/3.14;
	auto rotation= -(inclination)+90;
	
	//Set the rotation range
	if(rotation<=70 && rotation>=-70){
		
		cannon->setRotation(rotation);
	}
}

