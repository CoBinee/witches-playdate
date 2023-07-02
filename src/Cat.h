// Cat.h - 猫
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"
#include "Aseprite.h"
#include "Define.h"


// 範囲
//
enum {
    kCatRangeLeft = 8, 
    kCatRangeRight = 392, 
    kCatRangeBottom = 200, 
};

// 矩形
//
enum {
    kCatRectLeft = -8, 
    kCatRectTop = -16, 
    kCatRectRight = 7, 
    kCatRectBottom = -1, 
};

// 猫
//
struct Cat {

    // アクタ
    struct Actor actor;

    // 位置
    struct Vector position;

    // 向き
    float direction;

    // 速度
    float speed;

    // ミリ秒
    int millisecond;

    // 矩形
    struct Rect rect;

    // アニメーション
    struct AsepriteSpriteAnimation animation;

};

// 外部参照関数
//
extern void CatInitialize(void);
extern void CatLoad(void);
extern int CatIsHit(struct Rect *rect);
