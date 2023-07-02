// You.h - あなた
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"
#include "Aseprite.h"
#include "Define.h"


// 中心
//
enum {
    kYouCenterX = 0, 
    kYouCenterY = -10, 
};

// 照準
//
enum {
    kYouAimR = 26, 
};

// 範囲
//
enum {
    kYouRangeLeft = 8, 
    kYouRangeRight = 392, 
    kYouRangeBottom = 200, 
};

// 矩形
//
enum {
    kYouRectLeft = -8, 
    kYouRectTop = -12, 
    kYouRectRight = 7, 
    kYouRectBottom = -1, 
};

// 点滅
//
enum {
    kYouBlinkDamage = 30, 
};

// アニメーション
//
enum {
    kYouAnimationWitch = 0, 
    kYouAnimationAim, 
    kYouAnimationSize, 
};

// あなた
//
struct You {

    // アクタ
    struct Actor actor;

    // 位置
    struct Vector position;

    // 向き
    float direction;

    // 移動
    float moveSpeed;
    float moveMaximum;
    float moveAccel;

    // ジャンプ
    float jumpSpeed;
    float jumpStart;
    float jumpGravity;

    // 中心
    struct Vector center;

    // 照準
    struct Vector aim;
    float aimDegree;
    float aimRadian;

    // 放つ
    int fire;

    // 点滅
    int blink;

    // 矩形
    struct Rect rect;

    // アニメーション
    struct AsepriteSpriteAnimation animations[kYouAnimationSize];

};

// 外部参照関数
//
extern void YouInitialize(void);
extern void YouLoad(void);
extern struct Vector *YouGetPosition(void);
extern struct Rect *YouGetRect(void);
extern void YouDamage(int direction);
