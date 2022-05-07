// Status.c - ステータス
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Game.h"
#include "Back.h"
#include "Status.h"

// 内部関数
//
static void StatusUnload(struct Status *status);
static void StatusDraw(struct Status *status);
static void StatusLoop(struct Status *status);

// 内部変数
//


// ステータスを初期化する
//
void StatusInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Status) > kActorBlockSize) {
        playdate->system->error("%s: %d: status actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Status));
    }
}

// ステータスを読み込む
//
void StatusLoad(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Status *status = (struct Status *)ActorLoad((ActorFunction)StatusLoop, kGamePriorityStatus);
    if (status == NULL) {
        playdate->system->error("%s: %d: status actor is not loaded.", __FILE__, __LINE__);
    }

    // ステータスの初期化
    {
        // 解放処理の設定
        ActorSetUnload(&status->actor, (ActorFunction)StatusUnload);

        // タグの設定
        ActorSetTag(&status->actor, kGameTagStatus);
    }
}

// ステータスを解放する
//
static void StatusUnload(struct Status *status)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// ステータスを描画する
//
static void StatusDraw(struct Status *status)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // スプライトの描画
    AsepriteDrawSpriteAnimation(&status->animations[kStatusAnimationBack], 0, 200, kDrawModeCopy, kBitmapUnflipped);
    AsepriteDrawRotatedSpriteAnimation(&status->animations[kStatusAnimationYou], 20, 238, 0.0f, 0.5f, 1.0f, 1.0f, 1.0f, kDrawModeCopy);
    AsepriteDrawRotatedSpriteAnimation(&status->animations[kStatusAnimationRival], 380, 238, 0.0f, 0.5f, 1.0f, -1.0f, 1.0f, kDrawModeCopy);

    // タイムの表示
    {
        char *text;
        playdate->system->formatString(&text, "%d", GameGetViewTime());
        IocsSetFont(kIocsFontGame);
        playdate->graphics->setDrawMode(kDrawModeFillWhite);
        playdate->graphics->drawText(text, strlen(text), kUTF8Encoding, 232 - IocsGetTextWidth(kIocsFontGame, text), 224);
        playdate->system->realloc(text, 0);
    }

    // スコアの描画
    int light = BackGetScoreLight();
    int dark = BackGetScoreDark();
    {
        char *text;
        playdate->system->formatString(&text, "%d", light);
        IocsSetFont(kIocsFontGame);
        playdate->graphics->setDrawMode(kDrawModeFillWhite);
        playdate->graphics->drawText(text, strlen(text), kUTF8Encoding, 40, 224);
        playdate->system->realloc(text, 0);
    }
    {
        char *text;
        playdate->system->formatString(&text, "%d", dark);
        IocsSetFont(kIocsFontGame);
        playdate->graphics->setDrawMode(kDrawModeFillWhite);
        playdate->graphics->drawText(text, strlen(text), kUTF8Encoding, 400 - 40 - IocsGetTextWidth(kIocsFontGame, text), 224);
        playdate->system->realloc(text, 0);
    }
    {
        int width = 304 * light / (light + dark);
        if (width == 0 && light > 0) {
            width = 1;
        }
        playdate->graphics->setDrawMode(kDrawModeCopy);
        playdate->graphics->fillRect(48, 210, width, 6, kColorWhite);
    }
}

// ステータスが待機する
//
static void StatusLoop(struct Status *status)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (status->actor.state == 0) {

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&status->animations[kStatusAnimationBack], "status", "Idle", false);
        AsepriteStartSpriteAnimation(&status->animations[kStatusAnimationYou], "you", "Idle", true);
        AsepriteStartSpriteAnimation(&status->animations[kStatusAnimationRival], "rival", "Idle", true);

        // 初期化の完了
        ++status->actor.state;
    }

    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&status->animations[kStatusAnimationBack]);
    AsepriteUpdateSpriteAnimation(&status->animations[kStatusAnimationYou]);
    AsepriteUpdateSpriteAnimation(&status->animations[kStatusAnimationRival]);

    // 描画処理の設定
    ActorSetDraw(&status->actor, (ActorFunction)StatusDraw, kGameOrderStatus);
}

