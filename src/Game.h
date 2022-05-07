// Game.h - ゲーム
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Define.h"


// ゲーム関数
//
typedef void (*GameFunction)(void *game);

// タイム
enum {
    kGameTimeStart = 1999, 
};

// ゲーム
//
struct Game {

    // 処理関数
    GameFunction function;

    // 状態
    int state;

    // タイム
    int time;

    // プレイ中
    bool play;

};

// スプライト
//
typedef enum {
    kGameSpriteNameBack = 0, 
    kGameSpriteNameYou, 
    kGameSpriteNameAim, 
    kGameSpriteNameMagic, 
    kGameSpriteNameRival, 
    kGameSpriteNameCat, 
    kGameSpriteNameCrystal, 
    kGameSpriteNameStatus,  
    kGameSpriteNameFade,  
    kGameSpriteNameSize, 
} GameSpriteName;

// オーディオ
//
enum {
    kGameAudioSampleSize, 
};

// プライオリティ
//
enum {
    kGamePriorityNull = 0, 
    kGamePriorityBack, 
    kGamePriorityYou, 
    kGamePriorityRival, 
    kGamePriorityCat, 
    kGamePriorityCrystal, 
    kGamePriorityMagic, 
    kGamePriorityStatus, 
    kGamePriorityAnnounce, 
    kGamePriorityFade, 
};

// タグ
//
enum {
    kGameTagNull = 0, 
    kGameTagBack, 
    kGameTagYou, 
    kGameTagMagic, 
    kGameTagRival, 
    kGameTagCat, 
    kGameTagCrystal, 
    kGameTagStatus, 
    kGameTagAnnounce, 
    kGameTagFade, 
};

// 描画順
//
enum {
    kGameOrderNull = 0, 
    kGameOrderBack, 
    kGameOrderMagic, 
    kGameOrderRival, 
    kGameOrderYou, 
    kGameOrderCrystal, 
    kGameOrderCat, 
    kGameOrderStatus, 
    kGameOrderAnnounce, 
    kGameOrderFade, 
};


// 外部参照関数
//
extern void GameUpdate(struct Game *game);
extern bool GameIsPlay(void);
extern int GameGetRealTime(void);
extern int GameGetViewTime(void);

