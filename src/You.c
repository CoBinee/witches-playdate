// You.c - あなた
//

// 外部参照
//
#include <string.h>
#include <math.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Actor.h"
#include "Game.h"
#include "You.h"
#include "Magic.h"

// 内部関数
//
static void YouUnload(struct You *you);
static void YouDraw(struct You *you);
static void YouIdle(struct You *you);
static void YouWalk(struct You *you);
static void YouJump(struct You *you);
static void YouMoveLr(struct You *you);
static void YouCalcPosition(struct You *you);
static void YouFire(struct You *you);
static void YouCalcRect(struct You *you);

// 内部変数
//


// あなたを初期化する
//
void YouInitialize(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの確認
    if (sizeof (struct You) > kActorBlockSize) {
        playdate->system->error("%s: %d: you actor size is over: %d bytes.", __FILE__, __LINE__, sizeof (struct You));
    }
}

// あなたを読み込む
//
void YouLoad(void)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // アクタの登録
    struct You *you = (struct You *)ActorLoad((ActorFunction)YouIdle, kGamePriorityYou);
    if (you == NULL) {
        playdate->system->error("%s: %d: you actor is not loaded.", __FILE__, __LINE__);
    }

    // あなたの初期化
    {
        // 解放処理の設定
        ActorSetUnload(&you->actor, (ActorFunction)YouUnload);

        // タグの設定
        ActorSetTag(&you->actor, kGameTagYou);

        // 位置の設定
        you->position.x = (float)((kYouRangeRight - kYouRangeLeft) / 4);
        you->position.y = (float)kYouRangeBottom;

        // 向きの設定
        you->direction = 1.0f;

        // 移動の設定
        you->moveSpeed = 0.0f;
        you->moveMaximum = 4.0f;
        you->moveAccel = you->moveMaximum / 16.0f;

        // ジャンプの設定
        you->jumpSpeed = 0.0f;
        you->jumpStart = 6.0f;
        you->jumpGravity = you->jumpStart / 16.0f;

        // 点滅の設定
        you->blink = 0;

        // 位置の計算
        YouCalcPosition(you);

        // 矩形の計算
        YouCalcRect(you);
    }
}

// あなたを解放する
//
static void YouUnload(struct You *you)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }
}

// あなたを描画する
//
static void YouDraw(struct You *you)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // スプライトの描画
    if ((you->blink & 0x02) == 0) {
        AsepriteDrawRotatedSpriteAnimation(&you->animations[kYouAnimationWitch], (int)you->position.x,  (int)you->position.y, 0.0f, 0.5f, 1.0f, you->direction, 1.0f, kDrawModeCopy);
    }
    if (GameIsPlay() && you->blink == 0) {
        AsepriteDrawRotatedSpriteAnimation(&you->animations[kYouAnimationAim], (int)you->aim.x,  (int)you->aim.y, you->aimDegree, 0.5f, 0.5f, 1.0f, 1.0f, kDrawModeXOR);
    }
}

// あなたが待機する
//
static void YouIdle(struct You *you)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (you->actor.state == 0) {

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&you->animations[kYouAnimationWitch], "you", "Idle", true);
        AsepriteStartSpriteAnimation(&you->animations[kYouAnimationAim], "aim", "Null", true);

        // 初期化の完了
        ++you->actor.state;
    }

    // 位置の計算
    YouCalcPosition(you);

    // 矩形の計算
    YouCalcRect(you);

    // 点滅の更新
    if (you->blink > 0) {
        --you->blink;
    }
    
    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&you->animations[kYouAnimationWitch]);
    AsepriteUpdateSpriteAnimation(&you->animations[kYouAnimationAim]);

    // 描画処理の設定
    ActorSetDraw(&you->actor, (ActorFunction)YouDraw, kGameOrderYou);

    // プレイの開始
    if (GameIsPlay()) {
        ActorTransition(&you->actor, (ActorFunction)YouWalk);
    }
}

// あなたが歩く
//
static void YouWalk(struct You *you)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (you->actor.state == 0) {

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&you->animations[kYouAnimationWitch], "you", "Idle", true);
        AsepriteStartSpriteAnimation(&you->animations[kYouAnimationAim], "aim", "Idle", true);

        // 初期化の完了
        ++you->actor.state;
    }

    // 左右の移動
    YouMoveLr(you);

    // ジャンプ
    if (you->position.y >= (float)kYouRangeBottom) {
        if (GameIsPlay()) {
            if (
                IocsIsButtonEdge(kButtonUp) || 
                IocsIsButtonEdge(kButtonDown) || 
                IocsIsButtonEdge(kButtonLeft) || 
                IocsIsButtonEdge(kButtonRight) || 
                IocsIsButtonEdge(kButtonA) || 
                IocsIsButtonEdge(kButtonB)
            ) {
                you->jumpSpeed = -you->jumpStart;
                you->position.y += you->jumpSpeed;
                ActorTransition(&you->actor, (ActorFunction)YouJump);
            }
        }
    }

    // 位置の計算
    YouCalcPosition(you);

    // 矩形の計算
    YouCalcRect(you);

    // 魔法を放つ
    YouFire(you);

    // 点滅の更新
    if (you->blink > 0) {
        --you->blink;
    }
    
    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&you->animations[kYouAnimationWitch]);
    AsepriteUpdateSpriteAnimation(&you->animations[kYouAnimationAim]);

    // 描画処理の設定
    ActorSetDraw(&you->actor, (ActorFunction)YouDraw, kGameOrderYou);
}

