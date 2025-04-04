#include "SecretGame01LayerR.h"
#include "SecretLayer6R.h"

using namespace geode::prelude;

#ifdef NEEDS_DECOMP

SecretGame01LayerR* SecretGame01LayerR::create() {
	auto res = new SecretGame01LayerR();
	if (res && res->init()) {
		res->autorelease();
		return res;
	}
	delete res;
	return nullptr;
}

bool SecretGame01LayerR::init() {
	if (!CCLayer::init()) return false;

	if (Mod::get()->getSettingValue<bool>("fixDifficultyFrame")) {
		// Thank you for you help, dank!
		GameManager::get()->loadIcon(1, (int) IconType::Cube, -1);
		GameManager::get()->loadIcon(4, (int) IconType::Cube, -1);
		GameManager::get()->loadIcon(12, (int) IconType::Cube, -1);
		GameManager::get()->loadIcon(73, (int) IconType::Cube, -1);
	}
	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	auto menu = CCMenu::create();
	this->m_menu = menu;
	this->addChild(menu);
	menu->setTouchEnabled(false);

	auto icons = CCArray::create();
	this->m_icons = icons;
	icons->retain();

	auto correctIcons = CCArray::create();
	this->m_correctIcons = correctIcons;
	correctIcons->retain();

	auto timeBar = CCSprite::createWithSpriteFrameName("whiteSquare20_001.png");
	this->m_timeBar = timeBar;
	float xOffset = (winSize.width * 0.5f) - 50;
	timeBar->setPosition(CCPoint(xOffset, 40.f));
	timeBar->setAnchorPoint(CCPoint(0.f, 0.5f));
	timeBar->setScaleY(0.6f);
	timeBar->setColor({ 100, 100, 100 });
	timeBar->setOpacity(100);
	timeBar->setScaleX(100.f / timeBar->getContentWidth());
	this->addChild(timeBar);
	timeBar->setVisible(false);

	this->scheduleUpdate();

	return true;
}

int SecretGame01LayerR::getRowsForDifficulty(int difficulty) {
	int rows = 3;
	int rowsArray[13] = { 2, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 5 };

	if (difficulty < 13)
		rows = rowsArray[difficulty];

	return rows;
}

unsigned int SecretGame01LayerR::getTimeForDifficulty(int difficulty) {
	unsigned int timesForDifficulty[] = { 3, 5, 6, 7, 5, 6, 8, 10, 5, 6, 8, 10, 12, 0, 0, 0 };
	unsigned int time = 10;

	if (difficulty < 13) time = timesForDifficulty[difficulty];

	return time;
}

unsigned int SecretGame01LayerR::getCountForDifficulty(int difficulty) {
	unsigned int count = 2;
	unsigned int difficultyLess = difficulty - 1;

	unsigned int countsForDifficulty[]{ 3, 4, 5, 3, 4, 5, 6, 3, 4, 5, 6, 7 };

	if (difficultyLess < 12) count = countsForDifficulty[difficultyLess];

	return count;
}

void SecretGame01LayerR::gameStep01() {
	int countForDifficulty = this->getCountForDifficulty(this->m_difficulty);
	auto wrongIcons = CCArray::create();
	wrongIcons->addObjectsFromArray(this->m_icons);
	this->m_correctIcons->removeAllObjects();
	if (0 < countForDifficulty) {
		int count = 0;
		do {
			int random = rand();
			count = count + 1;
			unsigned int i = random % wrongIcons->count();
			CCObject* randomIcon = wrongIcons->objectAtIndex(i);
			wrongIcons->removeObjectAtIndex(i, true);
			this->m_correctIcons->addObject(randomIcon);
		} while (count != countForDifficulty);
	}
	float delayTime = 0.f;
	int arraySize = this->m_correctIcons->count();
	for (size_t i = 0; i < arraySize; i = i + 1) {
		auto object = static_cast<CCMenuItemSpriteExtra*>(this->m_correctIcons->objectAtIndex(i));

		auto normalImage = object->getNormalImage();
		auto normalImageChildren = normalImage->getChildren();
		auto firstObject = normalImageChildren->objectAtIndex(0);

		_ccColor3B tintToColor;
		delayTime = i * 0.2f;

		auto delay = CCDelayTime::create(delayTime);
		tintToColor = ccc3(0, 255, 0);
		auto tintTo = CCTintTo::create(0.1f, tintToColor.r, tintToColor.g, tintToColor.b);
		auto delay2 = CCDelayTime::create(0.1);
		tintToColor = ccc3(75, 75, 75);
		auto tintTo2 = CCTintTo::create(0.2f, tintToColor.r, tintToColor.g, tintToColor.b);
		auto sequence = CCSequence::create(delay, tintTo, delay2, tintTo2, nullptr);
		normalImage->runAction(sequence);

		delay = CCDelayTime::create(delayTime);
		tintToColor = ccc3(0, 255, 0);
		tintTo = CCTintTo::create(0.1f, tintToColor.r, tintToColor.g, tintToColor.b);
		delay2 = CCDelayTime::create(0.1);
		tintToColor = ccc3(60, 60, 60);
		tintTo2 = CCTintTo::create(0.2f, tintToColor.r, tintToColor.g, tintToColor.b);
		sequence = CCSequence::create((CCFiniteTimeAction*) delay, tintTo, delay2, tintTo2, nullptr);
		static_cast<CCMenuItemSpriteExtra*>(firstObject)->runAction(sequence);

		delayTime = delayTime + 0.5f;
	}
	auto delay = CCDelayTime::create(delayTime);
	auto callfunc = CCCallFunc::create(this, callfunc_selector(SecretGame01LayerR::gameStep02));
	auto sequence = CCSequence::create(delay, callfunc, nullptr);
	this->runAction(sequence);
}

