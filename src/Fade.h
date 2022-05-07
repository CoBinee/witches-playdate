// Fade.h - フェード
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"
#include "Aseprite.h"


// 大きさ
//
enum {
    kFadeSizeX = 50, 
    kFadeSizeY = 30, 
    kFadeSizePixel = 8, 
};

// フェード
//
enum {
    kFadeIn = 0, 
    kFadeOut, 
};

// フェード
//
struct Fade {

    // アクタ
    struct Actor actor;

    // アニメーション
    struct AsepriteSpriteAnimation *animations;

};

// 外部参照関数
//
extern void FadeInitialize(void);
extern void FadeLoad(int type);
extern void FadeKill(void);
extern bool FadeIsDone(void);
