#ifndef PLAYER_PROGRESS_H
#define PLAYER_PROGRESS_H

#include <stdint.h>

#ifndef PLAYER_PROGRESS_MAX_PLAYERS
#define PLAYER_PROGRESS_MAX_PLAYERS 4
#endif

class PlayerProgress {
public:
    static PlayerProgress &Instance();

    void ResetGame();
    void ResetForBall(uint8_t playerNumber);

    uint32_t GetScore(uint8_t playerNumber) const;
    void SetScore(uint8_t playerNumber, uint32_t score);
    void AddScore(uint8_t playerNumber, uint32_t points);

    uint8_t GetBonus(uint8_t playerNumber) const;
    void SetBonus(uint8_t playerNumber, uint8_t bonus);
    void AddBonus(uint8_t playerNumber, uint8_t bonus);

    uint8_t GetBonusMultiplier(uint8_t playerNumber) const;
    void SetBonusMultiplier(uint8_t playerNumber, uint8_t multiplier);

    uint8_t GetModeProgress(uint8_t playerNumber) const;
    void SetModeProgress(uint8_t playerNumber, uint8_t progress);

private:
    PlayerProgress();
    PlayerProgress(const PlayerProgress &);
    PlayerProgress &operator=(const PlayerProgress &);

    uint32_t scores[PLAYER_PROGRESS_MAX_PLAYERS];
    uint8_t bonuses[PLAYER_PROGRESS_MAX_PLAYERS];
    uint8_t bonusMultipliers[PLAYER_PROGRESS_MAX_PLAYERS];
    uint8_t modeProgress[PLAYER_PROGRESS_MAX_PLAYERS];
};

#endif
