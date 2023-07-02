// Iocs.h - 入出力制御システム
//
#pragma once

// 参照ファイル
// 
#include <stdbool.h>
#include "pd_api.h"


// フレームレート
//
enum {
    kIocsFrameRate = 30, 
    kIocsFrameMillisecond = 1000 / kIocsFrameRate, 
};

// フォント
//
typedef enum {
    kIocsFontSystem = 0, 
    kIocsFontJapanese, 
    kIocsFontGame, 
    kIocsFontSize, 
} IocsFont;

// ボタン
//
typedef enum {
    kIocsButtonLeft = 0, 
    kIocsButtonRight, 
    kIocsButtonUp, 
    kIocsButtonDown, 
    kIocsButtonB, 
    kIocsButtonA, 
    kIocsButtonSize, 
} IocsButton;

// キーコード
//
enum {
    kIocsKeycodeSize = 256, 
};

// オーディオ
//
typedef enum {
    kIocsAudioSystemSampleNull = 0, 
    kIocsAudioSystemSamplePipo, 
    kIocsAudioSystemSamplePi, 
    kIocsAudioSystemSamplePo, 
    kIocsAudioSystemSampleSize, 
} IocsAudioSystemSample;
enum {
    kIocsAudioEffectSampleSize = 16, 
    kIocsAudioEffectPlayerSize = 8, 
};
enum {
    kIocsAudioMusicSampleRate = 44100, 
};

// 乱数
//
struct XorShift {
    uint32_t seed;
};

// 入出力聖書システム
//
struct Iocs {

    // Playdate
    PlaydateAPI *playdate;

    // フォント
    LCDFont *fonts[kIocsFontSize];

    // 画面
    LCDColor screenColor;

    // ボタン
    PDButtons buttonPush;
    PDButtons buttonEdge;
    PDButtons buttonRepeat;
    int buttonRepeatCounts[kIocsButtonSize];
    int buttonRepeatCountDelay;
    int buttonRepeatCountInterval;

    // クランク
    float crankAngle;
    float crankChange;

    // キーコード
    bool keycodePresseds[kIocsKeycodeSize];

    // オーディオ
    AudioSample *audioSystemSamples[kIocsAudioSystemSampleSize];
    int audioSystemFrames[kIocsAudioSystemSampleSize];
    SamplePlayer *audioSystemPlayer;
    AudioSample *audioEffectSamples[kIocsAudioEffectSampleSize];
    int audioEffectFrames[kIocsAudioEffectSampleSize];
    SamplePlayer *audioEffectPlayers[kIocsAudioEffectPlayerSize];
    FilePlayer *audioMusicPlayer;

    // 乱数
    struct XorShift xorshift;

};


// 外部関数
//
extern void IocsInitialize(PlaydateAPI *playdate);
extern struct Iocs *IocsGetInstance(void);
extern PlaydateAPI *IocsGetPlaydate(void);
extern void IocsEventHandler(PDSystemEvent event, uint32_t arg);
extern void IocsUpdateBegin(void);
extern void IocsUpdateEnd(void);
extern int IocsGetFrameRate(void);
extern int IocsGetFrameMillisecond(void);
extern void IocsSetFont(IocsFont font);
extern int IocsGetFontHeight(IocsFont font);
extern int IocsGetTextWidth(IocsFont font, const char *text);
extern void IocsSetScreenColor(LCDColor color);
extern void IocsClearScreen(void);
extern bool IocsIsButtonPush(PDButtons button);
extern bool IocsIsButtonEdge(PDButtons button);
extern bool IocsIsButtonRepeat(PDButtons button);
extern float IocsGetCrankAngle(void);
extern float IocsGetCrankChange(void);
extern bool IocsIsKeycodePressed(int keycode);
extern void IocsPlayAudioSystem(IocsAudioSystemSample sample, int repeat);
extern void IocsStopAudioSystem(void);
extern void IocsLoadAudioEffects(const char *paths[], int size);
extern void IocsUnloadAllAudioEffects(void);
extern int IocsPlayAudioEffect(int sample, int repeat);
extern void IocsStopAudioEffect(int player);
extern void IocsStopAllAudioEffects(void);
extern void IocsPlayAudioMusic(const char *path, int repeat);
extern void IocsStopAudioMusic(float fade);
extern int IocsGetCharByte(char c);
extern int IocsGetTextLength(const char *text);
extern void IocsSetRandomSeed(struct XorShift *xorshift, uint32_t seed);
extern int IocsGetRandomNumber(struct XorShift *xorshift);
extern int IocsGetRandomRange(struct XorShift *xorshift, int minimum, int maximum);
extern bool IocsGetRandomBool(struct XorShift *xorshift);

