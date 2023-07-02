// Game.c - ゲーム
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
#include "Game.h"
#include "Back.h"
#include "You.h"
#include "Magic.h"
#include "Rival.h"
#include "Cat.h"
#include "Crystal.h"
#include "Status.h"
#include "Announce.h"
#include "Notice.h"
#include "Fade.h"

// 内部関数
//
static void GameUnload(struct Game *game);
static void GameTransition(struct Game *game, GameFunction function);
static void GameLoad(struct Game *game);
static void GameStart(struct Game *game);
static void GamePlay(struct Game *game);
static void GameTimeUp(struct Game *game);
static void GameResult(struct Game *game);
static void GameDone(struct Game *game);
static void GameHit(struct Game *game);

// 内部変数
//
static const char *gameSpriteNames[] = {
    "back", 
    "you", 
    "aim", 
    "magic", 
    "rival", 
    "cat", 
    "crystal", 
    "status", 
    "notice", 
    "fade", 
};
static const char *gameAudioEffectPaths[] = {
    "sounds/jump", 
    "sounds/hit", 
    "sounds/cat", 
};
static const char *gameAudioMusicPaths[] = {
    "sounds/bgm", 
    "sounds/win", 
    "sounds/lose", 
};


// ゲームを更新する
//
void GameUpdate(struct Game *game)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (game == NULL) {

        // ゲームの作成
        game = playdate->system->realloc(NULL, sizeof (struct Game));
        if (game == NULL) {
            playdate->system->error("%s: %d: game instance is not created.", __FILE__, __LINE__);
            return;
        }
        memset(game, 0, sizeof (struct Game));

        // ゲームの初期化
        {
            // ユーザデータの設定
            SceneSetUserdata(game);

            // 解放の設定
            SceneSetUnload((SceneFunction)GameUnload);

            // タイムの設定
            game->time = kGameTimeStart;

            // プレイ中の設定
            game->play = false;
        }

        // スプライトの読み込み
        AsepriteLoadSpriteList(gameSpriteNames, kGameSpriteNameSize);

        // オーディオの読み込み
        IocsLoadAudioEffects(gameAudioEffectPaths, kGameAudioEffectSize);

        // 背景の初期化
        BackInitialize();

        // あなたの初期化
        YouInitialize();

        // 魔法の初期化
        MagicInitialize();

        // ライバルの初期化
        RivalInitialize();

        // 猫の初期化
        CatInitialize();

        // 結晶の初期化
        CrystalInitialize();

        // ステータスの初期化
        StatusInitialize();

        // 告知の初期化
        AnnounceInitialize();

        // 通知の初期化
        NoticeInitialize();

        // フェードの初期化
        FadeInitialize();

        // 処理の設定
        GameTransition(game, (GameFunction)GameLoad);
    }

    // 処理の更新
    if (game->function != NULL) {
        (*game->function)(game);
    }
}

// ゲームを解放する
//
static void GameUnload(struct Game *game)
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
static void GameTransition(struct Game *game, GameFunction function)
{
    game->function = function;
    game->state = 0;
}

// ゲームを読み込む
//
static void GameLoad(struct Game *game)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (game->state == 0) {

        // 背景の読み込み
        BackLoad();

        // あなたの読み込み
        YouLoad();

        // ライバルの読み込み
        RivalLoad();

        // 猫の読み込み
        CatLoad();

        // ステータスの読み込み
        StatusLoad();

        // 通知の読み込み
        NoticeLoad();
        
        // フェードの読み込み
        FadeLoad(kFadeTypeIn);

        // BGM の再生
        IocsPlayAudioMusic(gameAudioMusicPaths[kGameAudioMusicBgm], 0);

        // 初期化の完了
        ++game->state;
    }

    // フェードの完了
    if (FadeIsDone()) {

        // フェードの削除　
        FadeKill();

        // 処理の遷移
        GameTransition(game, (GameFunction)GameStart);
    }
}

// ゲームを開始する
//
static void GameStart(struct Game *game)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (game->state == 0) {

        // 告知の読み込み
        {
            static const char *text = "LIGHT UP THE DARKNESS";
            AnnounceLoad(text, kAnnounceIdleMillisecond);
        }

        // 初期化の完了
        ++game->state;
    }

    // 告知の完了
    if (AnnounceIsDone()) {

        // 処理の遷移
        GameTransition(game, (GameFunction)GamePlay);
    }
}

