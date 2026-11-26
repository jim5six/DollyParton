#include "PlayerProgress.h"

PlayerProgressManager &PlayerProgressManager::Instance()
{
    static PlayerProgressManager instance;
    return instance;
}

PlayerProgressManager::PlayerProgressManager()
{
    
}

void PlayerProgressManager::ResetGame()
{
    for (uint8_t player = 0; player < PLAYER_PROGRESS_MAX_PLAYERS; ++player)
    {
        score[player] = 0;
        bonus[player] = 0;
        bonusMultiplier[player] = 1;
        modeProgress[player] = 0;

        //Reset dolly and parton letters progress
        for (uint8_t letter = 0; letter < DOLLY_LETTERS_COUNT; ++letter)
        {
            DollyLettersProgress[player][letter] = 0;
        }
        for (uint8_t letter = 0; letter < PARTON_LETTERS_COUNT; ++letter)
        {
            PartonLettersProgress[player][letter] = 0;
        }
    }
}

void PlayerProgressManager::ResetForBall(uint8_t playerNumber)
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return;

    // TODO: Bonus hold?
    bonus[playerNumber] = 0;
    bonusMultiplier[playerNumber] = 1;
    modeProgress[playerNumber] = 0;
}

uint32_t PlayerProgressManager::GetScore(uint8_t playerNumber) const
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return 0;

    return score[playerNumber];
}

void PlayerProgressManager::SetScore(uint8_t playerNumber, uint32_t value)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        score[playerNumber] = value;
}

void PlayerProgressManager::AddScore(uint8_t playerNumber, uint32_t value)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        score[playerNumber] += value;
}

uint8_t PlayerProgressManager::GetBonus(uint8_t playerNumber) const
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return 0;

    uint8_t DollyBonusTotal = 1; // 1000 minimum bonus

    for (int letter = 0; letter < DOLLY_LETTERS_COUNT; letter++)
    {
        if (DollyLettersProgress[playerNumber][letter])
        {
            DollyBonusTotal += 1;
        }
    }

    for (int letter = 0; letter < PARTON_LETTERS_COUNT; letter++)
    {
        if (PartonLettersProgress[playerNumber][letter])
        {
            DollyBonusTotal += 1;
        }
    }

    return DollyBonusTotal + bonus[playerNumber];
}

void PlayerProgressManager::SetBonus(uint8_t playerNumber, uint8_t bonusToAdd)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        bonus[playerNumber] = bonusToAdd;
}

void PlayerProgressManager::AddBonus(uint8_t playerNumber, uint8_t bonustoAdd)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        bonus[playerNumber] += bonustoAdd;
}

void PlayerProgressManager::SpotNextDollyLetter(uint8_t playerNumber)
{
    // Spot the next unlit letter
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
    {
        for (uint8_t letter = 0; letter < DOLLY_LETTERS_COUNT; letter++)
        {
            if (!DollyLettersProgress[playerNumber][letter])
            {
                DollyLettersProgress[playerNumber][letter] = true;
                break;
            }
        }
    }
}

bool PlayerProgressManager::IsDollyLetterSpotted(uint8_t playerNumber, DollyLetter letter) const
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && letter < DOLLY_LETTERS_COUNT)
        return DollyLettersProgress[playerNumber][letter];
    return false;
}

bool PlayerProgressManager::IsPartonLetterSpotted(uint8_t playerNumber, PartonLetter letter) const
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && letter < PARTON_LETTERS_COUNT)
        return PartonLettersProgress[playerNumber][letter];
    return false;
}

void PlayerProgressManager::SpotPartonLetter(uint8_t playerNumber, PartonLetter letter)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && letter < PARTON_LETTERS_COUNT)
        PartonLettersProgress[playerNumber][letter] = true;
}

SongMode PlayerProgressManager::GetLitSongMode(uint8_t playerNumber) const
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        return litSongMode[playerNumber];
    return SONG_MODE_NONE;
}

void PlayerProgressManager::CycleLitSongMode(uint8_t playerNumber)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
    {
        litSongMode[playerNumber] = static_cast<SongMode>((litSongMode[playerNumber] + 1) % SONG_MODE_COUNT);
    }
}

void PlayerProgressManager::SetSongModePlayed(uint8_t playerNumber, SongMode songMode)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && songMode < SONG_MODE_COUNT)
    {
        songModesPlayed[playerNumber][songMode] = true;
    }
}

bool PlayerProgressManager::HasSongModeBeenPlayed(uint8_t playerNumber, SongMode songMode) const
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && songMode < SONG_MODE_COUNT)
        return songModesPlayed[playerNumber][songMode];
    return false;
}

void PlayerProgressManager::ResetSongModePlayed(uint8_t playerNumber, SongMode songMode)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && songMode < SONG_MODE_COUNT)
    {
        songModesPlayed[playerNumber][songMode] = false;
    }
}
