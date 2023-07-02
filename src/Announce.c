// Announce.c - 告知
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Game.h"
#include "Announce.h"

// 内部関数
//
static void AnnounceUnload(struct Announce *announce);
static void AnnounceDraw(struct Announce *announce);
static void AnnounceLoop(struct Announce *announce);

// 内部変数
//


// 告知を初期化する
//
void AnnounceInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Announce) > kActorBlockSize) {
        playdate->system->error("%s: %d: announce actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Announce));
    }
}

// 告知を読み込む
//
void AnnounceLoad(const char *text, int idle)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Announce *announce = (struct Announce *)ActorLoad((ActorFunction)AnnounceLoop, kGamePriorityAnnounce);
    if (announce == NULL) {
        playdate->system->error("%s: %d: announce actor is not loaded.", __FILE__, __LINE__);
    }

    // 告知の初期化
    {
        // 解放処理の設定
        ActorSetUnload(&announce->actor, (ActorFunction)AnnounceUnload);

        // タグの設定
        ActorSetTag(&announce->actor, kGameTagAnnounce);

        // テキストの設定
        announce->text = text;

        // 待機の設定
        announce->idle = idle;
    }
}

// 告知を解放する
//
static void AnnounceUnload(struct Announce *announce)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// 告知を描画する
//
static void AnnounceDraw(struct Announce *announce)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // クリップの設定
    {
        int width = kAnnounceRectWidth * (kAnnounceFadeMillisecond - announce->fade) / kAnnounceFadeMillisecond;
        playdate->graphics->setClipRect((kAnnounceRectWidth - width) / 2 + kAnnounceRectX, kAnnounceRectY, width, kAnnounceRectHeight);
    }

    // 帯の描画
    {
        playdate->graphics->setDrawMode(kDrawModeCopy);
        playdate->graphics->fillRect(kAnnounceRectX, kAnnounceRectY, kAnnounceRectWidth, kAnnounceRectHeight, kColorBlack);
    }

    // テキストの描画
    {
        IocsSetFont(kIocsFontGame);
        playdate->graphics->setDrawMode(kDrawModeFillWhite);
        playdate->graphics->drawText(announce->text, strlen(announce->text), kUTF8Encoding, (kAnnounceRectWidth - IocsGetTextWidth(kIocsFontGame, announce->text)) / 2 + kAnnounceRectX, (kAnnounceRectHeight - IocsGetFontHeight(kIocsFontGame)) / 2 + kAnnounceRectY);
    }

    // クリップの削除
    playdate->graphics->clearClipRect();
}

// 告知が待機する
//
static void AnnounceLoop(struct Announce *announce)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (announce->actor.state == 0) {

        // フェードの設定
        announce->fade = kAnnounceFadeMillisecond;

        // 初期化の完了
        ++announce->actor.state;
    }

    // 待機とフェードの更新
    {
        int frame = IocsGetFrameMillisecond();
        if (announce->idle > 0) {
            if (announce->fade > 0) {
                announce->fade -= frame;
                if (announce->fade < 0) {
                    announce->fade = 0;
                }
            } else {
                announce->idle -= frame;
                if (announce->idle < 0) {
                    announce->idle = 0;
                }
            }
        } else {
            if (announce->fade < kAnnounceFadeMillisecond) {
                announce->fade += frame;
                if (announce->fade >= kAnnounceFadeMillisecond) {
                    announce->fade = kAnnounceFadeMillisecond;
                    ActorUnload(&announce->actor);
                    return;
                }
            }
        }
    }

    // 描画処理の設定
    ActorSetDraw(&announce->actor, (ActorFunction)AnnounceDraw, kGameOrderAnnounce);
}

// 告知が完了したかどうかを判定する
//
bool AnnounceIsDone(void)
{
    return ActorFindWithTag(kGameTagAnnounce) == NULL ? true : false;
}


