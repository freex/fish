#include "MainMenuLayer.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

// android effect only support ogg
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#define BG_MUSIC        "Audio/music_1.ogg"
#else
#define BG_MUSIC        "Audio/music_1.mp3"
#endif // CC_PLATFOR_ANDROID
Scene* MainMenuLayer::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
	//这个居然是个宏！！！调用了init方法。你看看那个宏就知道啦
    auto layer = MainMenuLayer::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool MainMenuLayer::init()
{
	//Get the windows size
	auto winSize = Director::getInstance()->getWinSize();
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Point origin = Director::getInstance()->getVisibleOrigin();

	//设置背景图片
	auto ui_background = Sprite::create("MainMenu/ui_background_normal-hd.png");
	ui_background->setAnchorPoint(ccp(0.0f,0.0f));
	ui_background->setPosition(Point(0.0f,0.0f));
	addChild(ui_background,0.0f);

	//Set game logo
	auto ui_Logo = Sprite::create("MainMenu/main_ui_title_cn-hd.png");
	ui_Logo->setPosition(Point(winSize.width/2.0f, winSize.height/1.35f));
	addChild(ui_Logo,2);

	auto paopao_left = ParticleSystemQuad::create("MainMenu/lizhi_qipao.plist");
	paopao_left->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
	paopao_left->setPosition(Point(10, 0));
	addChild(paopao_left, 10);

	
	auto paopao_right = ParticleSystemQuad::create("MainMenu/lizhi_qipao.plist");
	paopao_right->setPositionType(ParticleSystemQuad::PositionType::GROUPED);
	paopao_right->setPosition(Point(visibleSize.width - 10, 0));
	addChild(paopao_right, 10);

	
	//Read the texture to sprite frame cache
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MainMenu/UI_GameMenuText_cn-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MainMenu/UI_GameStartMenuLayer-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MainMenu/FishActor-Small-hd.plist");
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("MainMenu/FishActor-Marlin-hd.plist");

	
	//Create the start button of the game
	auto startGameBtn = Sprite::createWithSpriteFrameName("ui_button_box02_02.png");
	auto startGameBtnPush = Sprite::createWithSpriteFrameName("ui_button_box02_01.png");
	auto startGameFont = Sprite::createWithSpriteFrameName("ui_2p_010.png");
	
	//Create the scene choose button
	auto sceneChooseBtn = Sprite::createWithSpriteFrameName("ui_button_box01_02.png");
	auto sceneChooseBtnPush = Sprite::createWithSpriteFrameName("ui_button_box01_01.png");
	auto sceneChooseFont = Sprite::createWithSpriteFrameName("button_other_014.png");
	
	//Create the menu
	auto startGameMenuItem = MenuItemSprite::create(startGameBtn, startGameBtnPush, CC_CALLBACK_1(MainMenuLayer::startGameEvent, this));
	auto sceneChooseMenuItem = MenuItemSprite::create(sceneChooseBtn, sceneChooseBtnPush, CC_CALLBACK_1(MainMenuLayer::sceneChoose, this));
	sceneChooseMenuItem->setPosition(Point(startGameMenuItem->getPosition().x, startGameMenuItem->getPosition().y-140));
	auto startGameMenu = Menu::create(startGameMenuItem, sceneChooseMenuItem, NULL);
	
	//Set the posiiton of menu
	startGameMenu->setPosition(Point(ui_Logo->getPosition().x, ui_Logo->getPosition().y-230));
	startGameFont->setPosition(Point(ui_Logo->getPosition().x, ui_Logo->getPosition().y-220));
	sceneChooseFont->setPosition(Point(ui_Logo->getPosition().x, ui_Logo->getPosition().y-370));
	
	//Add the menu into the scene
	addChild(startGameMenu,2);
	addChild(startGameFont,3);
	addChild(sceneChooseFont,3);

	//创建一个精灵


	auto fishe_sprite = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("SmallFish_actor_001.png"));
	//auto fishe_sprite = fishes[0];
	//得到显示内容(鱼)的范围
	Size size=fishe_sprite->getContentSize();
	fishe_sprite->setPosition(Point(visibleSize.width-size.width,visibleSize.height-visibleSize.height/6));
	addChild(fishe_sprite, 1);

	//创建两个精灵
	auto fishe_sprite2 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("SmallFish_actor_002.png"));
	//得到显示内容的范围
	fishe_sprite2->setPosition(Point(visibleSize.width-size.width,visibleSize.height-visibleSize.height/2));
	addChild(fishe_sprite2, 1);

	//创建三个精灵
	auto fishe_sprite3 = Sprite::createWithSpriteFrame(SpriteFrameCache::getInstance()->getSpriteFrameByName("AngelFish_death_005.png"));
	//得到显示内容的范围
	fishe_sprite3->setPosition(Point(visibleSize.width-visibleSize.width/2, size.height));
	addChild(fishe_sprite3, 1);
	turnBackV(fishe_sprite3);
		
	// 之前的移动
	// auto moveAction1 = MoveTo::create(10,Point(size.width,fishe_sprite->getPositionY()));
	// auto moveAction2 = MoveTo::create(5,Point(visibleSize.width - size.width,fishe_sprite->getPositionY()));
	//auto seq = Sequence::create(moveAction1,moveAction2,NULL);

	auto moveAction = RepeatForever::create(
		Sequence::create(
		MoveTo::create(12, 
			Point(size.width,fishe_sprite->getPositionY())),
			CallFunc::create(CC_CALLBACK_0(MainMenuLayer::turnBack,this,fishe_sprite)),
		MoveTo::create(8, 
			Point(visibleSize.width - size.width,fishe_sprite->getPositionY())),
			CallFunc::create(CC_CALLBACK_0(MainMenuLayer::turnBack,this,fishe_sprite)), NULL));

	auto moveAction3 = RepeatForever::create(
		Sequence::create(
		MoveTo::create(5, 
		Point(fishe_sprite3->getPositionX(),visibleSize.height - size.height)),
			CallFunc::create(CC_CALLBACK_0(MainMenuLayer::turnBack,this,fishe_sprite3)),
		MoveTo::create(4, 
			Point(fishe_sprite3->getPositionX(), size.height)),
			CallFunc::create(CC_CALLBACK_0(MainMenuLayer::turnBack,this,fishe_sprite3)), NULL));

	auto moveAction2 = RepeatForever::create(
		Sequence::create(
		MoveTo::create(5, 
			Point(size.width,fishe_sprite2->getPositionY())),
			CallFunc::create(CC_CALLBACK_0(MainMenuLayer::turnBack,this,fishe_sprite2)),
		MoveTo::create(4, 
			Point(visibleSize.width - size.width,fishe_sprite2->getPositionY())),
			CallFunc::create(CC_CALLBACK_0(MainMenuLayer::turnBack,this,fishe_sprite2)), NULL));

	//((SmallFishActor*)moveAction)->activateFishMovement();
	auto fishes = Vector<SpriteFrame*>();//动态数组容器
	fishes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("SmallFish_actor_001.png"));
	fishes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("SmallFish_actor_002.png"));
	fishes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("SmallFish_actor_003.png"));
	fishes.pushBack(SpriteFrameCache::getInstance()->getSpriteFrameByName("SmallFish_actor_004.png"));

	//Create swimming animation
	auto fishAnimation = Animation::createWithSpriteFrames(fishes,0.1);
	auto fishAnimate = Animate::create(fishAnimation);

	fishe_sprite->runAction(moveAction);
	fishe_sprite->runAction(RepeatForever::create(fishAnimate));
	fishe_sprite2->runAction(moveAction2);
	//fishe_sprite2->runAction(RepeatForever::create(fishAnimate));
	fishe_sprite3->runAction(moveAction3);

	//设置音乐背景
    CocosDenshion::SimpleAudioEngine::getInstance()->preloadBackgroundMusic(BG_MUSIC);
    CocosDenshion::SimpleAudioEngine::getInstance()->setBackgroundMusicVolume(0.5);
	CocosDenshion::SimpleAudioEngine::getInstance()->playBackgroundMusic(BG_MUSIC, true);
    
    return true;
}

