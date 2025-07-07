#include <Geode/Geode.hpp>

#include "SecretLayer6R.h"
using namespace geode::prelude;

#include <Geode/modify/SecretLayer5.hpp>
class $modify(MySecretLayer5, SecretLayer5) {
    struct Fields : DialogDelegate {
        CCMenuItemSpriteExtra* m_playerBtn = nullptr;
        void dialogClosed(DialogLayer* layer) override {
            if (!this->m_playerBtn) return;
            auto fadeIn = CCFadeIn::create(0.25f);
            auto scaleTo = CCScaleTo::create(0.25f, 1.f);
            auto easeIn = CCEaseIn::create(scaleTo, 2.f);
            auto sequence = CCSequence::create(fadeIn, easeIn, nullptr);
            this->m_playerBtn->runAction(sequence);
            this->m_playerBtn->setEnabled(true);
        }
    };

    bool init() override {
        if (!SecretLayer5::init()) {
            return false;
        }

        CCMenu* menu = this->getChildByType<CCMenu>(0);

        if (!menu) return true;

        CCSprite* playerBtnSprite = CCSprite::create("playerButton.png"_spr);
        CCMenuItemSpriteExtra* playerBtn = CCMenuItemSpriteExtra::create(playerBtnSprite, nullptr, this, menu_selector(MySecretLayer5::onIcon));
        CCDirector* director = CCDirector::sharedDirector();
        float right = director->getScreenRight();
        float bottom = director->getScreenBottom();

        auto position = ccp(right - playerBtn->getContentWidth() / 2 - 4, bottom + playerBtn->getContentHeight() / 2 + 4);
        playerBtn->setPosition(menu->convertToNodeSpace(position));
        playerBtn->setID("icon-button"_spr);

        if (!Mod::get()->getSavedValue<bool>("icon-unlocked", false)) {
            playerBtn->setOpacity(0);
            playerBtn->setScale(1.5f);
            playerBtn->setEnabled(false);
        }
        this->m_fields->m_playerBtn = playerBtn;
        menu->addChild(playerBtn);

        return true;
    }

    void enableIcon() {
        Mod::get()->setSavedValue<bool>("icon-unlocked", true);
        this->m_textInput->setString("");
        this->m_textInput->onClickTrackNode(false);

        CCArray* objects = CCArray::create();
        objects->addObject(DialogObject::create("The Wraith", "<cj>\"Mastermind\"</c>, you say...<d050>\n<cy>That reminds me</c><d010>.<d015>.<d020>.", 53, 1.f, true, ccc3(255, 255, 255)));
        objects->addObject(DialogObject::create("The Wraith", "I got <cy>this minigame</c> lying around...", 54, 1.f, true, ccc3(255, 255, 255)));
        auto dialogLayer = DialogLayer::createWithObjects(objects, 4);
        dialogLayer->setID("mastermind-dialog"_spr);
        dialogLayer->m_delegate = this->m_fields.self();
        this->addChild(dialogLayer, 100);
        dialogLayer->updateChatPlacement(DialogChatPlacement::Center);
        dialogLayer->animateInRandomSide();
    }

    void onIcon(CCObject* sender) {
#ifdef NEEDS_DECOMP
        auto scene = SecretLayer6R::scene();
#else
        auto scene = SecretLayer6::scene();
#endif
        auto director = CCDirector::sharedDirector();
        auto transition = CCTransitionFade::create(0.5f, scene);

        director->pushScene(transition);
    }

    void onSubmit(CCObject* sender) {
        std::string text = this->m_textInput->getString();
        std::transform(
            text.begin(),
            text.end(),
            text.begin(),
            [](unsigned char c) {
                return std::tolower(c);
            });

        if (text == "mastermind" && !Mod::get()->getSavedValue<bool>("icon-unlocked", false)) {
            this->enableIcon();
            return;
        }
        SecretLayer5::onSubmit(sender);
    }
};