//
// Created by user on 2016/10/29.
//

#include "audio/include/AudioEngine.h"
#include "GameScene.h"
#include "AI1.h"
#include "AI2.h"
#include "AI3.h"
#include "UIDialog.h"
#include "NativeCodeLauncher.h"
#include "TitleScene.h"

#if(CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
static bool sAdflg = false;
#endif
static GameScene* sGameScene = nullptr;

Scene* GameScene::createScene()
{
    sGameScene      = GameScene::create();
    Scene* scene    = Scene::create();
    scene->addChild(sGameScene, 0, 0);
    return scene;
}

GameScene* GameScene::getGameScene()
{
    return sGameScene;
}

GameScene::GameScene()
{
    // マネージャー
    this->manager = Manager::getManager();
    
    // ユーザー
    // <! プレイヤー
    this->player_character = PlayerCharacter::create(0);
    this->characters.emplace_back(this->player_character);
    // <! npc1
    NonPlayerCharacter* npc1 = NonPlayerCharacter::create(1, "panda.png");
    npc1->setAI(AI1::create());
    this->characters.emplace_back(npc1);
    // <! npc2
    NonPlayerCharacter* npc2 = NonPlayerCharacter::create(2, "inu.png");
    npc2->setAI(AI2::create());
    this->characters.emplace_back(npc2);
    // <! npc3
    NonPlayerCharacter* npc3 = NonPlayerCharacter::create(3, "usagi.png");
    npc3->setAI(AI3::create());
    this->characters.emplace_back(npc3);
    
    this->manager->setCharacters(this->characters);
    
    // タッチイベントを登録する
    cocos2d::EventListenerTouchOneByOne* eventListener = EventListenerTouchOneByOne::create();
    eventListener->onTouchBegan = [this](Touch* touch, Event* event) {
        return this->touchBegan(touch->getLocation());
    };
    eventListener->onTouchMoved = [this](Touch* touch, Event* event) {
        return this->touchBegan(touch->getLocation());
    };
    eventListener->onTouchEnded = [this](Touch* touch, Event* event) {
        this->touchEnd();
    };
    auto dispatcher = Director::getInstance()->getEventDispatcher();
    dispatcher->addEventListenerWithSceneGraphPriority(eventListener, this);
    
    //　カード画像をキャッシュに読み込む
    TextureCache* textureCache = Director::getInstance()->getTextureCache();
    for (int i = 1; i <= 13; i++) {
        textureCache->addImage(StringUtils::format("s%d.png", i));
        textureCache->addImage(StringUtils::format("h%d.png", i));
        textureCache->addImage(StringUtils::format("d%d.png", i));
        textureCache->addImage(StringUtils::format("c%d.png", i));
    }
    textureCache->addImage("back_card.png");
    textureCache->addImage("joker.png");
    
    // 背景
    this->background_image = Sprite::create("bg.png");
    this->background_image->setPosition(Director::getInstance()->getVisibleSize() / 2);
    CC_SAFE_RETAIN(this->background_image);
    
    // 枠線
    createFrameLine(0, Vec2(226.0f, 6.0f), 1048.0f, 216.0f);
    createFrameLine(1, Vec2(4.0f, 500.0f), 216.0f, 216.0f);
    createFrameLine(2, Vec2(4.0f, 278.0f), 216.0f, 216.0f);
    createFrameLine(3, Vec2(4.0f, 56.0f), 216.0f, 216.0f);
    createFrameLine(4, Vec2(226.0f, 228.0f), 1048.0f, 488.0f);
    
    // カード
    for (int i = 1; i <= 13; i++) {
        this->game_cards.emplace_back(Card::create(Mark::SPADE, i));
        this->game_cards.emplace_back(Card::create(Mark::HEART, i));
        this->game_cards.emplace_back(Card::create(Mark::DIAMOND, i));
        this->game_cards.emplace_back(Card::create(Mark::CLUB, i));
    }
    if (config_use_joker) {
        this->game_cards.emplace_back(Card::create(Mark::JOKER, 99));
    }
}

/**
 * 枠線を描画する
 */