void SecretGame01LayerR::gameStep02() {
	this->m_menu->setTouchEnabled(true);
	if (this->m_icons) {
		CCObject* iconObject;
		CCARRAY_FOREACH(this->m_icons, iconObject) {
			auto icon = static_cast<CCMenuItemSpriteExtra*>(iconObject);
			auto normalImage = static_cast<CCMenuItemSpriteExtra*>(icon->getNormalImage());
			auto children = normalImage->getChildren();
			auto icon2 = static_cast<CCMenuItemSpriteExtra*>(children->objectAtIndex(0));
			auto tintTo = CCTintTo::create(0.2f, 125, 125, 125);
			normalImage->runAction(tintTo);
			tintTo = CCTintTo::create(0.2f, 100, 100, 100);
			icon2->runAction(tintTo);
		}
	}
	this->resetGameTimer();
}

void SecretGame01LayerR::resetGameTimer() {
	this->m_timerRunning = true;
	this->m_timePassed = 0.f;
	auto time = this->getTimeForDifficulty(this->m_difficulty);
	this->m_maxTime = time;
	this->update(0);
	this->m_timeBar->setVisible(true);
}

void SecretGame01LayerR::showGameWon() {
	this->m_difficulty++;
	this->scaleOutGame(true);
}

void SecretGame01LayerR::scaleOutGame(bool won) {
	this->m_timeBar->setVisible(false);
	this->m_timerRunning = false;
	this->m_menu->setTouchEnabled(false);

	_ccColor3B color;

	if (won) color = { 0, 255, 0 };
	else color = { 255, 0, 0 };

	int tagOfNextIcon = -1;

	if (this->m_nextIconIndex < this->m_correctIcons->count()) {
		auto nextIcon = static_cast<CCMenuItemSpriteExtra*>(this->m_correctIcons->objectAtIndex(this->m_nextIconIndex));
		tagOfNextIcon = nextIcon->getTag();
	}

	CCObject* iconObject;

	CCARRAY_FOREACH(this->m_icons, iconObject) {
		auto icon = static_cast<CCMenuItemSpriteExtra*>(iconObject);
		auto normalImage = static_cast<CCMenuItemSpriteExtra*>(icon->getNormalImage());
		auto childrenOfNormalImage = normalImage->getChildren();
		auto firstObjectOfNormalImage = static_cast<CCMenuItemSpriteExtra*>(childrenOfNormalImage->objectAtIndex(0));
		auto normalImageCopy = normalImage;

		_ccColor3B innerColor = color;
		if (won) innerColor = { 150, 255, 0 };
		else if (tagOfNextIcon == icon->getTag()) {
			innerColor = { 255, 175, 0 };
		}

		auto tintTo = betterTintTo(0.1f, innerColor);
		auto delay = CCDelayTime::create(0.2f);
		auto fadeOut = CCFadeOut::create(0.5f);
		auto sequence = CCSequence::create(tintTo, delay, fadeOut, nullptr);
		firstObjectOfNormalImage->runAction(sequence);
		normalImageCopy = firstObjectOfNormalImage;

		if (won) tintTo = betterTintTo(0.1f, color);
		else tintTo = betterTintTo(0.1f, innerColor);

		delay = CCDelayTime::create(0.3f);
		auto scaleTo = CCScaleTo::create(0.5f, 0.f);
		auto easeInOut = CCEaseInOut::create(scaleTo, 2.f);
		sequence = CCSequence::create(tintTo, delay, easeInOut, nullptr);
		normalImage->runAction(sequence);
	}
	auto delay = CCDelayTime::create(1.f);
	auto callFunc = CCCallFunc::create(this, callfunc_selector(SecretGame01LayerR::resetGame));
	auto sequence = CCSequence::create(delay, callFunc, nullptr);
	this->runAction(sequence);
}

