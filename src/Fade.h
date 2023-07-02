// Fade.h - フェード
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"
#include "Aseprite.h"


// 種類
//
enum {
    kFadeTypeIn = 0, 
    kFadeTypeOut, 
};

// 大きさ
//
enum {
    kFadeSizeX = 50, 
    kFadeSizeY = 30, 
    kFadeSizePixel = 8, 
};

// フェード
//
struct Fade {

    // アクタ
    struct Actor actor;

    // 種類
    int type;

    // フィルタ
    int filter;

    // アニメーション
    struct AsepriteSpriteAnimation *animations;

};

// 外部参照関数
//
extern void FadeInitialize(void);
extern void FadeLoad(int type);
extern void FadeKill(void);
extern bool FadeIsDone(void);
