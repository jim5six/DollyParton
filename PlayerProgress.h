#ifndef PLAYER_PROGRESS_H
#define PLAYER_PROGRESS_H

#include <stdint.h>
#include "DollyParton.h"

#ifndef PLAYER_PROGRESS_MAX_PLAYERS
#define PLAYER_PROGRESS_MAX_PLAYERS 4
#endif

enum SongMode
    {
        SONG_MODE_JOLENE = 0,
        SONG_MODE_9_TO_5,
        SONG_MODE_COAT_OF_MANY_COLORS,
        SONG_MODE_BABY_IM_BURNIN,
        SONG_MODE_IM_GONE,
        SONG_MODE_COUNT,
        SONG_MODE_NONE,
    };

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

    void SpotNextDollyLetter(uint8_t playerNumber);
    bool IsDollyLetterSpotted(uint8_t playerNumber, DollyLetter letter) const;
    bool IsPartonLetterSpotted(uint8_t playerNumber, PartonLetter letter) const;
    void SpotPartonLetter(uint8_t playerNumber, PartonLetter letter);

    SongMode GetLitSongMode(uint8_t playerNumber) const;
    void CycleLitSongMode(uint8_t playerNumber);
    void SetSongModePlayed(uint8_t playerNumber, SongMode songMode);
    bool HasSongModeBeenPlayed(uint8_t playerNumber, SongMode songMode) const;
    void ResetSongModePlayed(uint8_t playerNumber, SongMode songMode);

private:
    PlayerProgressManager();
    PlayerProgressManager(const PlayerProgressManager &);
    PlayerProgressManager &operator=(const PlayerProgressManager &);

    uint32_t score[PLAYER_PROGRESS_MAX_PLAYERS];
    uint8_t bonus[PLAYER_PROGRESS_MAX_PLAYERS];
    uint8_t bonusMultiplier[PLAYER_PROGRESS_MAX_PLAYERS];
    uint8_t modeProgress[PLAYER_PROGRESS_MAX_PLAYERS];
    SongMode litSongMode[PLAYER_PROGRESS_MAX_PLAYERS];
    bool songModesPlayed[PLAYER_PROGRESS_MAX_PLAYERS][SONG_MODE_COUNT];

    bool DollyLettersProgress[PLAYER_PROGRESS_MAX_PLAYERS][DOLLY_LETTERS_COUNT];
    bool PartonLettersProgress[PLAYER_PROGRESS_MAX_PLAYERS][PARTON_LETTERS_COUNT];
};

#endif
