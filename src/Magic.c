// Magic.c - 魔法
//

// 外部参照
//
#include <string.h>
#include <math.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Game.h"
#include "Back.h"
#include "Magic.h"

// 内部関数
//
static void MagicUnload(struct Magic *magic);
static void MagicDraw(struct Magic *magic);
static void MagicShot(struct Magic *magic);
static void MagicCalcRect(struct Magic *magic);

// 内部変数
//
static const char *magicAnimationNames[] = {
    "Idle0", 
    "Idle1", 
    "Idle2", 
    "Idle3", 
    "Idle4", 
    "Idle5", 
};


// 魔法を初期化する
//
void MagicInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Magic) > kActorBlockSize) {
        playdate->system->error("%s: %d: magic actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Magic));
    }
}

// 魔法を読み込む
//
void MagicLoad(float x, float y, float angle)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Magic *magic = (struct Magic *)ActorLoad((ActorFunction)MagicShot, kGamePriorityMagic);
    if (magic == NULL) {
        playdate->system->error("%s: %d: magic actor is not loaded.", __FILE__, __LINE__);
    }

    // 魔法の初期化
    {
        // 解放処理の設定
        ActorSetUnload(&magic->actor, (ActorFunction)MagicUnload);

        // タグの設定
        ActorSetTag(&magic->actor, kGameTagMagic);

        // 位置の設定
        magic->position.x = x;
        magic->position.y = y;

        // ベクトルの設定
        magic->vector.x = (float)kMagicSpeed * sinf(angle);
        magic->vector.y = (float)-kMagicSpeed * cosf(angle);

        // 回転の設定
        magic->rotation = (float)IocsGetRandomRange(NULL, 0, 359);

        // 矩形の計算
        MagicCalcRect(magic);
    }
}

// 魔法を解放する
//
static void MagicUnload(struct Magic *magic)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// 魔法を描画する
//
static void MagicDraw(struct Magic *magic)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // スプライトの描画
    AsepriteDrawRotatedSpriteAnimation(&magic->animation, (int)magic->position.x,  (int)magic->position.y, magic->rotation, 0.5f, 0.5f, 1.0f, 1.0f, kDrawModeXOR);
}

// 魔法が飛ぶ
//
static void MagicShot(struct Magic *magic)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (magic->actor.state == 0) {

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&magic->animation, "magic", magicAnimationNames[IocsGetRandomRange(NULL, 0, 5)], true);

        // 初期化の完了
        ++magic->actor.state;
    }

    // 解放の設定
    bool unload = false;

    // 移動
    for (int cycle = 0; cycle < kMagicCycle; cycle++) {
        magic->position.x += magic->vector.x;
        magic->position.y += magic->vector.y;
        if (BackIsDarkThenLight((int)magic->position.x, (int)magic->position.y)) {
            unload = true;
        }
    }
    if (magic->position.x <= (float)kMagciRangeLeft || magic->position.x >=(float)kMagicRangeRight || magic->position.y <= (float)kMagciRangeTop || magic->position.y >= (float)kMagicRangeBottom) {
        unload = true;
    }
    if (unload) {
        ActorUnload(&magic->actor);
        return;
    }

    // 矩形の計算
    MagicCalcRect(magic);

    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&magic->animation);

    // 描画処理の設定
    ActorSetDraw(&magic->actor, (ActorFunction)MagicDraw, kGameOrderMagic);
}

// 矩形を計算する
//
static void MagicCalcRect(struct Magic *magic)
{
    magic->rect.left = (int)magic->position.x + kMagicRectLeft;
    magic->rect.top = (int)magic->position.y + kMagicRectTop;
    magic->rect.right = (int)magic->position.x + kMagicRectRight;
    magic->rect.bottom = (int)magic->position.y + kMagicRectBottom;
}

