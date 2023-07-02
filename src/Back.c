// Back.c - 背景
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Aseprite.h"
#include "Game.h"
#include "Back.h"

// 内部関数
//
static void BackUnload(struct Back *back);
static void BackDraw(struct Back *back);
static void BackLoop(struct Back *back);
static bool BackSetLight(int x, int y, int color);

// 内部変数
//
static const char *backAnimationNames[kBackColorSize] = {
    "0", 
    "1", 
    "2", 
    "3", 
    "4", 
    "5", 
    "6", 
    "7", 
    "8", 
};


// 背景を初期化する
//
void BackInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Back) > kActorBlockSize) {
        playdate->system->error("%s: %d: back actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Back));
    }
}

// 背景を読み込む
//
void BackLoad(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Back *back = (struct Back *)ActorLoad((ActorFunction)BackLoop, kGamePriorityBack);
    if (back == NULL) {
        playdate->system->error("%s: %d: back actor is not loaded.", __FILE__, __LINE__);
    }

    // 背景の初期化
    {
        // 解放処理の設定
        ActorSetUnload(&back->actor, (ActorFunction)BackUnload);

        // タグの設定
        ActorSetTag(&back->actor, kGameTagBack);

        // 色の作成
        back->colors = playdate->system->realloc(NULL, kBackSizeX * kBackSizeY * sizeof (int));
        if (back->colors == NULL) {
            playdate->system->error("%s: %d: color array is not created.", __FILE__, __LINE__);
        }

        // 色の設定
        for (int y = 0; y < kBackSizeY; y++) {
            for (int x = 0; x < kBackSizeX; x++) {
                back->colors[y * kBackSizeX + x] = kBackColorDark;
            }
        }

        // ヘッドの設定
        for (int x = 0; x < kBackSizeX; x++) {
            back->heads[x] = kBackSizeY;
        }

        // 速度の設定
        for (int x = 0; x < kBackSizeX; x++) {
            back->speeds[x] = 0;
        }

        // スコアの設定
        back->scoreLight = 0;
        back->scoreDark = kBackSizeX * kBackSizeY * kBackColorDark;

        // アニメーションの作成
        back->animations = playdate->system->realloc(NULL, kBackSizeX * kBackSizeY * sizeof (struct AsepriteSpriteAnimation));
        if (back->animations == NULL) {
            playdate->system->error("%s: %d: sprite animation is not created.", __FILE__, __LINE__);
        }
    }
}

// 背景を解放する
//
static void BackUnload(struct Back *back)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 色の解放
    if (back->colors != NULL) {
        playdate->system->realloc(back->colors, 0);
    }

    // アニメーションの解放
    if (back->animations != NULL) {
        playdate->system->realloc(back->animations, 0);
    }
}

// 背景を描画する
//
static void BackDraw(struct Back *back)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // スプライトの描画
    for (int y = 0; y < kBackSizeY; y++) {
        for (int x = 0; x < kBackSizeX; x++) {
            AsepriteDrawSpriteAnimation(&back->animations[y * kBackSizeX + x], x * kBackSizePixel, y * kBackSizePixel, kDrawModeCopy, kBitmapUnflipped);
        }
    }
}