void GameScene::createFrameLine(int line_no, Vec2 point, float width, float height)
{
    Color4F color       = Color4F::YELLOW;
    float size          = 2.0f;
    
    float bottom        = point.x;
    float left          = point.y;
    float top           = point.x + width;
    float right         = point.y + height;
    
    Vec2 bottomLeft     = Vec2(bottom, left);
    Vec2 topLeft        = Vec2(top, left);
    Vec2 topRight       = Vec2(top, right);
    Vec2 bottomRight    = Vec2(bottom, right);
    
    this->frame_lines[line_no][0] = DrawNode::create();
    this->frame_lines[line_no][1] = DrawNode::create();
    this->frame_lines[line_no][2] = DrawNode::create();
    this->frame_lines[line_no][3] = DrawNode::create();
    
    this->frame_lines[line_no][0]->drawSegment( bottomLeft, topLeft, size, color );
    this->frame_lines[line_no][1]->drawSegment( topLeft, topRight, size, color );
    this->frame_lines[line_no][2]->drawSegment( topRight, bottomRight, size, color );
    this->frame_lines[line_no][3]->drawSegment( bottomRight, bottomLeft, size, color );
    
    CC_SAFE_RETAIN(this->frame_lines[line_no][0]);
    CC_SAFE_RETAIN(this->frame_lines[line_no][1]);
    CC_SAFE_RETAIN(this->frame_lines[line_no][2]);
    CC_SAFE_RETAIN(this->frame_lines[line_no][3]);
}

GameScene::~GameScene()
{
    CC_SAFE_DELETE(manager);
    for (Character* charcter : this->characters) {
        CC_SAFE_DELETE(charcter);
    }
    
    CC_SAFE_RELEASE_NULL(this->background_image);
    
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            CC_SAFE_RELEASE_NULL(this->frame_lines[i][j]);
        }
    }
    
    for (Card* card : this->game_cards) {
        CC_SAFE_DELETE(card);
    }
}

bool GameScene::init()
{
    if (!Layer::init()) {
        return false;
    }
    
    //<! 変数初期化
    this->manager->init();
    for (Character* character : this->characters) {
        character->init();
    }
    
    //<! カードを配る
    this->manager->dealCard(this->game_cards);
    
    //<! 画面をクリアする
    removeAllChildren();
    
    //<! 背景
    addChild(this->background_image);
    
    //<! 枠線
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 4; j++) {
            addChild(this->frame_lines[i][j]);
        }
    }
    
    //<! player-character
    addChild(this->player_character->label_rank);
    addChild(this->player_character->label_pass);
    addChild(this->player_character->label_message);
    addChild(this->player_character->button_pass);
    this->player_character->button_pass->addTouchEventListener(CC_CALLBACK_2(GameScene::onPassButtonTouched, this));
    this->player_character->sortCards();
    std::vector<Card*> cards = this->player_character->hands->getCards();
    for (int i = 0; i < cards.size(); i++) {
        addChild(cards.at(i)->card_image);
        cards.at(i)->card_image->setGlobalZOrder(i);
    }
    
    //<! non-player-character
    for (int i = 0; i < this->characters.size(); i++) {
        Character* character = this->characters.at(i);
        if (character->getType() == Character::TYPE_PLAYER) {
            continue;
        }
        addChild(character->thumbnail);
        addChild(character->label_rank);
        addChild(character->label_pass);
        addChild(character->label_message);
        std::vector<Card*> cards = character->hands->getCards();
        for (int j = 0; j < cards.size(); j++) {
            addChild(cards.at(j)->card_image);
            cards.at(j)->card_image->setGlobalZOrder(j);
        }
    }
    
    return true;
}

void GameScene::onEnterTransitionDidFinish()
{
    //<! ゲームを開始する
    this->manager->start();
}

/**
 * 画面タッチ開始または移動を検知した時に呼ばれる処理
 */
