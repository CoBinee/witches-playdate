// Cat.c - 猫
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Game.h"
#include "Back.h"
#include "You.h"
#include "Cat.h"

// 内部関数
//
static void CatUnload(struct Cat *cat);
static void CatDraw(struct Cat *cat);
static void CatStay(struct Cat *cat);
static void CatWalk(struct Cat *cat);
static void CatCalcRect(struct Cat *cat);

// 内部変数
//


// 猫を初期化する
//
void CatInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct Cat) > kActorBlockSize) {
        playdate->system->error("%s: %d: cat actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct Cat));
    }
}

// 猫を読み込む
//
void CatLoad(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct Cat *cat = (struct Cat *)ActorLoad((ActorFunction)CatStay, kGamePriorityCat);
    if (cat == NULL) {
        playdate->system->error("%s: %d: cat actor is not loaded.", __FILE__, __LINE__);
    }

    // 猫の初期化
    {
        // 解放処理の設定
        ActorSetUnload(&cat->actor, (ActorFunction)CatUnload);

        // タグの設定
        ActorSetTag(&cat->actor, kGameTagCat);

        // 位置の設定
        cat->position.x = (float)((kCatRangeRight - kCatRangeLeft) * 3 / 5);
        cat->position.y = (float)kCatRangeBottom;

        // 向きの設定
        cat->direction = 1.0f;

        // 速度の設定
        cat->speed = 0.5f;

        // 矩形の計算
        CatCalcRect(cat);
    }
}

// 猫を解放する
//
static void CatUnload(struct Cat *cat)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// 猫を描画する
//
static void CatDraw(struct Cat *cat)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // スプライトの描画
    AsepriteDrawRotatedSpriteAnimation(&cat->animation, (int)cat->position.x,  (int)cat->position.y, 0.0f, 0.5f, 1.0f, cat->direction, 1.0f, kDrawModeCopy);
}

// 猫が待機する
//
static void CatStay(struct Cat *cat)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (cat->actor.state == 0) {

        // ミリ秒の設定
        {
            int maximum = BackGetScoreLight() >= BackGetScoreDark() ? 5000 : 10000;
            cat->millisecond = IocsGetRandomRange(NULL, 3000, maximum);
        }

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&cat->animation, "cat", "Idle", true);

        // 初期化の完了
        ++cat->actor.state;
    }

    // ミリ秒の更新
    if (GameIsPlay()) {
        cat->millisecond -= IocsGetFrameMillisecond();
        if (cat->millisecond <= 0) {
            ActorTransition(&cat->actor, (ActorFunction)CatWalk);
        }
    }

    // 矩形の計算
    CatCalcRect(cat);

    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&cat->animation);

    // 描画処理の設定
    ActorSetDraw(&cat->actor, (ActorFunction)CatDraw, kGameOrderCat);
}

// 猫が歩く
//
static void CatWalk(struct Cat *cat)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (cat->actor.state == 0) {

        // 向きの設定
        cat->direction = (float)((IocsGetRandomNumber(NULL) & 0x02) - 1);
        if (BackGetScoreLight() >= BackGetScoreDark()) {
            if (IocsGetRandomRange(NULL, 0, 3) != 0) {
                struct Vector *yp = YouGetPosition();
                if (yp != NULL) {
                    if (yp->x < cat->position.x) {
                        cat->direction = -1;
                    } else if (yp->x > cat->position.x) {
                        cat->direction = 1;
                    }
                }
            }
        }

        // ミリ秒の設定
        cat->millisecond = IocsGetRandomRange(NULL, 1500, 10000);

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&cat->animation, "cat", "Walk", true);

        // 初期化の完了
        ++cat->actor.state;
    }

    // 移動
    cat->position.x += cat->speed * cat->direction;
    if (cat->position.x <= (float)kCatRangeLeft) {
        cat->position.x = (float)kCatRangeLeft;
        cat->direction = -cat->direction;
    } else if (cat->position.x >= (float)kCatRangeRight) {
        cat->position.x = (float)kCatRangeRight;
        cat->direction = -cat->direction;
    }

    // ミリ秒の更新
    cat->millisecond -= IocsGetFrameMillisecond();
    if (cat->millisecond <= 0) {
        if (IocsGetRandomRange(NULL, 0, 3) != 0) {
            ActorTransition(&cat->actor, (ActorFunction)CatStay);
        } else {
            ActorTransition(&cat->actor, (ActorFunction)CatWalk);
        }
    }

    // 矩形の計算
    CatCalcRect(cat);

    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&cat->animation);

    // 描画処理の設定
    ActorSetDraw(&cat->actor, (ActorFunction)CatDraw, kGameOrderCat);
}

// 矩形を計算する
//
static void CatCalcRect(struct Cat *cat)
{
    cat->rect.left = (int)cat->position.x + kCatRectLeft;
    cat->rect.top = (int)cat->position.y + kCatRectTop;
    cat->rect.right = (int)cat->position.x + kCatRectRight;
    cat->rect.bottom = (int)cat->position.y + kCatRectBottom;
}

// 猫とヒットしたかどうかを判定する
//
int CatIsHit(struct Rect *rect)
{
    int result = 0;
    struct Cat *cat = (struct Cat *)ActorFindWithTag(kGameTagCat);
    while (cat != NULL) {
        if (rect->left > cat->rect.right || rect->right < cat->rect.left || rect->top > cat->rect.bottom || rect->bottom < cat->rect.top) {
            cat = (struct Cat *)ActorNextWithTag(&cat->actor);
        } else {
            result = (rect->left + rect->right) / 2 < (int)cat->position.x ? -1 : 1;
            cat = NULL;
        }
    }
    if (result != 0) {
        IocsPlayAudioEffect(kGameAudioEffectCat, 1);
    }
    return result;
}

