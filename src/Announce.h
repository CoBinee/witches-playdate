// Announce.h - 告知
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"


// 待機
//
enum {
    kAnnounceIdleMillisecond = 2500, 
};

// フェード
//
enum {
    kAnnounceFadeMillisecond = 750, 
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

    // 待機
    int idle;

    // フェード
    int fade;

};

// 外部参照関数
//
extern void AnnounceInitialize(void);
extern void AnnounceLoad(const char *text, int idle);
extern bool AnnounceIsDone(void);
