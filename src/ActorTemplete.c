// Templete.c - テンプレート
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Game.h"
#include "ActorTemplete.h"

// 内部関数
//
static void TempleteUnload(struct Templete *templete);
static void TempleteDraw(struct Templete *templete);
static void TempleteLoop(struct Templete *templete);

// 内部変数
//


// テンプレートを初期化する
//
void TempleteInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Templete) > kActorBlockSize) {
        playdate->system->error("%s: %d: templete actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Templete));
    }
}

// テンプレートを読み込む
//
void TempleteLoad(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Templete *templete = (struct Templete *)ActorLoad((ActorFunction)TempleteLoop, kGamePriorityTemplete);
    if (templete == NULL) {
        playdate->system->error("%s: %d: templete actor is not loaded.", __FILE__, __LINE__);
    }

    // テンプレートの初期化
    {
        // 解放処理の設定
        ActorSetUnload(&templete->actor, (ActorFunction)TempleteUnload);

        // タグの設定
        ActorSetTag(&templete->actor, kGameTagTemplete);
    }
}

// テンプレートを解放する
//
static void TempleteUnload(struct Templete *templete)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// テンプレートを描画する
//
static void TempleteDraw(struct Templete *templete)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// テンプレートが待機する
//
static void TempleteLoop(struct Templete *templete)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (templete->actor.state == 0) {

        // 初期化の完了
        ++templete->actor.state;
    }

    // 描画処理の設定
    ActorSetDraw(&templete->actor, (ActorFunction)TempleteDraw, kGameOrderTemplete);
}

