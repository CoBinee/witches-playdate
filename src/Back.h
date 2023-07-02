// Back.h - 背景
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
    kBackSizeX = 50, 
    kBackSizeY = 25, 
    kBackSizePixel = 8, 
};

// 色
//
enum {
    kBackColorLight = 0, 
    kBackColorDark = 8, 
    kBackColorSize, 
};

// 速度
//
enum {
    kBackSpeed = 3, 
};

// 背景
//
struct Back {

    // アクタ
    struct Actor actor;

    // 色
    int *colors;

    // ヘッド
    int heads[kBackSizeX];

    // 速度
    int speeds[kBackSizeX];

    // スコア
    int scoreLight;
    int scoreDark;

    // アニメーション
    struct AsepriteSpriteAnimation *animations;

};

// 外部参照関数
//
extern void BackInitialize(void);
extern void BackLoad(void);
extern bool BackIsDarkThenLight(int x, int y);
extern int BackGetScoreLight(void);
extern int BackGetScoreDark(void);
