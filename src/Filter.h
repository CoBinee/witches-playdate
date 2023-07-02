// Filter.h - フィルタ
//
#pragma once

// 外部参照
//
#include <stdbool.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Define.h"


// フィルタ
//
enum {
    kFilterBlack = 0, 
    kFilterNone = 8, 
    kFilterWhite = 16, 
    kFilterSize, 
};

// 外部参照関数
//
extern LCDColor FilterGetColor(int filter);

