// Starter.h - スターター
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"
#include "Aseprite.h"
#include "Define.h"


// ミリ秒
//
enum {
    kStarterMillisecondDone = 1500, 
};

// 点滅
//
enum {
    kStarterBlinkCycle = 500, 
    kStarterBlinkInterval = 250, 
};

// スターター
//
struct Starter {

    // アクタ
    struct Actor actor;

    // クランク
    float crank;
    
    // ミリ秒
    int millisecond;

    // 点滅
    int blink;

    // アニメーション
    struct AsepriteSpriteAnimation animation;

};

// 外部参照関数
//
extern void StarterInitialize(void);
extern void StarterLoad(void);
extern bool StarterIsDone(void);
