#ifndef PLAYER_PROGRESS_H
#define PLAYER_PROGRESS_H

#include <stdint.h>
#include "DollyParton.h"

#ifndef PLAYER_PROGRESS_MAX_PLAYERS
#define PLAYER_PROGRESS_MAX_PLAYERS 4
#endif

/**
 * This class manages the holdover progress of each player in the game, including score, bonus, and letter collection for Dolly and Parton.
 */

class PlayerProgressManager {
public:
    static PlayerProgressManager &Instance();

    void ResetGame();
    void ResetForBall(uint8_t playerNumber);

    uint32_t GetScore(uint8_t playerNumber) const;
    void SetScore(uint8_t playerNumber, uint32_t score);
    void AddScore(uint8_t playerNumber, uint32_t points);

    uint8_t GetBonus(uint8_t playerNumber) const;
    void SetBonus(uint8_t playerNumber, uint8_t bonustoAdd);
    void AddBonus(uint8_t playerNumber, uint8_t bonustoAdd);

    void SpotDollyLetter(uint8_t playerNumber, DollyLetter letter);
    void SpotPartonLetter(uint8_t playerNumber, PartonLetter letter);

private:
    PlayerProgressManager();
    PlayerProgressManager(const PlayerProgressManager &);
    PlayerProgressManager &operator=(const PlayerProgressManager &);

    uint32_t score[PLAYER_PROGRESS_MAX_PLAYERS];
    uint8_t bonus[PLAYER_PROGRESS_MAX_PLAYERS];
    uint8_t bonusMultiplier[PLAYER_PROGRESS_MAX_PLAYERS];
    uint8_t modeProgress[PLAYER_PROGRESS_MAX_PLAYERS];

    bool DollyLettersProgress[PLAYER_PROGRESS_MAX_PLAYERS][DOLLY_LETTERS_COUNT];
    bool PartonLettersProgress[PLAYER_PROGRESS_MAX_PLAYERS][PARTON_LETTERS_COUNT];
};

#endif