// 背景が待機する
//
static void BackLoop(struct Back *back)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (back->actor.state == 0) {

        // アニメーションの開始
        for (int y = 0; y < kBackSizeY; y++) {
            for (int x = 0; x < kBackSizeX; x++) {
                AsepriteStartSpriteAnimation(&back->animations[y * kBackSizeX + x], "back", backAnimationNames[back->colors[y * kBackSizeX + x]], false);
            }
        }

        // 初期化の完了
        ++back->actor.state;
    }

    // 闇化
    if (GameIsPlay()) {
        for (int x = 0; x < kBackSizeX; x++) {
            int y = back->heads[x];
            if (y < kBackSizeY) {
                ++back->speeds[x];
                if (back->speeds[x] >= kBackSpeed - (y & 0x01)) {   // 難易度調整
                    if (back->colors[y * kBackSizeX + x] < kBackColorDark) {
                        ++back->colors[y * kBackSizeX + x];
                        ++back->scoreDark;
                        --back->scoreLight;
                        AsepriteStartSpriteAnimation(&back->animations[y * kBackSizeX + x], "back", backAnimationNames[back->colors[y * kBackSizeX + x]], false);
                    } else {
                        ++back->heads[x];
                    }
                    back->speeds[x] = 0;
                }
            }
        }
    }

    // スプライトの更新
    for (int y = 0; y < kBackSizeY; y++) {
        for (int x = 0; x < kBackSizeX; x++) {
            AsepriteUpdateSpriteAnimation(&back->animations[y * kBackSizeX + x]);
        }
    }

    // 描画処理の設定
    ActorSetDraw(&back->actor, (ActorFunction)BackDraw, kGameOrderBack);
}

// 闇を明るくする
//
bool BackIsDarkThenLight(int x, int y)
{
    // 背景の取得
    struct Back *back = (struct Back *)ActorFindWithTag(kGameTagBack);
    if (back == NULL) {
        return false;
    }

    // 位置の取得
    x /= kBackSizePixel;
    y /= kBackSizePixel;

    // 闇を明るく
    bool result = false;
    if (BackSetLight(x + 0, y + 0, kBackColorDark)) {
        BackSetLight(x - 1, y + 0, kBackColorDark / 2);
        BackSetLight(x + 1, y + 0, kBackColorDark / 2);
        BackSetLight(x + 0, y - 1, kBackColorDark / 2);
        BackSetLight(x + 0, y + 1, kBackColorDark / 2);
        BackSetLight(x - 1, y - 1, kBackColorDark / 4);
        BackSetLight(x + 1, y - 1, kBackColorDark / 4);
        BackSetLight(x - 1, y + 1, kBackColorDark / 4);
        BackSetLight(x + 1, y + 1, kBackColorDark / 4);
        result = true;
    }
    if (0 <= x && x < kBackSizeX && 0 <= y && y < kBackSizeY) {
        if (back->colors[y * kBackSizeX + x] > kBackColorLight) {
            back->colors[y * kBackSizeX + x] = kBackColorLight;
            AsepriteStartSpriteAnimation(&back->animations[y * kBackSizeX + x], "back", backAnimationNames[back->colors[y * kBackSizeX + x]], false);
            if (x > 0) {

            }
        }

    }

    // 終了
    return result;
}
static bool BackSetLight(int x, int y, int color)
{
    // 背景の取得
    struct Back *back = (struct Back *)ActorFindWithTag(kGameTagBack);
    if (back == NULL) {
        return false;
    }

    // 明るくする
    bool result = false;
    int score = 0;
    if (0 <= x && x < kBackSizeX && 0 <= y && y < kBackSizeY) {
        if (back->colors[y * kBackSizeX + x] > 0) {
            if (back->colors[y * kBackSizeX + x] >= color) {
                score += color;
                back->colors[y * kBackSizeX + x] -= color;
            } else {
                score += back->colors[y * kBackSizeX + x];
                back->colors[y * kBackSizeX + x] = 0;
            }
            if (back->heads[x] > y) {
                back->heads[x] = y;
                back->speeds[x] = 0;
            }
            back->scoreLight += score;
            back->scoreDark -= score;
            AsepriteStartSpriteAnimation(&back->animations[y * kBackSizeX + x], "back", backAnimationNames[back->colors[y * kBackSizeX + x]], false);
            result = true;
        }
    }

    // 終了
    return result;
}

// スコアを取得する
//
int BackGetScoreLight(void)
{
    struct Back *back = (struct Back *)ActorFindWithTag(kGameTagBack);
    return back != NULL ? back->scoreLight : 0;
}
int BackGetScoreDark(void)
{
    struct Back *back = (struct Back *)ActorFindWithTag(kGameTagBack);
    return back != NULL ? back->scoreDark : 0;
}