void SecretGame01LayerR::showGameOver() {
	if (Mod::get()->getSettingValue<bool>("alwaysWin")) return this->showGameWon();
	this->m_difficulty = 0;
	this->scaleOutGame(false);
}

void SecretGame01LayerR::resetGame() {
	auto icons = this->m_icons;

	CCObject* iconObject;
	CCARRAY_FOREACH(icons, iconObject) {
		auto icon = static_cast<CCMenuItemSpriteExtra*>(iconObject);

		icon->removeFromParentAndCleanup(true);
	}

	icons->removeAllObjects();
	int rows = SecretGame01LayerR::getRowsForDifficulty(this->m_difficulty);

	auto director = CCDirector::sharedDirector();
	auto winSize = director->getWinSize();

	std::vector<std::vector<float>> lookupTable = { // I am so sorry, I just can't deal with the positions 
		{11.5, 7.25f},
		{15.25, 10.f},
		{17.25, 11.25f},
		{18.3, 12.f},
		{19.25, 12.5f},
		{19.75, 12.75f}
	};

	auto point = CCPoint(winSize.width * 0.5f, winSize.height * 0.5f);
	float halfRowsMathX = rows * lookupTable[rows - 2][0] - point.x;
	float halfRowsMathY = rows * lookupTable[rows - 2][1] + point.y;
	float xOffset = rows * 4.f - 4.f - halfRowsMathX;
	float yOffset = rows * 4.f + 10.f + halfRowsMathY;

	auto frame = getFrameForDifficulty(this->m_difficulty);

	int i = 0;
	if (rows * rows != 0) {
		do {
			int gridY = 0;
			if (rows != 0) gridY = i / rows;
			int gridX = i - gridY * rows;

			auto sprite = CCSprite::createWithSpriteFrameName(frame.c_str());
			sprite->setColor({ 75, 75, 75 });

			auto sprite2Frame = frame;
			int pos = sprite2Frame.rfind('_');
			if (pos != std::string::npos) {
				sprite2Frame.insert(pos + 1, "2_");
			}
			auto sprite2 = CCSprite::createWithSpriteFrameName(sprite2Frame.c_str());
			sprite->addChild(sprite2);
			sprite2->setPosition(sprite->getContentSize() / 2.f);
			sprite2->setColor({ 60, 60, 60 });
			sprite2->setZOrder(-1);

			auto selectButton = CCMenuItemSpriteExtra::create(sprite, nullptr, this, menu_selector(SecretGame01LayerR::onSelectButton));
			selectButton->setSizeMult(1.2f);
			selectButton->setTag(i);
			this->m_icons->addObject(selectButton);
			this->m_menu->addChild(selectButton);

			float x = gridX * 38.f + xOffset;
			float y = gridY * 38.f - yOffset;
			point = CCPoint(x, -y);
			auto nodeSpacePoint = m_menu->convertToNodeSpace(point);
			selectButton->setPosition(nodeSpacePoint);
			i = i + 1;
			sprite->setScale(0.1f);
			auto scaleTo = CCScaleTo::create(0.6f, 1.0f);
			auto easeElasticOut = CCEaseElasticOut::create(scaleTo, 0.6f);
			sprite->runAction(easeElasticOut);
		} while (i != rows * rows);
	}
	this->m_menu->setTouchEnabled(false);
	this->m_nextIconIndex = 0;

	auto delay = CCDelayTime::create(0.8f);
	auto callfunc = CCCallFunc::create(this, callfunc_selector(SecretGame01LayerR::gameStep01));
	auto sequence = CCSequence::create(delay, callfunc, nullptr);

	this->runAction(sequence);
}

