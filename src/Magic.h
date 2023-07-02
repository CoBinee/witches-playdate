// Magic.h - 魔法
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
    kMagciRangeLeft = -8, 
    kMagicRangeRight = 409, 
    kMagciRangeTop = -8, 
    kMagicRangeBottom = 208, 
};

// 矩形
//
enum {
    kMagicRectLeft = -2, 
    kMagicRectTop = -2, 
    kMagicRectRight = 1, 
    kMagicRectBottom = 1, 
};

// 速度
//
enum {
    kMagicSpeed = 8, 
    kMagicCycle = 2, 
};

// 魔法
//
struct Magic {

    // アクタ
    struct Actor actor;

    // 位置
    struct Vector position;

    // ベクトル
    struct Vector vector;

    // 回転
    float rotation;

    // 矩形
    struct Rect rect;

    // アニメーション
    struct AsepriteSpriteAnimation animation;
};

// 外部参照関数
//
extern void MagicInitialize(void);
extern void MagicLoad(float x, float y, float angle);
