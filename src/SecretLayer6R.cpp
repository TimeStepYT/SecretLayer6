#include "SecretLayer6R.h"
#include <Geode/binding/SecretLayer5.hpp>
using namespace geode::prelude;

#ifndef NEEDS_DECOMP
void MySecretLayer6::onBack(CCObject* sender) {
#else
void SecretLayer6R::onBack(CCObject* sender) {
#endif
	auto director = CCDirector::sharedDirector();
	auto gameManager = GameManager::get();

	gameManager->fadeInMusic("secretLoop04.ogg");
	director->popSceneWithTransition(0.5f, PopTransition::kPopTransitionFade);
}

#ifdef NEEDS_DECOMP
CCScene* SecretLayer6R::scene() {
	CCScene* scene = CCScene::create();
	scene->retain();
	// AppDelegate::get()->m_runningScene = scene;
	SecretLayer6R* secretLayer = SecretLayer6R::create();
	scene->addChild(secretLayer);
	return scene;
}

SecretLayer6R* SecretLayer6R::create() {
	auto res = new SecretLayer6R();
	if (res && res->init()) {
		res->autorelease();
		return res;
	}
	delete res;
	return nullptr;
}

void SecretLayer6R::startGame01() {
	if (this->m_gameLayer) this->m_gameLayer->resetGame();
}

bool SecretLayer6R::init() {
	if (!CCLayer::init()) return false;

	CCTextureCache* textureCache = CCTextureCache::sharedTextureCache();
	CCSpriteFrameCache* frameCache = CCSpriteFrameCache::sharedSpriteFrameCache();
	GameManager* gameManager = GameManager::sharedState();
	CCSprite* background = CCSprite::create("GJ_gradientBG.png");
	CCLabelBMFont* title = CCLabelBMFont::create("...", "goldFont.fnt");

	textureCache->addImage("SecretSheet.png", false);
	frameCache->addSpriteFramesWithFile("SecretSheet.plist");
	gameManager->fadeInMusic("secretLoop03.mp3");

	this->setKeypadEnabled(true);
	this->setKeyboardEnabled(true);

	CCDirector* director = CCDirector::sharedDirector();
	CCSize winSize = director->getWinSize();

	auto point = ccp(0, 0);
	background->setAnchorPoint(point);
	this->addChild(background);
	background->setScaleX((winSize.width + 10.f) / background->getContentWidth());
	background->setScaleY((winSize.height + 10.f) / background->getContentHeight());
	point = ccp(-5, -5);
	background->setPosition(point);
	background->setColor(ccc3(30, 0, 60));
	title->setScale(1.0f);
	this->addChild(title);
	point = ccp(winSize.width * 0.5f, winSize.height - 24.f);
	title->setPosition(point);
	SecretGame01LayerR* gameLayer = SecretGame01LayerR::create();
	this->m_gameLayer = gameLayer;
	this->addChild(gameLayer);

	CCSprite* backArrow = CCSprite::createWithSpriteFrameName("GJ_arrow_01_001.png");
	CCMenuItemSpriteExtra* backArrowNode = CCMenuItemSpriteExtra::create(backArrow, nullptr, this, menu_selector(SecretLayer6R::onBack));

	backArrowNode->setSizeMult(1.5f);
	auto menu = CCMenu::create(backArrowNode, 0);
	this->addChild(menu);
	director = CCDirector::sharedDirector();
	float left = director->getScreenLeft();
	director = CCDirector::sharedDirector();
	float top = director->getScreenTop();
	point = ccp(left + 24.f, top - 23.f);
	menu->setPosition(point);

	if (PlatformToolbox::isControllerConnected())
		GameToolbox::addBackButton(this, backArrowNode);

	auto delay = CCDelayTime::create(0.5f);
	auto startGame = CCCallFunc::create(this, callfunc_selector(SecretLayer6R::startGame01));
	CCSequence* sequence = CCSequence::create(delay, startGame, nullptr);
	this->runAction(sequence);
	return true;
}

void SecretLayer6R::keyBackClicked() {
	this->onBack(this);
}
#endif