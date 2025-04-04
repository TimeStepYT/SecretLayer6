#pragma once
#include <Geode/Geode.hpp>

#ifdef NEEDS_DECOMP
class SecretGame01LayerR : public cocos2d::CCLayer {
public:
	static SecretGame01LayerR* create();

	bool init();
	void update(float);
	void gameStep01();
	void gameStep02();
	void resetGameTimer();
	void scaleOutGame(bool);
	void showGameOver();
	void showGameWon();
	void didSelectCorrectObject(CCMenuItemSpriteExtra*);
	void onSelectButton(cocos2d::CCObject*);
	void resetGame();
	int getRowsForDifficulty(int);
	unsigned int getTimeForDifficulty(int);
	unsigned int getCountForDifficulty(int);
	gd::string getFrameForDifficulty(int);
	cocos2d::CCTintTo* betterTintTo(float, cocos2d::_ccColor3B);

	unsigned int m_difficulty;
	cocos2d::CCMenu* m_menu;
	int m_nextIconIndex;
	cocos2d::CCArray* m_icons;
	cocos2d::CCArray* m_correctIcons;
	cocos2d::CCSprite* m_timeBar;
	bool m_timerRunning;
	float m_timePassed;
	float m_maxTime;
};
#endif