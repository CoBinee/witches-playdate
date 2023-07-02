// Crystal.c - 結晶
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Game.h"
#include "Crystal.h"

// 内部関数
//
static void CrystalUnload(struct Crystal *crystal);
static void CrystalDraw(struct Crystal *crystal);
static void CrystalFall(struct Crystal *crystal);
static void CrystalBomb(struct Crystal *crystal);
static void CrystalCalcRect(struct Crystal *crystal);

// 内部変数
//


// 結晶を初期化する
//
void CrystalInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Crystal) > kActorBlockSize) {
        playdate->system->error("%s: %d: crystal actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Crystal));
    }
}

// 結晶を読み込む
//
void CrystalLoad(int priority, int tag, int order)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Crystal *crystal = (struct Crystal *)ActorLoad((ActorFunction)CrystalFall, priority);
    if (crystal == NULL) {
        playdate->system->error("%s: %d: crystal actor is not loaded.", __FILE__, __LINE__);
    }

    // 結晶の初期化
    {
        // 解放処理の設定
        ActorSetUnload(&crystal->actor, (ActorFunction)CrystalUnload);

        // タグの設定
        ActorSetTag(&crystal->actor, tag);

        // 位置の設定
        crystal->position.x = (float)IocsGetRandomRange(NULL, kCrystalRangeLeft, kCrystalRangeRight);
        crystal->position.y = (float)kCrystalRangeTop;

        // 速度の設定
        crystal->speed = (float)IocsGetRandomRange(NULL, 5, 9) / 10.0f;

        // 描画順の設定
        crystal->order = order;

        // 矩形の計算
        CrystalCalcRect(crystal);
    }
}

// 結晶を解放する
//
static void CrystalUnload(struct Crystal *crystal)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// 結晶を描画する
//
static void CrystalDraw(struct Crystal *crystal)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // スプライトの描画
    AsepriteDrawRotatedSpriteAnimation(&crystal->animation, (int)crystal->position.x,  (int)crystal->position.y, 0.0f, 0.5f, 0.5f, 1.0f, 1.0f, kDrawModeXOR);
}

// 結晶が落下する
//
static void CrystalFall(struct Crystal *crystal)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (crystal->actor.state == 0) {

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&crystal->animation, "crystal", "Idle", true);

        // 初期化の完了
        ++crystal->actor.state;
    }

    // 移動
    crystal->position.y += crystal->speed;
    if (crystal->position.y >= (float)kCrystalRangeBottom) {
        ActorUnload(&crystal->actor);
        return;
    }

    // 矩形の計算
    CrystalCalcRect(crystal);

    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&crystal->animation);

    // 描画処理の設定
    ActorSetDraw(&crystal->actor, (ActorFunction)CrystalDraw, crystal->order);
}

// 結晶が爆発する
//
static void CrystalBomb(struct Crystal *crystal)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (crystal->actor.state == 0) {

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&crystal->animation, "crystal", "Bomb", false);

        // 初期化の完了
        ++crystal->actor.state;
    }

    // 矩形の計算
    CrystalCalcRect(crystal);

    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&crystal->animation);

    // 描画処理の設定
    ActorSetDraw(&crystal->actor, (ActorFunction)CrystalDraw, crystal->order);

    // アニメーションの完了
    if (AsepriteIsSpriteAnimationDone(&crystal->animation)) {
        ActorUnload(&crystal->actor);
    }
}

// 矩形を計算する
//
static void CrystalCalcRect(struct Crystal *crystal)
{
    crystal->rect.left = (int)crystal->position.x + kCrystalRectLeft;
    crystal->rect.top = (int)crystal->position.y + kCrystalRectTop;
    crystal->rect.right = (int)crystal->position.x + kCrystalRectRight;
    crystal->rect.bottom = (int)crystal->position.y + kCrystalRectBottom;
}

// 結晶とヒットしたかどうかを判定する
//
int CrystalIsHit(struct Rect *rect, int tag)
{
    int result = 0;
    struct Crystal *crystal = (struct Crystal *)ActorFindWithTag(tag);
    while (crystal != NULL) {
        if (crystal->speed <= 0.0f || rect->left > crystal->rect.right || rect->right < crystal->rect.left || rect->top > crystal->rect.bottom || rect->bottom < crystal->rect.top) {
            crystal = (struct Crystal *)ActorNextWithTag(&crystal->actor);
        } else {
            result = (rect->left + rect->right) / 2 < (int)crystal->position.x ? -1 : 1;
            crystal = NULL;
        }
    }
    if (result != 0) {
        IocsPlayAudioEffect(kGameAudioEffectHit, 1);
    }
    return result;
}
int CrystalIsBomb(struct Rect *rect, int tag)
{
    int result = 0;
    struct Crystal *crystal = (struct Crystal *)ActorFindWithTag(tag);
    while (crystal != NULL) {
        if (crystal->speed <= 0.0f || rect->left > crystal->rect.right || rect->right < crystal->rect.left || rect->top > crystal->rect.bottom || rect->bottom < crystal->rect.top) {
            crystal = (struct Crystal *)ActorNextWithTag(&crystal->actor);
        } else {
            result = (rect->left + rect->right) / 2 < (int)crystal->position.x ? -1 : 1;
            crystal->speed = 0.0f;
            ActorTransition(&crystal->actor, (ActorFunction)CrystalBomb);
            crystal = NULL;
        }
    }
    return result;
}


