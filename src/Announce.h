// Announce.h - 告知
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"


// ミリ秒
//
enum {
    kAnnounceMillisecondFade = 750, 
    kAnnounceMillisecondIdle = 2500, 
};

// 矩形
//
enum {
    kAnnounceRectX = 0, 
    kAnnounceRectY = ((208 - 48) / 2), 
    kAnnounceRectWidth = 400, 
    kAnnounceRectHeight = 48, 
};

// 告知
//
struct Announce {

    // アクタ
    struct Actor actor;

    // テキスト
    const char *text;

    // ミリ秒
    int millisecondFade;
    int millisecondIdle;

    // 解放
    bool unload;

};

// 外部参照関数
//
extern void AnnounceInitialize(void);
extern void AnnounceLoad(const char *text);
extern bool AnnounceIsDone(void);
