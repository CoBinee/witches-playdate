// Notice.c - 通知
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Game.h"
#include "Notice.h"

// 内部関数
//
static void NoticeUnload(struct Notice *notice);
static void NoticeDraw(struct Notice *notice);
static void NoticeLoop(struct Notice *notice);

// 内部変数
//


// 通知を初期化する
//
void NoticeInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Notice) > kActorBlockSize) {
        playdate->system->error("%s: %d: notice actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Notice));
    }
}

// 通知を読み込む
//
void NoticeLoad(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Notice *notice = (struct Notice *)ActorLoad((ActorFunction)NoticeLoop, kGamePriorityNotice);
    if (notice == NULL) {
        playdate->system->error("%s: %d: notice actor is not loaded.", __FILE__, __LINE__);
    }

    // 通知の初期化
    {
        // 解放処理の設定
        ActorSetUnload(&notice->actor, (ActorFunction)NoticeUnload);

        // タグの設定
        ActorSetTag(&notice->actor, kGameTagNotice);
    }
}

// 通知を解放する
//
static void NoticeUnload(struct Notice *notice)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// 通知を描画する
//
static void NoticeDraw(struct Notice *notice)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // スプライトの描画
    {
        int offset = (int)(96.0f * (1.0f - cosf((float)M_PI * 0.5f * (float)notice->inout / (float)kNoticeInoutMillisecond)));
        AsepriteDrawSpriteAnimation(&notice->animations[kNoticeAnimationJump], -offset, 0, kDrawModeCopy, kBitmapUnflipped);
        AsepriteDrawSpriteAnimation(&notice->animations[kNoticeAnimationMove], offset, 0, kDrawModeCopy, kBitmapUnflipped);
    }
}

// 通知が待機する
//
static void NoticeLoop(struct Notice *notice)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (notice->actor.state == 0) {

        // インアウトの設定
        notice->inout = kNoticeInoutMillisecond;

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&notice->animations[kNoticeAnimationJump], "notice", "Left", true);
        AsepriteStartSpriteAnimation(&notice->animations[kNoticeAnimationMove], "notice", "Right", true);

        // 初期化の完了
        ++notice->actor.state;
    }

    // イン
    if (notice->actor.state == 1) {
        notice->inout -= IocsGetFrameMillisecond();
        if (notice->inout <= 0) {
            notice->inout = 0;
            AsepriteStartSpriteAnimation(&notice->animations[kNoticeAnimationJump], "notice", "Jump", false);
            AsepriteStartSpriteAnimation(&notice->animations[kNoticeAnimationMove], "notice", "Move", false);
            ++notice->actor.state;
        }

    // 待機
    } else if (notice->actor.state == 2) {
        if (AsepriteIsSpriteAnimationDone(&notice->animations[kNoticeAnimationJump])) {
            ++notice->actor.state;
        }

    // アウト
    } else {
        notice->inout += IocsGetFrameMillisecond();
        if (notice->inout >= kNoticeInoutMillisecond) {
            notice->inout = kNoticeInoutMillisecond;
            ActorUnload(&notice->actor);
            return;
        }
    }

    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&notice->animations[kNoticeAnimationJump]);
    AsepriteUpdateSpriteAnimation(&notice->animations[kNoticeAnimationMove]);

    // 描画処理の設定
    ActorSetDraw(&notice->actor, (ActorFunction)NoticeDraw, kGameOrderNotice);
}
