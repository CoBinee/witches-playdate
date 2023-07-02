// Logo.c - ロゴ
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Title.h"
#include "Crystal.h"
#include "Logo.h"

// 内部関数
//
static void LogoUnload(struct Logo *logo);
static void LogoDraw(struct Logo *logo);
static void LogoLoop(struct Logo *logo);

// 内部変数
//


// ロゴを初期化する
//
void LogoInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Logo) > kActorBlockSize) {
        playdate->system->error("%s: %d: logo actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Logo));
    }
}

// ロゴを読み込む
//
void LogoLoad(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Logo *logo = (struct Logo *)ActorLoad((ActorFunction)LogoLoop, kTitlePriorityLogo);
    if (logo == NULL) {
        playdate->system->error("%s: %d: logo actor is not loaded.", __FILE__, __LINE__);
    }

    // ロゴの初期化
    {
        // 解放処理の設定
        ActorSetUnload(&logo->actor, (ActorFunction)LogoUnload);

        // タグの設定
        ActorSetTag(&logo->actor, kTitleTagLogo);

        // 結晶の設定
        logo->crystal = 0;
    }
}

// ロゴを解放する
//
static void LogoUnload(struct Logo *logo)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// ロゴを描画する
//
static void LogoDraw(struct Logo *logo)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // スプライトの描画
    AsepriteDrawSpriteAnimation(&logo->animation, 0, 0, kDrawModeCopy, kBitmapUnflipped);
}

// ロゴが待機する
//
static void LogoLoop(struct Logo *logo)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (logo->actor.state == 0) {

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&logo->animation, "logo", "Idle", true);

        // 初期化の完了
        ++logo->actor.state;
    }

    // 結晶
    {
        logo->crystal -= IocsGetFrameMillisecond();
        if (logo->crystal <= 0) {
            CrystalLoad(kTitlePriorityCrystal, kTitleTagCrystal, kTitleOrderCrystal);
            logo->crystal = IocsGetRandomRange(NULL, 1500, 3000);
        }
    }

    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&logo->animation);

    // 描画処理の設定
    ActorSetDraw(&logo->actor, (ActorFunction)LogoDraw, kTitleOrderLogo);
}
