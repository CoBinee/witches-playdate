// Define.h - 定義
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"


// ベクタ
//
struct Vector {
    float x;
    float y;
};

// 矩形
//
struct Rect {
    int left;
    int top;
    int right;
    int bottom;
};

