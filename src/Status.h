// Status.h - ステータス
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"
#include "Aseprite.h"
#include "Define.h"


// アニメーション
//
enum {
    kStatusAnimationBack = 0, 
    kStatusAnimationYou, 
    kStatusAnimationRival, 
    kStatusAnimationSize, 
};

// ステータス
//
struct Status {

    // アクタ
    struct Actor actor;

    // アニメーション
    struct AsepriteSpriteAnimation animations[kStatusAnimationSize];

};

// 外部参照関数
//
extern void StatusInitialize(void);
extern void StatusLoad(void);
