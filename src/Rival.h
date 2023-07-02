// Rival.h - ライバル
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
    kRivalRangeLeft = 12, 
    kRivalRangeRight = 388, 
    kRivalRangeMiddle = 48, 
};

// ライバル
//
struct Rival {

    // アクタ
    struct Actor actor;

    // 位置
    struct Vector position;

    // 向き
    float direction;

    // 移動
    float moveSpeed;

    // カーブ
    float curveSpeed;
    float curveMaximum;
    float curveAccel;

    // 結晶
    int crystal;

    // アニメーション
    struct AsepriteSpriteAnimation animation;

};

// 外部参照関数
//
extern void RivalInitialize(void);
extern void RivalLoad(void);
