// Templete.h - テンプレート
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Actor.h"


// テンプレート
//
struct Templete {

    // アクタ
    struct Actor actor;

};

// 外部参照関数
//
extern void TempleteInitialize(void);
extern void TempleteLoad(void);
