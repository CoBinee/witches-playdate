// Starter.c - スターター
//

// 外部参照
//
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Application.h"
#include "Title.h"
#include "Starter.h"

// 内部関数
//
static void StarterUnload(struct Starter *starter);
static void StarterDraw(struct Starter *starter);
static void StarterLoop(struct Starter *starter);

// 内部変数
//


// スターターを初期化する
//
void StarterInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Starter) > kActorBlockSize) {
        playdate->system->error("%s: %d: starter actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Starter));
    }
}

// スターターを読み込む
//
void StarterLoad(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Starter *starter = (struct Starter *)ActorLoad((ActorFunction)StarterLoop, kTitlePriorityStarter);
    if (starter == NULL) {
        playdate->system->error("%s: %d: starter actor is not loaded.", __FILE__, __LINE__);
    }

    // スターターの初期化
    {
        // 解放処理の設定
        ActorSetUnload(&starter->actor, (ActorFunction)StarterUnload);

        // タグの設定
        ActorSetTag(&starter->actor, kTitleTagStarter);

        // クランクの初期化
        starter->crank = 0.0f;

        // ミリ秒の設定
        starter->millisecond = 0;

        // 点滅の設定
        starter->blink = 0;
    }
}

// スターターを解放する
//
static void StarterUnload(struct Starter *starter)
{
    ;
}

// スターターを描画する
//
static void StarterDraw(struct Starter *starter)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // スプライトの描画
    if (starter->blink < kStarterBlinkCycle) {
        AsepriteDrawSpriteAnimation(&starter->animation, 0, 0, kDrawModeCopy, kBitmapUnflipped);
    }
}

// スターターが待機する
//
static void StarterLoop(struct Starter *starter)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (starter->actor.state == 0) {

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&starter->animation, "notice", "Start", true);

        // 初期化の完了
        ++starter->actor.state;
    }

    // クランクの入力
    if (starter->actor.state == 1) {
        starter->crank += IocsGetCrankChange();
        if (starter->crank <= -360.0f || starter->crank >= 360.0f) {
            IocsPlayAudioSystem(kIocsAudioSystemSamplePipo, 1);
            ++starter->actor.state;
        }

        // スプライトの更新
        AsepriteUpdateSpriteAnimation(&starter->animation);

    // クランクの完了
    } else {

        // 点滅の更新
        starter->blink += kStarterBlinkInterval;
        if (starter->blink >= 2 * kStarterBlinkCycle) {
            starter->blink -= 2 * kStarterBlinkCycle;
        }
        
        // ミリ秒の更新
        starter->millisecond += IocsGetFrameMillisecond();
        if (starter->millisecond > kStarterMillisecondDone) {
            ActorUnload(&starter->actor);
            return;
        }
    }

    // 描画処理の設定
    ActorSetDraw(&starter->actor, (ActorFunction)StarterDraw, kTitleOrderStarter);
}

// スターターが完了したかどうかを判定する
//
bool StarterIsDone(void)
{
    return ActorFindWithTag(kTitleTagStarter) == NULL ? true : false;
}