void MainMenuLayer::turnBack(Node* sender){
	
	if(sender->getRotation()==0.0f){
		
		sender->setRotation(180.00f);
	}else {
		
		sender->setRotation(0.00f);
	}
}

void MainMenuLayer::turnBackV(Node* sender){
	
	if(sender->getRotation()==0.0f){
		
		sender->setRotation(90.00f);
	}else {
		
		sender->setRotation(180.00f);
	}
}

void MainMenuLayer::startGameEvent(Ref* sender){

	//Create the scene of FishingScene with a transtionFadeBL effect
	auto replaceScene = TransitionFadeBL::create(2, FishingScene::create());
	
	//Replace the scene with the trasition effect scene
	Director::getInstance()->replaceScene(replaceScene);
	CCLOG("start");
}

void MainMenuLayer::sceneChoose(Ref* sender){
	CCLOG("choose");
		
}


//void MainMenuLayer::fishActorsInital(){
//	
//	//Get the window size
//	auto winSize = Director::getInstance()->getWinSize();
//	
//	//Create fishes
//	for (int fishIndex=0; fishIndex<7; fishIndex++) {//同种类鱼的条数
//
//		auto smallFishActor = FishActor::createWithType(FishActor::FishActorType::SmallFish);
//		auto angelFishActor = FishActor::createWithType(FishActor::FishActorType::AngelFish);
//		auto croakerFishActor = FishActor::createWithType(FishActor::FishActorType::Croaker);
//		auto amphiprionFishActor = FishActor::createWithType(FishActor::FishActorType::Bream);
//		auto breamFishActor = FishActor::createWithType(FishActor::FishActorType::SmallFish);
//		
//		//Set the position of the fishes like a matrix
//		smallFishActor->setPosition(Point(2000-winSize.width/10*(fishIndex+1),winSize.height-winSize.height/6));
//		angelFishActor->setPosition(Point(2000-winSize.width/10*(fishIndex+1),winSize.height-winSize.height/6*2));
//		croakerFishActor->setPosition(Point(2000-winSize.width/10*(fishIndex+1),winSize.height-winSize.height/6*3));
//		amphiprionFishActor->setPosition(Point(2000-winSize.width/10*(fishIndex+1),winSize.height-winSize.height/6*4));
//		breamFishActor->setPosition(Point(2000-winSize.width/10*(fishIndex+1),winSize.height-winSize.height/6*5));
//		
//		smallFishActor->runAction(createFishMoveAction(smallFishActor));
//		angelFishActor->runAction(createFishMoveAction(angelFishActor));
//		croakerFishActor->runAction(createFishMoveAction(croakerFishActor));
//		amphiprionFishActor->runAction(createFishMoveAction(amphiprionFishActor));
//		breamFishActor->runAction(createFishMoveAction(breamFishActor));
//
//		
//		//Add the fishes into the scene
//		addChild(smallFishActor,1);
//		addChild(angelFishActor,1);
//		addChild(croakerFishActor,1);
//		addChild(amphiprionFishActor,1);
//		addChild(breamFishActor,1);
//	}
//	
//	auto marlin = FishActor::createWithType(FishActor::FishActorType::MarlinsFish);
//	marlin->setVisible(false);
//	marlin->setPosition(Point(1000, winSize.height/2));
//	marlin->runAction(MainMenuLayer::createMarlinMoveAction((MarlinsFishActor*)marlin));
//	addChild(marlin, 1);
//}


void MainMenuLayer::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
