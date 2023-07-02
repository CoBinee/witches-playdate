// Notice.h - 通知
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"
#include "Aseprite.h"
#include "Define.h"


// インアウト
//
enum {
    kNoticeInoutMillisecond = 660, 
};

// アニメーション
//
enum {
    kNoticeAnimationJump = 0, 
    kNoticeAnimationMove, 
    kNoticeAnimationSize, 
};

// 通知
//
struct Notice {

    // アクタ
    struct Actor actor;

    // インアウト
    int inout;

    // アニメーション
    struct AsepriteSpriteAnimation animations[kNoticeAnimationSize];

};

// 外部参照関数
//
extern void NoticeInitialize(void);
extern void NoticeLoad(void);
