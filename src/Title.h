// Title.h - タイトル
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"


// タイトル関数
//
typedef void (*TitleFunction)(void *game);

// タイトル
//
struct Title {

    // 処理関数
    TitleFunction function;

    // 状態
    int state;
};

// スプライト
//
typedef enum {
    kTitleSpriteNameLogo = 0, 
    kTitleSpriteNameCrystal, 
    kTitleSpriteNameFade, 
    kTitleSpriteNameSize, 
} TitleSpriteName;

// プライオリティ
//
enum {
    kTitlePriorityNull = 0, 
    kTitlePriorityLogo, 
    kTitlePriorityCrystal, 
    kTitlePriorityStarter, 
};

// タグ
//
enum {
    kTitleTagNull = 0, 
    kTitleTagLogo, 
    kTitleTagCrystal, 
    kTitleTagStarter, 
};

// 描画順
//
enum {
    kTitleOrderNull = 0, 
    kTitleOrderLogo, 
    kTitleOrderCrystal, 
    kTitleOrderStarter, 
};


// 外部参照関数
//
extern void TitleUpdate(struct Title *title);