void SecretGame01LayerR::update(float dt) {
	if (!this->m_timerRunning) return;
	auto timeBar = this->m_timeBar;
	this->m_timePassed = this->m_timePassed + dt;
	auto timeBarSize = timeBar->getContentWidth();
	float scale = 0.01f;
	if (0.01f < (100.f / timeBarSize) * (1.f - this->m_timePassed / this->m_maxTime)
		) {
		timeBar = this->m_timeBar;
		timeBarSize = timeBar->getContentWidth();
		scale = (100.f / timeBarSize) * (1.f - this->m_timePassed / this->m_maxTime);
	}
	timeBar->setScaleX(scale);
	if (this->m_maxTime <= this->m_timePassed) {
		this->showGameOver();
		return;
	}
}

void SecretGame01LayerR::onSelectButton(CCObject* sender) {
	CCMenuItemSpriteExtra* clickedObject = nullptr;

	auto tag = sender->getTag();
	int correctIconsCount = this->m_correctIcons->count();
	if (correctIconsCount - 1 < this->m_nextIconIndex) {
		clickedObject = nullptr;
		if (tag == -1) {
			this->m_nextIconIndex++;
			int arrayCount = this->m_correctIcons->count();
			if (arrayCount <= this->m_nextIconIndex) {
				this->showGameWon();
				return;
			}
			this->didSelectCorrectObject(clickedObject);
			return;
		}
	}
	else {
		clickedObject = static_cast<CCMenuItemSpriteExtra*>(this->m_correctIcons->objectAtIndex(this->m_nextIconIndex));
		correctIconsCount = clickedObject->getTag();
		if (correctIconsCount == tag) {
			this->m_nextIconIndex++;
			int arrayCount = this->m_correctIcons->count();
			if (arrayCount <= this->m_nextIconIndex) {
				this->showGameWon();
				return;
			}
			this->didSelectCorrectObject(clickedObject);
			return;
		}
	}
	this->showGameOver();
}

void SecretGame01LayerR::didSelectCorrectObject(CCMenuItemSpriteExtra* object) {
	if (!object) return;

	this->m_icons->removeObject(object, true);
	object->setEnabled(false);
	auto disabledImage = object->getNormalImage();
	auto children = disabledImage->getChildren();
	auto firstChild = static_cast<CCMenuItemSpriteExtra*>(children->objectAtIndex(0));
	auto tintTo = CCTintTo::create(0.1f, 0, -1, 0);
	auto delay = CCDelayTime::create(0.05f);
	auto fadeOut = CCFadeOut::create(0.3f);
	auto delay2 = CCDelayTime::create(0.3f);
	auto callfunc = CCCallFunc::create(object, callfunc_selector(CCNode::removeMeAndCleanup));
	auto sequence = CCSequence::create(tintTo, delay, fadeOut, delay2, callfunc, nullptr);
	disabledImage->runAction(sequence);
	auto tintTo2 = CCTintTo::create(0.1f, 0, -1, 0);
	delay = CCDelayTime::create(0.05f);
	auto fadeOut2 = CCFadeOut::create(0.3f);
	delay2 = CCDelayTime::create(0.3f);
	callfunc = CCCallFunc::create(object, callfunc_selector(CCNode::removeMeAndCleanup));
	sequence = CCSequence::create(tintTo2, delay, fadeOut2, delay2, callfunc, nullptr);
	firstChild->runAction(sequence);
	delay = CCDelayTime::create(0.15f);
	auto scaleTo = CCScaleTo::create(0.3f, 0.1f);
	auto easeInOut = CCEaseInOut::create(scaleTo, 2.f);
	sequence = CCSequence::create(delay, easeInOut, nullptr);
	disabledImage->runAction(sequence);
}

CCTintTo* SecretGame01LayerR::betterTintTo(float duration, _ccColor3B color) {
	return CCTintTo::create(duration, color.r, color.g, color.b);
}

std::string SecretGame01LayerR::getFrameForDifficulty(int difficulty) {
	std::string frame = "";
	switch (difficulty) {
		case 0:
			frame = "player_01_001.png";
			break;
		default:
			frame = "player_04_001.png";
			break;
		case 4: [[fallthrough]];
		case 5: [[fallthrough]];
		case 6: [[fallthrough]];
		case 7:
			frame = "player_12_001.png";
			break;
		case 8: [[fallthrough]];
		case 9: [[fallthrough]];
		case 10: [[fallthrough]];
		case 11: [[fallthrough]];
		case 12:
			frame = "player_73_001.png";
			break;
	}
	if (!Mod::get()->getSettingValue<bool>("fixDifficultyFrame")) frame = "player_01_001.png";
	return frame;
}
#endif