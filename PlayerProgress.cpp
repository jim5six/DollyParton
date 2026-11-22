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

    return bonus[playerNumber];
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

void PlayerProgressManager::SpotDollyLetter(uint8_t playerNumber, DollyLetter letter)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && letter < DOLLY_LETTERS_COUNT)
        DollyLettersProgress[playerNumber][letter] = true;
}

void PlayerProgressManager::SpotPartonLetter(uint8_t playerNumber, PartonLetter letter)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS && letter < PARTON_LETTERS_COUNT)
        PartonLettersProgress[playerNumber][letter] = true;
}
