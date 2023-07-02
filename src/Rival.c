// Rival.c - ライバル
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Game.h"
#include "Rival.h"
#include "Crystal.h"

// 内部関数
//
static void RivalUnload(struct Rival *rival);
static void RivalDraw(struct Rival *rival);
static void RivalFly(struct Rival *rival);

// 内部変数
//


// ライバルを初期化する
//
void RivalInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Rival) > kActorBlockSize) {
        playdate->system->error("%s: %d: rival actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Rival));
    }
}

// ライバルを読み込む
//
void RivalLoad(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Rival *rival = (struct Rival *)ActorLoad((ActorFunction)RivalFly, kGamePriorityRival);
    if (rival == NULL) {
        playdate->system->error("%s: %d: rival actor is not loaded.", __FILE__, __LINE__);
    }

    // ライバルの初期化
    {
        // 解放処理の設定
        ActorSetUnload(&rival->actor, (ActorFunction)RivalUnload);

        // タグの設定
        ActorSetTag(&rival->actor, kGameTagRival);

        rival->position.x = (float)((kRivalRangeRight - kRivalRangeLeft) * 3 / 4);
        rival->position.y = (float)kRivalRangeMiddle;

        // 向きの設定
        rival->direction = -1.0f;

        // 移動の設定
        rival->moveSpeed = 1.5f;

        // カーブの設定
        rival->curveMaximum = 2.0f;
        rival->curveSpeed = rival->curveMaximum;
        rival->curveAccel = rival->curveMaximum / 16.0f;

        // 結晶の設定
        rival->crystal = 6500;
    }
}

// ライバルを解放する
//
static void RivalUnload(struct Rival *rival)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// ライバルを描画する
//
static void RivalDraw(struct Rival *rival)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // スプライトの描画
    AsepriteDrawRotatedSpriteAnimation(&rival->animation, (int)rival->position.x,  (int)rival->position.y, 0.0f, 0.5f, 0.5f, rival->direction, 1.0f, kDrawModeCopy);
}

// ライバルが飛ぶ
//
static void RivalFly(struct Rival *rival)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (rival->actor.state == 0) {

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&rival->animation, "rival", "Idle", true);

        // 初期化の完了
        ++rival->actor.state;
    }

    // 移動
    if (rival->position.x <= (float)kRivalRangeLeft || rival->position.x >= (float)kRivalRangeRight) {
        rival->direction = -rival->direction;
    }
    rival->position.x += rival->moveSpeed * rival->direction;

    // カーブ
    if (rival->curveSpeed <= -rival->curveMaximum || rival->curveSpeed >= rival->curveMaximum) {
        rival->curveAccel = -rival->curveAccel;
    }
    rival->curveSpeed += rival->curveAccel;
    rival->position.y += rival->curveSpeed;

    // 結晶
    if (GameIsPlay()) {
        rival->crystal -= IocsGetFrameMillisecond();
        if (rival->crystal <= 0) {
            CrystalLoad(kGamePriorityCrystal, kGameTagCrystal, kGameOrderCrystal);
            rival->crystal = GameGetRealTime() + 750;
        }
    }

    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&rival->animation);

    // 描画処理の設定
    ActorSetDraw(&rival->actor, (ActorFunction)RivalDraw, kGameOrderRival);
}

