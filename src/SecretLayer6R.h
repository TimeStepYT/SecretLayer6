#pragma once
#include <Geode/Geode.hpp>
#include "SecretGame01LayerR.h"

#ifdef GEODE_IS_WINDOWS
class SecretLayer6R : public cocos2d::CCLayer {
public:
	static cocos2d::CCScene* scene();
	static SecretLayer6R* create();

	bool init();
	void startGame01();
	void keyBackClicked(); 
	void onBack(cocos2d::CCObject* sender);

	SecretGame01LayerR* m_gameLayer;
};
#else
#include <Geode/modify/SecretLayer6.hpp>
class $modify(MySecretLayer6, SecretLayer6) {
	void onBack(CCObject*);
};
#endif