bool GameScene::touchBegan(Vec2 pos)
{
    if (!this->player_character->is_touch_enabled) {
        return false;
    }
    
    std::vector<Card*> target_cards = this->manager->askTargetCards();
    
    std::vector<Card*> enable_cards = {};
    for (Card* player_card : this->player_character->hands->getCards()) {
        for (Card* target_card : target_cards) {
            if (target_card == player_card) {
                enable_cards.emplace_back(player_card);
            }
        }
    }
    
    std::vector<Card*> enable_cards_with_joker = {};
    if (this->player_character->touched_card && this->player_character->touched_card->isJoker()) {
        for (Card* player_card : this->player_character->hands->getCards()) {
            for (Card* target_card : this->manager->askTargetCardsWithJoker()) {
                if (target_card == player_card) {
                    bool have_proxy_card = false;
                    for (Card* enable_card : enable_cards) {
                        if (enable_card->mark == target_card->mark) {
                            if ((enable_card->number + 1 == target_card->number) || (enable_card->number - 1 == target_card->number)) {
                                have_proxy_card = true;
                                break;
                            }
                        }
                    }
                    if (!have_proxy_card) {
                        enable_cards.emplace_back(player_card);
                        enable_cards_with_joker.emplace_back(player_card);
                    }
                }
            }
        }
    }
    
    for (int i = (int)enable_cards.size() - 1; i >= 0; i--) {
        Card* card = enable_cards.at(i);
        
        Vec2    card_position   = card->card_image->getPosition();
        float   left            = card_position.x - (CARD_WIDTH * CARD_SCALE_PLAYER) / 2;
        float   right           = card_position.x + (CARD_WIDTH * CARD_SCALE_PLAYER) / 2;
        float   bottom          = card_position.y - (CARD_HEIGHT * CARD_SCALE_PLAYER) / 2;
        float   top             = card_position.y + (CARD_HEIGHT * CARD_SCALE_PLAYER) / 2;
        
        if (pos.x >= left && pos.x <= right && pos.y >= bottom && pos.y <= top) {
            Card* touching_card = card;
            
            if (this->player_character->touched_card) {
                if (this->player_character->touched_card == touching_card) {
                    // タッチしたカードが変わっていない場合何もしない
                    break;
                } else {
                    if (this->player_character->touched_card->isJoker()) {
                        if (this->player_character->touched_card->pair_card) {
                            if (this->player_character->touched_card->pair_card == touching_card) {
                                // タッチしたカードが変わっていない場合何もしない
                                break;
                            } else {
                                if (Card::existCardInVector(enable_cards_with_joker, touching_card)) {
                                    Card* proxy_card = nullptr;
                                    for (Card* target_card : target_cards) {
                                        if (target_card->mark == touching_card->mark) {
                                            if ((target_card->number + 1 == touching_card->number) || (target_card->number - 1 == touching_card->number)) {
                                                proxy_card = target_card;
                                                break;
                                            }
                                        }
                                    }
                                    this->player_character->touched_card->pair_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y);
                                    this->player_character->touched_card->proxy_card = proxy_card;
                                    this->player_character->touched_card->pair_card = touching_card;
                                    touching_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y + 30);
                                } else {
                                    this->player_character->touched_card->pair_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y);
                                    this->player_character->touched_card->proxy_card = nullptr;
                                    this->player_character->touched_card->pair_card = nullptr;
                                    this->player_character->touched_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y);
                                    touching_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y + 30);
                                    this->player_character->touched_card = touching_card;
                                }
                            }
                        } else {
                            if (Card::existCardInVector(enable_cards_with_joker, touching_card)) {
                                Card* proxy_card = nullptr;
                                for (Card* target_card : target_cards) {
                                    if (target_card->mark == touching_card->mark) {
                                        if ((target_card->number + 1 == touching_card->number) || (target_card->number - 1 == touching_card->number)) {
                                            proxy_card = target_card;
                                            break;
                                        }
                                    }
                                }
                                this->player_character->touched_card->proxy_card = proxy_card;
                                this->player_character->touched_card->pair_card = touching_card;
                                touching_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y + 30);
                            } else {
                                this->player_character->touched_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y);
                                touching_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y + 30);
                                this->player_character->touched_card = touching_card;
                            }
                        }
                    } else {
                        this->player_character->touched_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y);
                        touching_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y + 30);
                        this->player_character->touched_card = touching_card;
                    }
                }
            } else {
                touching_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y + 30);
                this->player_character->touched_card = touching_card;
            }
            
            break;
        }
    }
    
    return true;
}