// ゲームをプレイする
//
static void GamePlay(struct Game *game)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (game->state == 0) {

        // プレイの開始
        game->play = true;

        // 初期化の完了
        ++game->state;
    }

    // ヒット判定
    GameHit(game);

    // タイムの更新
    if (game->time > 0) {
        --game->time;
    } else {

        // 処理の遷移
        GameTransition(game, (GameFunction)GameTimeUp);
    }
}

// ゲームが終了する
//
static void GameTimeUp(struct Game *game)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (game->state == 0) {

        // 告知の読み込み
        {
            static const char *text = "TIME UP";
            AnnounceLoad(text, kAnnounceIdleMillisecond);
        }

        // プレイの完了
        game->play = false;

        // BGM の停止
        IocsStopAudioMusic(3.0f);

        // 初期化の完了
        ++game->state;
    }

    // 告知の完了
    if (AnnounceIsDone()) {

        // 処理の遷移
        GameTransition(game, (GameFunction)GameResult);
    }
}

// ゲームの結果を表示する
//
static void GameResult(struct Game *game)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (game->state == 0) {

        // 告知の読み込み
        {
            static const char *text[] = {
                "LIGHT WINS", 
                "DARKNESS WINS", 
                "WORLD IS GRAY", 
            };
            int light = BackGetScoreLight();
            int dark = BackGetScoreDark();
            int result = light > dark ? 0 : (light < dark ? 1 : 2);
            AnnounceLoad(text[result], 2 * kAnnounceIdleMillisecond);
            if (result == 0) {
                IocsPlayAudioMusic(gameAudioMusicPaths[kGameAudioMusicWin], 1);
            } else if (result == 1) {
                IocsPlayAudioMusic(gameAudioMusicPaths[kGameAudioMusicLose], 1);
            }
        }

        // プレイの完了
        game->play = false;

        // 初期化の完了
        ++game->state;
    }

    // 告知の完了
    if (AnnounceIsDone()) {

        // 処理の遷移
        GameTransition(game, (GameFunction)GameDone);
    }
}

// ゲームを完了する
//
static void GameDone(struct Game *game)
{
    // Playdate の取得
    PlaydateAPI *playdate = IocsGetPlaydate();
    if (playdate == NULL) {
        return;
    }

    // 初期化
    if (game->state == 0) {

        // フェードの読み込み
        FadeLoad(kFadeTypeOut);

        // 初期化の完了
        ++game->state;
    }

    // フェードの完了
    if (FadeIsDone()) {

        // アクタの解放
        ActorUnloadAll();

        // シーンの遷移
        ApplicationTransition(kApplicationSceneTitle);
    }
}

// ゲームがプレイ中かどうかを判定する
//
bool GameIsPlay(void)
{
    struct Game *game = (struct Game *)SceneGetUserdata();
    return game != NULL ? game->play : false;
}

// ゲームの時間を取得する
//
int GameGetRealTime(void)
{
    struct Game *game = (struct Game *)SceneGetUserdata();
    return game != NULL ? game->time : 0;
}
int GameGetViewTime(void)
{
    struct Game *game = (struct Game *)SceneGetUserdata();
    return game != NULL ? game->time / 2 : 0;
}

// ヒット判定を行う
//
static void GameHit(struct Game *game)
{
    // 魔法との判定
    {
        struct Magic *magic = (struct Magic *)ActorFindWithTag(kGameTagMagic);
        while (magic != NULL) {
            int result = CrystalIsBomb(&magic->rect, kGameTagCrystal);
            if (result != 0) {
                ActorUnload(&magic->actor);
            }
            magic = (struct Magic *)ActorNextWithTag(&magic->actor);
        }
    }

    // あなたとの判定
    {
        struct Rect *rect = YouGetRect();
        if (rect != NULL) {
            int result = CatIsHit(rect);
            if (result == 0) {
                result = CrystalIsHit(rect, kGameTagCrystal);
            }
            if (result != 0) {
                YouDamage(result);
            }
        }
    }
}

