#ifndef GAMEPLAY_MANAGER_H
#define GAMEPLAY_MANAGER_H

#include <stdint.h>
#include "PlayerProgress.h"

#define BALL_SAVE_GRACE_PERIOD 2000

class GameplayManager {
public:
    static GameplayManager &Instance();

    void StartBall(uint32_t currentTime, uint32_t playerUp);
    bool ManageGameMode(uint32_t currentTime);
    void ShowGameplayLamps(uint32_t currentTime);
    void SetCurrentPlayer(uint8_t playerNumber) { currentPlayer = playerNumber; }

    void ConfigureBallSave(uint32_t durationMs, uint8_t numberOfSaves = 0xFF);
    void StartBallSave(uint32_t currentTime) { ballSaveEndTime = currentTime + ballSaveDurationMs; numberOfBallSavesRemaining = 1; }
    bool BallSaveIsActive(uint32_t currentTime) const { return ballSaveEndTime && currentTime < ballSaveEndTime + BALL_SAVE_GRACE_PERIOD; }
    void EndBallSave();

private:
    enum GameMode
    {
        GAME_MODE_SKILL_SHOT,
        GAME_MODE_UNSTRUCTURED_PLAY,
    };

    uint8_t currentPlayer;                  // Current player number (0-based index)
    GameMode currentMode;                   // The current game mode
    uint32_t currentModeStartTime;          // The time when the current game mode started
    uint32_t ballFirstSwitchHitTime;        // The first time a playfield switch is hit during a ball, 0 if no switch has been hit yet

    uint32_t ballSaveDurationMs = 15000;     // Duration of the ball save in milliseconds;
    uint32_t ballSaveEndTime = 0;
    uint8_t numberOfBallSavesAllowed = 0xFF;
    uint8_t numberOfBallSavesRemaining = 0;
    bool samePlayerShootsAgain = false;

    PlayerProgressManager &playerProgress = PlayerProgressManager::Instance();

    GameplayManager();
    GameplayManager(const GameplayManager &);
    GameplayManager &operator=(const GameplayManager &);
    
    void ShowShootAgainLamp(uint32_t currentTime);
    void ShowDollyTargetLamps();
    void ShowDollyBonusLamps();
    void LightPartonBonusLamps();
    void ShowPartonBonusLamps();

    void SetMode(GameMode newMode, uint32_t currentTime) {
        currentMode = newMode;
        currentModeStartTime = currentTime;
    }
};

#endif
