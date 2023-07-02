// Crystal.h - 結晶
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
    kCrystalRangeLeft = 8, 
    kCrystalRangeRight = 392, 
    kCrystalRangeTop = -8, 
    kCrystalRangeBottom = 248, 
};

// 矩形
//
enum {
    kCrystalRectLeft = -5, 
    kCrystalRectTop = -5, 
    kCrystalRectRight = 4, 
    kCrystalRectBottom = 4, 
};

// 結晶
//
struct Crystal {

    // アクタ
    struct Actor actor;

    // 位置
    struct Vector position;

    // 速度
    float speed;

    // 描画順
    int order;

    // 矩形
    struct Rect rect;

    // アニメーション
    struct AsepriteSpriteAnimation animation;

};

// 外部参照関数
//
extern void CrystalInitialize(void);
extern void CrystalLoad(int priority, int tag, int order);
extern int CrystalIsHit(struct Rect *rect, int tag);
extern int CrystalIsBomb(struct Rect *rect, int tag);
