// Title.c - タイトル
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
#include "Title.h"
#include "Logo.h"
#include "Crystal.h"
#include "Starter.h"
#include "Fade.h"

// 内部関数
//
static void TitleUnload(struct Title *title);
static void TitleTransition(struct Title *title, TitleFunction function);
static void TitleLoad(struct Title *title);
static void TitlePlay(struct Title *title);
static void TitleDone(struct Title *title);

// 内部変数
//
static const char *titleSpriteNames[] = {
    "logo", 
    "crystal", 
    "notice", 
    "fade", 
};


// タイトルを更新する
//
void TitleUpdate(struct Title *title)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (title == NULL) {

        // タイトルの作成
        title = playdate->system->realloc(NULL, sizeof (struct Title));
        if (title == NULL) {
            playdate->system->error("%s: %d: title instance is not created.", __FILE__, __LINE__);
            return;
        }
        memset(title, 0, sizeof (struct Title));

        // タイトルの初期化
        {
            // ユーザデータの設定
            SceneSetUserdata(title);

            // 解放の設定
            SceneSetUnload((SceneFunction)TitleUnload);
        }

        // スプライトの読み込み
        AsepriteLoadSpriteList(titleSpriteNames, kTitleSpriteNameSize);

        // オーディオの読み込み
        // IocsLoadAudioEffects(titleAudioEffectPaths, kTitleAudioEffectSize);

        // ロゴの初期化
        LogoInitialize();

        // 結晶の初期化
        CrystalInitialize();
        
        // スターターの初期化
        StarterInitialize();

        // フェードの初期化
        FadeInitialize();

        // 処理の設定
        TitleTransition(title, (TitleFunction)TitleLoad);
    }

    // 処理の更新
    if (title->function != NULL) {
        (*title->function)(title);
    }
}

// タイトルを解放する
//
static void TitleUnload(struct Title *title)
{
    // アクタの解放
    ActorUnloadAll();

    // スプライトの解放
    AsepriteUnloadAllSprites();

    // オーディオの解放
    IocsUnloadAllAudioEffects();
}

// 処理を遷移する
//
static void TitleTransition(struct Title *title, TitleFunction function)
{
    title->function = function;
    title->state = 0;
}

// タイトルを読み込む
//
static void TitleLoad(struct Title *title)
{
    // 初期化
    if (title->state == 0) {

        // ロゴの読み込み
        LogoLoad();

        // フェードの読み込み
        FadeLoad(kFadeTypeIn);

        // 初期化の完了
        ++title->state;
    }

    // フェードの完了
    if (FadeIsDone()) {

        // フェードの削除　
        FadeKill();

        // 処理の遷移
        TitleTransition(title, (TitleFunction)TitlePlay);
    }
}

// タイトルをプレイする
//
static void TitlePlay(struct Title *title)
{
    // 初期化
    if (title->state == 0) {

        // スターターの読み込み
        StarterLoad();

        // 初期化の完了
        ++title->state;
    }

    //　スターターの完了
    if (StarterIsDone()) {

        // 処理の遷移
        TitleTransition(title, (TitleFunction)TitleDone);
    }
}

// タイトルを完了する
//
static void TitleDone(struct Title *title)
{
    // 初期化
    if (title->state == 0) {

        // フェードの読み込み
        FadeLoad(kFadeTypeOut);

        // 初期化の完了
        ++title->state;
    }

    // フェードの完了
    if (FadeIsDone()) {

        // アクタの解放
        ActorUnloadAll();

        // シーンの遷移
        ApplicationTransition(kApplicationSceneGame);
    }
}

