// Templete.c - テンプレート
//

// 外部参照
//
#include <string.h>
#include "pd_api.h"
#include "Iocs.h"
#include "Scene.h"
#include "Actor.h"
#include "Aseprite.h"
#include "Application.h"
#include "SceneTemplete.h"

// 内部関数
//
static void TempleteUnload(struct Templete *templete);
static void TempleteTransition(struct Templete *templete, TempleteFunction function);
static void TempletePlay(struct Templete *templete);
static void TempleteDone(struct Templete *templete);

// 内部変数
//
static const char *templeteSpriteNames[] = {
    "", 
};


// テンプレートを更新する
//
void TempleteUpdate(struct Templete *templete)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (templete == NULL) {

        // テンプレートの作成
        templete = playdate->system->realloc(NULL, sizeof (struct Templete));
        if (templete == NULL) {
            playdate->system->error("%s: %d: templete instance is not created.", __FILE__, __LINE__);
            return;
        }
        memset(templete, 0, sizeof (struct Templete));

        // テンプレートの初期化
        {
            // ユーザデータの設定
            SceneSetUserdata(templete);

            // 解放の設定
            SceneSetUnload((SceneFunction)TempleteUnload);
        }

        // スプライトの読み込み
        AsepriteLoadSpriteList(templeteSpriteNames, kTempleteSpriteNameSize);

        // 処理の設定
        TempleteTransition(templete, (TempleteFunction)TempletePlay);
    }

    // 処理の更新
    if (templete->function != NULL) {
        (*templete->function)(templete);
    }
}

// テンプレートを解放する
//
static void TempleteUnload(struct Templete *templete)
{
    // アクタの解放
    ActorUnloadAll();

    // スプライトの解放
    AsepriteUnloadAllSprites();
}

// 処理を遷移する
//
static void TempleteTransition(struct Templete *templete, TempleteFunction function)
{
    templete->function = function;
    templete->state = 0;
}

// テンプレートをプレイする
//
static void TempletePlay(struct Templete *templete)
{
    // 初期化
    if (templete->state == 0) {

        // 初期化の完了
        ++templete->state;
    }
}

// テンプレートを完了する
//
static void TempleteDone(struct Templete *templete)
{
    // 初期化
    if (templete->state == 0) {

        // 初期化の完了
        ++templete->state;
    }

    // シーンの遷移
    ApplicationTransition(kApplicationSceneGame);
}