// あなたがジャンプする
//
static void YouJump(struct You *you)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (you->actor.state == 0) {

        // アニメーションの開始
        AsepriteStartSpriteAnimation(&you->animations[kYouAnimationWitch], "you", "Jump", true);
        AsepriteStartSpriteAnimation(&you->animations[kYouAnimationAim], "aim", "Idle", true);

        // SE の再生
        IocsPlayAudioEffect(kGameAudioEffectJump, 1);

        // 初期化の完了
        ++you->actor.state;
    }

    // 左右の移動
    YouMoveLr(you);

    // ジャンプ
    you->jumpSpeed += you->jumpGravity;
    you->position.y += you->jumpSpeed;
    if (you->position.y >= (float)kYouRangeBottom) {
        you->position.y = (float)kYouRangeBottom;
        ActorTransition(&you->actor, (ActorFunction)YouWalk);
    }

    // 位置の計算
    YouCalcPosition(you);

    // 矩形の計算
    YouCalcRect(you);

    // 魔法を放つ
    YouFire(you);

    // 点滅の更新
    if (you->blink > 0) {
        --you->blink;
    }
    
    // スプライトの更新
    AsepriteUpdateSpriteAnimation(&you->animations[kYouAnimationWitch]);
    AsepriteUpdateSpriteAnimation(&you->animations[kYouAnimationAim]);

    // 描画処理の設定
    ActorSetDraw(&you->actor, (ActorFunction)YouDraw, kGameOrderYou);
}

// あなたを左右に移動させる
//
static void YouMoveLr(struct You *you)
{
    float crank = IocsGetCrankAngle();
    bool play = GameIsPlay();
    if (play && 180.0f < crank && crank < 360.0f) {
        you->moveSpeed -= you->moveAccel;
        if (you->moveSpeed < -you->moveMaximum) {
            you->moveSpeed = -you->moveMaximum;
        }
        you->direction = -1.0f;
    } else if (play && 0.0f < crank && crank < 180.0f) {
        you->moveSpeed += you->moveAccel;
        if (you->moveSpeed > you->moveMaximum) {
            you->moveSpeed = you->moveMaximum;
        }
        you->direction = 1.0f;
    } else if (you->moveSpeed < -you->moveAccel) {
        you->moveSpeed += you->moveAccel;
    } else if (you->moveSpeed > you->moveAccel) {
        you->moveSpeed -= you->moveAccel;
    } else {
        you->moveSpeed = 0.0f;
    }
    you->position.x += you->moveSpeed;
    if (you->position.x < (float)kYouRangeLeft) {
        you->position.x = (float)kYouRangeLeft;
    } else if (you->position.x > (float)kYouRangeRight) {
        you->position.x = (float)kYouRangeRight;
    }
}

// 位置を計算する
//
static void YouCalcPosition(struct You *you)
{
    // 中心の計算
    you->center.x = you->position.x + (float)kYouCenterX;
    you->center.y = you->position.y + (float)kYouCenterY;

    // 照準の計算
    you->aimDegree = IocsGetCrankAngle();
    if (you->aimDegree >= 360.0f) {
        you->aimDegree -= 360.0f;
    }
    you->aimRadian = you->aimDegree * (float)M_PI / 180.0f;
    you->aim.x = you->center.x + (float)kYouAimR * sinf(you->aimRadian);
    you->aim.y = you->center.y - (float)kYouAimR * cosf(you->aimRadian);
}

// あなたが魔法を放つ
//
static void YouFire(struct You *you)
{
    if (GameIsPlay() && you->blink == 0) {
        if (
            (you->direction < 0.0f && (you->aimDegree == 0.0f || you->aimDegree >= 180.0f)) || 
            (you->direction > 0.0f && you->aimDegree <= 180.0f)
        ) {
            MagicLoad(you->aim.x, you->aim.y, you->aimRadian);
        }
    }
}

// 矩形を計算する
//
static void YouCalcRect(struct You *you)
{
    you->rect.left = (int)you->position.x + kYouRectLeft;
    you->rect.top = (int)you->position.y + kYouRectTop;
    you->rect.right = (int)you->position.x + kYouRectRight;
    you->rect.bottom = (int)you->position.y + kYouRectBottom;
}

// あなたの矩形を位置する
//
struct Vector *YouGetPosition(void)
{
    struct You *you = (struct You *)ActorFindWithTag(kGameTagYou);
    return you != NULL ? &you->position : NULL;
}

// あなたの矩形を取得する
//
struct Rect *YouGetRect(void)
{
    struct You *you = (struct You *)ActorFindWithTag(kGameTagYou);
    return you != NULL && you->blink == 0 ? &you->rect : NULL;
}

// あなたがダメージを受ける
//
void YouDamage(int direction)
{
    struct You *you = (struct You *)ActorFindWithTag(kGameTagYou);
    if (you != NULL) {
        you->moveSpeed = 6.0f * (float)direction;
        you->blink = kYouBlinkDamage;
    }
}