/**
 * 画面タッチ終了を検知した時に呼ばれる処理
 */
void GameScene::touchEnd()
{
    if (!this->player_character->is_touch_enabled) {
        return;
    }
    
    if (!this->player_character->touched_card) {
        return;
    }
    
    if (this->player_character->touched_card->isJoker()) {
        if (!this->player_character->touched_card->pair_card) {
            return;
        }
    }
    
    this->player_character->is_touch_enabled = false;
    this->player_character->button_pass->setBright(false);
    this->player_character->button_pass->setTouchEnabled(false);
    
    this->player_character->process(Command::PUT_NORMAL);
}

/**
 * パスボタンをタッチした時に呼ばれる処理
 */
void GameScene::onPassButtonTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
    if (!this->player_character->is_touch_enabled) {
        return;
    }
    
    switch (type) {
        case ui::Widget::TouchEventType::BEGAN: {
            if (this->player_character->touched_card) {
                this->player_character->touched_card->proxy_card = nullptr;
                this->player_character->touched_card->pair_card = nullptr;
                this->player_character->touched_card->card_image->setPositionY(PLAYER_CHARACTER_CARD_POSITION_Y);
                this->player_character->touched_card = nullptr;
            }
            this->player_character->is_touch_enabled = false;
            this->player_character->button_pass->setBright(false);
            this->player_character->button_pass->setTouchEnabled(false);
            this->player_character->process(Command::PUT_NORMAL);
            break;
        }
        default:
            break;
    }
}

/**
 * 結果ダイアログ
 */
void GameScene::showResultDialog(std::string text, std::string text_right)
{
    cocos2d::ccMenuCallback action = CC_CALLBACK_1(GameScene::resultDialogCallback, this);
    std::vector<UIDialogButton*> buttons = {
        new UIDialogButton("つづける", action, 1),
        new UIDialogButton("もどる", action, 2),
        new UIDialogButton("おわる", action, 3),
    };
    auto* dialog = UIDialog::create("", text, text_right, buttons);
    addChild(dialog, 100, 100);
    dialog->setGlobalZOrder(100);
}

/**
 * 結果ダイアログコールバック
 */
void GameScene::resultDialogCallback(Ref* Sender)
{
    switch(((MenuItem*)Sender)->getTag())
    {
        case 1: // 続ける
        {
#if(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
            this->resumeNext();
            
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
            if (sAdflg) {
                sAdflg = false;
                this->resumeNext();
            } else {
                std::random_device rnd;
                std::mt19937 mt(rnd());
//                std::uniform_int_distribution<int> rand_dist(1, 10);
                std::uniform_int_distribution<int> rand_dist(1, 1);
                int x = rand_dist(mt);
                if (x == 1) {
                    sAdflg = true;
                    // 結果ダイアログを閉じる
                    static_cast<UIDialog*>(this->getChildByTag(100))->close();
                    // 広告を表示する
                    NativeCodeLauncher::advertise();
                    // ローディング中の旨を表示する
                    std::vector<UIDialogButton*> buttons = {};
                    auto* dialog = UIDialog::create("Now Loading…", "", "", buttons);
                    addChild(dialog, 100, 100);
                } else {
                    this->resumeNext();
                }
            }
#endif
            break;
        }
        case 2:  // タイトル画面へ
        {
            Scene* scene = TitleScene::createScene();
            Director::getInstance()->replaceScene(scene);
            break;
        }
        case 3:  // 終了
        {
            Director::getInstance()->end();
            break;
        }
    }
}

/**
 * ゲームスピード
 */
float GameScene::getGameSpeed()
{
    std::map<int, float> speed_map = {
        {SETTING_SLOW,      0.5},
        {SETTING_NORMAL,    1.0},
        {SETTING_QUICK,     2.0},
    };
    
    float speed = speed_map[config_speed_setting];
    
    if (this->player_character->isFinish()) {
        speed *= 2;
    }
    
    return speed;
}

/**
 * 次へ
 */
void GameScene::resumeNext()
{
    static_cast<UIDialog*>(this->getChildByTag(100))->close();
    this->init();
    this->manager->start();
}

