#ifndef FishJoyMini_MainMenuLayer_h
#define FishJoyMini_MainMenuLayer_h

#include "MainMenuScene.h"
#include "FishActor.h"
#include "FishingScene.h"


class MainMenuLayer : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();

    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();

	virtual void turnBack(Node* sender);
	virtual void turnBackV(Node* sender);
    	/** Callback func when touch the startgame button */
	void startGameEvent(Ref* sender);
	
	/** Callback func when touch the select scne button */
	void sceneChoose(Ref* sender);
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(MainMenuLayer);
};

#endif // FishJoyMini_MainMenuLayer_h
