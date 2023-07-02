// Filter.c - フィルタ
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Filter.h"

// 内部関数
//

// 内部変数
//
static const LCDPattern filterPatterns[kFilterSize] = {
    {
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
    }, 
    {
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
        0b11111111, 0b01110111, 0b11111111, 0b11011101, 0b11111111, 0b01110111, 0b11111111, 0b11011101, 
    }, 
    {
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
        0b11011101, 0b01110111, 0b11011101, 0b01110111, 0b11011101, 0b01110111, 0b11011101, 0b01110111, 
    }, 
    {
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
        0b01110111, 0b10101010, 0b11011101, 0b10101010, 0b01110111, 0b10101010, 0b11011101, 0b10101010, 
    }, 
    {
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
        0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 
    }, 
    {
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
        0b10001000, 0b01010101, 0b00100010, 0b01010101, 0b10001000, 0b01010101, 0b00100010, 0b01010101, 
    }, 
    {
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
        0b10001000, 0b00100010, 0b10001000, 0b00100010, 0b10001000, 0b00100010, 0b10001000, 0b00100010, 
    }, 
    {
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
        0b00000000, 0b10001000, 0b00000000, 0b00100010, 0b00000000, 0b10001000, 0b00000000, 0b00100010, 
    }, 
    {
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 
    }, 
    {
        0b00000000, 0b10001000, 0b00000000, 0b00100010, 0b00000000, 0b10001000, 0b00000000, 0b00100010, 
        0b00000000, 0b10001000, 0b00000000, 0b00100010, 0b00000000, 0b10001000, 0b00000000, 0b00100010, 
    }, 
    {
        0b10001000, 0b00100010, 0b10001000, 0b00100010, 0b10001000, 0b00100010, 0b10001000, 0b00100010, 
        0b10001000, 0b00100010, 0b10001000, 0b00100010, 0b10001000, 0b00100010, 0b10001000, 0b00100010, 
    }, 
    {
        0b10001000, 0b01010101, 0b00100010, 0b01010101, 0b10001000, 0b01010101, 0b00100010, 0b01010101, 
        0b10001000, 0b01010101, 0b00100010, 0b01010101, 0b10001000, 0b01010101, 0b00100010, 0b01010101, 
    }, 
    {
        0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 
        0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 0b01010101, 0b10101010, 
    }, 
    {
        0b01110111, 0b10101010, 0b11011101, 0b10101010, 0b01110111, 0b10101010, 0b11011101, 0b10101010, 
        0b01110111, 0b10101010, 0b11011101, 0b10101010, 0b01110111, 0b10101010, 0b11011101, 0b10101010, 
    }, 
    {
        0b11011101, 0b01110111, 0b11011101, 0b01110111, 0b11011101, 0b01110111, 0b11011101, 0b01110111, 
        0b11011101, 0b01110111, 0b11011101, 0b01110111, 0b11011101, 0b01110111, 0b11011101, 0b01110111, 
    }, 
    {
        0b11111111, 0b01110111, 0b11111111, 0b11011101, 0b11111111, 0b01110111, 0b11111111, 0b11011101, 
        0b11111111, 0b01110111, 0b11111111, 0b11011101, 0b11111111, 0b01110111, 0b11111111, 0b11011101, 
    }, 
    {
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
        0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 0b11111111, 
    }, 
};


// フィルタの色を取得する
//
LCDColor FilterGetColor(int filter)
{
    return (LCDColor)filterPatterns[filter];
}
