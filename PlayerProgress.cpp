#include "PlayerProgress.h"

static uint32_t score[PLAYER_PROGRESS_MAX_PLAYERS];
static uint8_t bonus[PLAYER_PROGRESS_MAX_PLAYERS];
static uint8_t bonusMultiplier[PLAYER_PROGRESS_MAX_PLAYERS];
static uint8_t modeProgress[PLAYER_PROGRESS_MAX_PLAYERS];
static SongMode litSongMode[PLAYER_PROGRESS_MAX_PLAYERS];
static bool songModesPlayed[PLAYER_PROGRESS_MAX_PLAYERS][SONG_MODE_COUNT];
static bool dollyLettersProgress[PLAYER_PROGRESS_MAX_PLAYERS][DOLLY_LETTERS_COUNT];
static bool partonLettersProgress[PLAYER_PROGRESS_MAX_PLAYERS][PARTON_LETTERS_COUNT];

void Player_ResetGame()
{
    for (uint8_t player = 0; player < PLAYER_PROGRESS_MAX_PLAYERS; ++player)
    {
        score[player] = 0;
        bonus[player] = 0;
        bonusMultiplier[player] = 1;
        modeProgress[player] = 0;

        for (uint8_t letter = 0; letter < DOLLY_LETTERS_COUNT; ++letter)
            dollyLettersProgress[player][letter] = false;

        for (uint8_t letter = 0; letter < PARTON_LETTERS_COUNT; ++letter)
            partonLettersProgress[player][letter] = false;

        litSongMode[player] = SONG_MODE_JOLENE;
    }
}

void Player_ResetForBall(uint8_t playerNumber)
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return;

    bonus[playerNumber] = 0;
    bonusMultiplier[playerNumber] = 1;
    modeProgress[playerNumber] = 0;
}

uint32_t Player_GetScore(uint8_t playerNumber)
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return 0;

    return score[playerNumber];
}

void Player_SetScore(uint8_t playerNumber, uint32_t value)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        score[playerNumber] = value;
}

void Player_AddScore(uint8_t playerNumber, uint32_t value)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        score[playerNumber] += value;
}

uint8_t Player_GetBonus(uint8_t playerNumber)
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return 0;

    uint8_t dollyBonusTotal = 1;
    for (uint8_t letter = 0; letter < DOLLY_LETTERS_COUNT; ++letter)
    {
        if (dollyLettersProgress[playerNumber][letter])
            ++dollyBonusTotal;
    }

    for (uint8_t letter = 0; letter < PARTON_LETTERS_COUNT; ++letter)
    {
        if (partonLettersProgress[playerNumber][letter])
            ++dollyBonusTotal;
    }

    return dollyBonusTotal + bonus[playerNumber];
}

void Player_SetBonus(uint8_t playerNumber, uint8_t bonusToAdd)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        bonus[playerNumber] = bonusToAdd;
}

void Player_AddBonus(uint8_t playerNumber, uint8_t bonusToAdd)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        bonus[playerNumber] += bonusToAdd;
}

void Player_SpotNextDollyLetter(uint8_t playerNumber)
{
    // Spot the next unlit letter
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
    {
        for (uint8_t letter = 0; letter < DOLLY_LETTERS_COUNT; letter++)
        {
            if (!dollyLettersProgress[playerNumber][letter])
            {
                dollyLettersProgress[playerNumber][letter] = true;
                break;
            }
        }
    }
}


void Player_ResetDollyLetters(uint8_t playerNumber)
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return;

    for (uint8_t letter = 0; letter < DOLLY_LETTERS_COUNT; ++letter)
    {
        if (!dollyLettersProgress[playerNumber][letter])
        {
            dollyLettersProgress[playerNumber][letter] = true;
            break;
        }
    }
}

bool Player_IsDollyLetterSpotted(uint8_t playerNumber, DollyLetter letter)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && letter < DOLLY_LETTERS_COUNT)
        return dollyLettersProgress[playerNumber][letter];

    return false;
}

bool Player_IsPartonLetterSpotted(uint8_t playerNumber, PartonLetter letter)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && letter < PARTON_LETTERS_COUNT)
        return partonLettersProgress[playerNumber][letter];

    return false;
}

void Player_SpotPartonLetter(uint8_t playerNumber, PartonLetter letter)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && letter < PARTON_LETTERS_COUNT)
        partonLettersProgress[playerNumber][letter] = true;
}

SongMode Player_GetLitSongMode(uint8_t playerNumber)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        return litSongMode[playerNumber];

    return SONG_MODE_NONE;
}

void Player_CycleLitSongMode(uint8_t playerNumber)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        litSongMode[playerNumber] = static_cast<SongMode>((litSongMode[playerNumber] + 1) % SONG_MODE_COUNT);
}

void Player_SetSongModePlayed(uint8_t playerNumber, SongMode songMode)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && songMode < SONG_MODE_COUNT)
        songModesPlayed[playerNumber][songMode] = true;
}

bool Player_HasSongModeBeenPlayed(uint8_t playerNumber, SongMode songMode)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && songMode < SONG_MODE_COUNT)
        return songModesPlayed[playerNumber][songMode];

    return false;
}

void Player_ResetSongModePlayed(uint8_t playerNumber, SongMode songMode)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && songMode < SONG_MODE_COUNT)
        songModesPlayed[playerNumber][songMode] = false;
}