// Templete.h - テンプレート
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"


// テンプレート関数
//
typedef void (*TempleteFunction)(void *game);

// テンプレート
//
struct Templete {

    // 処理関数
    TempleteFunction function;

    // 状態
    int state;
};

// スプライト
//
typedef enum {
    kTempleteSpriteNameNull = 0, 
    kTempleteSpriteNameSize, 
} TempleteSpriteName;

// プライオリティ
//
enum {
    kTempletePriorityNull = 0, 
};

// タグ
//
enum {
    kTempleteTagNull = 0, 
};

// 描画順
//
enum {
    kTempleteOrderNull = 0, 
};


// 外部参照関数
//
extern void TempleteUpdate(struct Templete *title);
