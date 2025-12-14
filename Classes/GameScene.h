//
// Created by user on 2016/10/29.
//

#ifndef SEVENS_GAMESCENE_H
#define SEVENS_GAMESCENE_H

//#include "cocos2d.h"
//#include "ui/CocosGUI.h"
//#include "Common.h"
#include "Manager.h"
#include "PlayerCharacter.h"
#include "NonPlayerCharacter.h"

USING_NS_CC;

class GameScene : public cocos2d::Layer
{
public:
    Manager* manager;
    std::vector<Character*> characters;
    PlayerCharacter* player_character;
    Sprite* background_image;
    DrawNode* frame_lines[5][4];
    std::vector<Card*> game_cards;
    
    static Scene* createScene();
    static GameScene* getGameScene();
    float getGameSpeed();
    void resumeNext();
    void showResultDialog(std::string text, std::string text_right);
    
private:
    GameScene();
    ~GameScene();
    CREATE_FUNC(GameScene);
    virtual bool init();
    virtual void onEnterTransitionDidFinish();
    void createFrameLine(int line_no, Vec2 point, float width, float height);
    bool touchBegan(Vec2 pos);
    void touchEnd();
    void onPassButtonTouched(Ref* pSender, ui::Widget::TouchEventType type);
    void resultDialogCallback(Ref* Sender);
};

#endif //SEVENS_GAMESCENE_H
