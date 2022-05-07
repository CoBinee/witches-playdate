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
void AnnounceLoad(const char *text)
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

        // 解放の設定
        announce->unload = false;
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
        int width = kAnnounceRectWidth * (kAnnounceMillisecondFade - announce->millisecondFade) / kAnnounceMillisecondFade;
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

        // ミリ秒の設定
        announce->millisecondFade = kAnnounceMillisecondFade;
        announce->millisecondIdle = kAnnounceMillisecondIdle;

        // 初期化の完了
        ++announce->actor.state;
    }

    // ミリ秒の更新
    {
        int frame = IocsGetFrameMillisecond();
        if (announce->millisecondIdle > 0) {
            if (announce->millisecondFade > 0) {
                announce->millisecondFade -= frame;
                if (announce->millisecondFade < 0) {
                    announce->millisecondFade = 0;
                }
            } else {
                announce->millisecondIdle -= frame;
                if (announce->millisecondIdle < 0) {
                    announce->millisecondIdle = 0;
                }
            }
        } else {
            if (announce->millisecondFade < kAnnounceMillisecondFade) {
                announce->millisecondFade += frame;
                if (announce->millisecondFade >= kAnnounceMillisecondFade) {
                    announce->millisecondFade = kAnnounceMillisecondFade;
                    announce->unload = true;
                }
            }
        }
    }

    // 描画処理の設定
    ActorSetDraw(&announce->actor, (ActorFunction)AnnounceDraw, kGameOrderAnnounce);

    // 解放
    if (announce->unload) {
        ActorUnload(&announce->actor);
    }
}

// 告知が完了したかどうかを判定する
//
bool AnnounceIsDone(void)
{
    return ActorFindWithTag(kGameTagAnnounce) == NULL ? true : false;
}


