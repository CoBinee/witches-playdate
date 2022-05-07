// Logo.h - ロゴ
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"
#include "Aseprite.h"
#include "Define.h"


// ロゴ
//
struct Logo {

    // アクタ
    struct Actor actor;

    // 結晶
    int crystal;

    // アニメーション
    struct AsepriteSpriteAnimation animation;

};

// 外部参照関数
//
extern void LogoInitialize(void);
extern void LogoLoad(void);
