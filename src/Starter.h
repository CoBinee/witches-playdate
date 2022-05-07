// Starter.h - スターター
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"
#include "Define.h"


// 点滅
//
enum {
    kStarterBlinkCycle = 500, 
};

// ミリ秒
//
enum {
    kStarterMillisecondDone = 1500, 
};

// スターター
//
struct Starter {

    // アクタ
    struct Actor actor;

    // 点滅
    int blink;

    // ミリ秒
    int millisecond;

    // クランク
    float crank;

};

// 外部参照関数
//
extern void StarterInitialize(void);
extern void StarterLoad(void);
extern bool StarterIsDone(void);
