#ifndef GAMEPLAY_MANAGER_H
#define GAMEPLAY_MANAGER_H

#include <stdint.h>
#include "PlayerProgress.h"
#include "AudioHandler.h"

#define BALL_SAVE_GRACE_PERIOD 2000

class GameplayManager {
public:
    static GameplayManager &Instance(AudioHandler &audioHandler);

    // Functions that are called by the main loop to manage gameplay
    // Eventually we should condense most of these into one call that happens per loop
    void StartBall(uint32_t currentTime, uint32_t playerUp);
    void EndBall(uint32_t currentTime);
    static byte GetAdjacentDisplayIndex(byte currentPlayer);
    bool ManageGameMode(uint32_t currentTime);
    void HandleGameplaySwitches(byte switchHit, uint32_t currentTime);
    void ShowGameplayLamps(uint32_t currentTime);

    void SetCurrentPlayer(uint8_t playerNumber) { currentPlayer = playerNumber; }

    void ConfigureBallSave(uint32_t durationMs, uint8_t numberOfSaves = 0xFF);
    void StartBallSave(uint32_t currentTime) { ballSaveEndTime = currentTime + ballSaveDurationMs; numberOfBallSavesRemaining = 1; }
    bool BallSaveIsActive(uint32_t currentTime) const { return ballSaveEndTime && currentTime < ballSaveEndTime + BALL_SAVE_GRACE_PERIOD; }
    void EndBallSave();
    bool PlayfieldValidated();
    uint8_t GetActiveBonusX();

private:
    enum GameMode
    {
        GAME_MODE_SKILL_SHOT,
        GAME_MODE_UNSTRUCTURED_PLAY,
        GAME_MODE_SONG
    };

    const uint32_t skillShotDurationMs = 10000;
    const uint32_t songAwardCountdownInterval = 100;
    const uint32_t songAwardScoreTick = 200;

    uint8_t currentPlayer;                  // Current player number (0-based index)
    GameMode currentMode;                   // The current game mode
    uint32_t currentModeStartTime;          // The time when the current game mode started

    SongMode activeSong = SONG_MODE_NONE;
    uint32_t songMusicStartTime = 0;
    uint32_t showNextSongDisplayEnd = 0;
    const uint32_t showSongDisplayTimeMs = 4000;
    uint32_t accumulatedSongBonus = 0;
    uint8_t songBonusX = 1;
    uint8_t songBonusesAwarded = 0;
    const uint8_t maxSongBonusesAllowed = 3;
    bool songBonusAwarding = false;
    uint32_t nextSongBonusAwardTime = 0;

    uint32_t ballSaveDurationMs = 15000;     // Duration of the ball save in milliseconds;
    uint32_t ballSaveEndTime = 0;
    uint8_t numberOfBallSavesAllowed = 0xFF;
    uint8_t numberOfBallSavesRemaining = 0;
    uint8_t playfieldMultiplier = 1;
    bool samePlayerShootsAgain = false;
    bool spinnerIsLit = false;

    uint32_t lastSwitchHitTime = 0;
    uint32_t ballFirstSwitchHitTime = 0;

    bool ballDollyStandupProgress[DOLLY_LETTERS_COUNT];
    uint8_t ballBonusX;

    AudioHandler &audio;
    PlayerProgressManager &playerProgress = PlayerProgressManager::Instance();

    explicit GameplayManager(AudioHandler &audio);
    GameplayManager(const GameplayManager &);
    GameplayManager &operator=(const GameplayManager &);
    
    void ShowShootAgainLamp(uint32_t currentTime);
    void ShowSpinnerLamp();
    void ShowDollyTargetLamps();
    void ShowDollyBonusLamps();
    void LightPartonBonusLamps();
    void ShowPartonBonusLamps();
    void ShowBonusXLamps();

    void AddToCurrentScore(uint32_t points, bool useMultiplier = true);
    void AddToSongBonus(uint32_t points, bool useMultiplier = true);
    void ValidateAndRegisterPlayfieldSwitch(uint32_t currentTime);
    void PlaySoundEffect(unsigned short soundIndex) { audio.PlaySound(soundIndex, AUDIO_PLAY_TYPE_WAV_TRIGGER); }

    void SpotDollyStandupLetter(DollyLetter letter);
    void AdvanceBonusX();
    void AdvanceSongBonusX();
    void StartSongMode(uint32_t currentTime);
    bool SongModeIsActive();

    void SetMode(GameMode newMode, uint32_t currentTime) {
        currentMode = newMode;
        currentModeStartTime = currentTime;
    }
};

#endif
