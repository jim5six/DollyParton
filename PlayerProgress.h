#ifndef PLAYER_PROGRESS_H
#define PLAYER_PROGRESS_H

#include <stdint.h>
#include "DollyParton.h"

#ifndef PLAYER_PROGRESS_MAX_PLAYERS
#define PLAYER_PROGRESS_MAX_PLAYERS 4
#endif

enum SongMode
{
    SONG_MODE_JOLENE = 0,           // Pops
    SONG_MODE_COAT_OF_MANY_COLORS,  // Saucer
    SONG_MODE_JOSHUA,               // Spinner
    SONG_MODE_ISLANDS,              // Inlines
    SONG_MODE_DUMB_BLONDE,          // Standups
    SONG_MODE_COUNT,
    SONG_MODE_NONE,
};

void Player_ResetGame();
void Player_ResetForBall(uint8_t playerNumber);

uint32_t Player_GetScore(uint8_t playerNumber);
void Player_SetScore(uint8_t playerNumber, uint32_t score);
void Player_AddScore(uint8_t playerNumber, uint32_t points);

uint8_t Player_GetBonus(uint8_t playerNumber);
void Player_SetBonus(uint8_t playerNumber, uint8_t bonusToAdd);
void Player_AddBonus(uint8_t playerNumber, uint8_t bonusToAdd);

void Player_SpotNextDollyLetter(uint8_t playerNumber);
bool Player_IsDollyLetterSpotted(uint8_t playerNumber, DollyLetter letter);
bool Player_IsPartonLetterSpotted(uint8_t playerNumber, PartonLetter letter);
void Player_SpotPartonLetter(uint8_t playerNumber, PartonLetter letter);

SongMode Player_GetLitSongMode(uint8_t playerNumber);
void Player_CycleLitSongMode(uint8_t playerNumber);
void Player_SetSongModePlayed(uint8_t playerNumber, SongMode songMode);
bool Player_HasSongModeBeenPlayed(uint8_t playerNumber, SongMode songMode);
void Player_ResetSongModePlayed(uint8_t playerNumber, SongMode songMode);

#endif