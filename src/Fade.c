// Fade.c - フェード
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Aseprite.h"
#include "Filter.h"
#include "Fade.h"

// 内部関数
//
static void FadeUnload(struct Fade *fade);
static void FadeDraw(struct Fade *fade);
static void FadeLoop(struct Fade *fade);

// 内部変数
//


// フェードを初期化する
//
void FadeInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Fade) > kActorBlockSize) {
        playdate->system->error("%s: %d: fade actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Fade));
    }
}

// フェードを読み込む
//
void FadeLoad(int type)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Fade *fade = (struct Fade *)ActorLoad((ActorFunction)FadeLoop, kActorPrioritySize - 1);
    if (fade == NULL) {
        playdate->system->error("%s: %d: fade actor is not loaded.", __FILE__, __LINE__);
    }

    // フェードの初期化
    {
        // 解放処理の設定
        ActorSetUnload(&fade->actor, (ActorFunction)FadeUnload);

        // タグの設定
        ActorSetTag(&fade->actor, kActorTagSize - 1);

        // 種類の設定
        fade->type = type;

        // フィルタの設定
        fade->filter = type == kFadeTypeIn ? kFilterBlack : kFilterNone;

        /*
        // アニメーションの作成
        fade->animations = playdate->system->realloc(NULL, kFadeSizeX * kFadeSizeY * sizeof (struct AsepriteSpriteAnimation));
        if (fade->animations == NULL) {
            playdate->system->error("%s: %d: sprite animation is not created.", __FILE__, __LINE__);
        }

        // アニメーションの開始
        {
            const char *name = type == kFadeIn ? "In" : "Out";
            for (int y = 0; y < kFadeSizeY; y++) {
                for (int x = 0; x < kFadeSizeX; x++) {
                    AsepriteStartSpriteAnimation(&fade->animations[y * kFadeSizeX + x], "fade", name, false);
                }
            }
        }
        */
    }
}

// フェードを解放する
//
static void FadeUnload(struct Fade *fade)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    /*
    // アニメーションの解放
    if (fade->animations != NULL) {
        playdate->system->realloc(fade->animations, 0);
    }
    */
}

// フェードを描画する
//
static void FadeDraw(struct Fade *fade)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // フィルタの描画
    {
        LCDColor color = FilterGetColor(fade->filter);
        playdate->graphics->setDrawMode(kDrawModeCopy);
        playdate->graphics->fillRect(0, 0, 400, 240, color);
    }

    /*
    // スプライトの描画
    for (int y = 0; y < kFadeSizeY; y++) {
        for (int x = 0; x < kFadeSizeX; x++) {
            AsepriteDrawSpriteAnimation(&fade->animations[y * kFadeSizeX + x], x * kFadeSizePixel, y * kFadeSizePixel, kDrawModeCopy, kBitmapUnflipped);
        }
    }
    */
}

// フェードを処理する
//
static void FadeLoop(struct Fade *fade)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (fade->actor.state == 0) {

        // 初期化の完了
        ++fade->actor.state;
    }

    // フィルタの更新
    if (fade->type == kFadeTypeIn) {
        if (fade->filter < kFilterNone) { 
            ++fade->filter;
        }
    } else {
        if (fade->filter > kFilterBlack) { 
            --fade->filter;
        }
    }

    /*
    // スプライトの更新
    for (int y = 0; y < kFadeSizeY; y++) {
        for (int x = 0; x < kFadeSizeX; x++) {
            AsepriteUpdateSpriteAnimation(&fade->animations[y * kFadeSizeX + x]);
        }
    }
    */

    // 描画処理の設定
    ActorSetDraw(&fade->actor, (ActorFunction)FadeDraw, kActorOrderFront);
}

// フェードを削除する
//
void FadeKill(void)
{
    struct Actor *actor = ActorFindWithTag(kActorTagSize - 1);
    if (actor != NULL) {
        ActorUnload(actor);
    }
}

// フェードが完了したかどうかを判定する
//
bool FadeIsDone(void)
{
    bool done = false;
    struct Fade *fade = (struct Fade *)ActorFindWithTag(kActorTagSize - 1);
    if (fade != NULL) {
        if (fade->type == kFadeTypeIn) {
            if (fade->filter == kFilterNone) {
                done = true;
            }
        } else {
            if (fade->filter == kFilterBlack) {
                done = true;
            }
        }
    }
    return done;
    // return fade != NULL && AsepriteIsSpriteAnimationDone(&fade->animations[0]) ? true : false;
}
