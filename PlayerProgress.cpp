#include "PlayerProgress.h"

PlayerProgress &PlayerProgress::Instance()
{
    static PlayerProgress instance;
    return instance;
}

PlayerProgress::PlayerProgress()
{
    ResetGame();
}

void PlayerProgress::ResetGame()
{
    for (uint8_t player = 0; player < PLAYER_PROGRESS_MAX_PLAYERS; ++player)
    {
        scores[player] = 0;
        bonuses[player] = 0;
        bonusMultipliers[player] = 1;
        modeProgress[player] = 0;
    }
}

void PlayerProgress::ResetForBall(uint8_t playerNumber)
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return;

    bonuses[playerNumber] = 0;
    bonusMultipliers[playerNumber] = 1;
    modeProgress[playerNumber] = 0;
}

uint32_t PlayerProgress::GetScore(uint8_t playerNumber) const
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return 0;

    return scores[playerNumber];
}

void PlayerProgress::SetScore(uint8_t playerNumber, uint32_t score)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        scores[playerNumber] = score;
}

void PlayerProgress::AddScore(uint8_t playerNumber, uint32_t points)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        scores[playerNumber] += points;
}

uint8_t PlayerProgress::GetBonus(uint8_t playerNumber) const
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return 0;

    return bonuses[playerNumber];
}

void PlayerProgress::SetBonus(uint8_t playerNumber, uint8_t bonus)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        bonuses[playerNumber] = bonus;
}

void PlayerProgress::AddBonus(uint8_t playerNumber, uint8_t bonus)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        bonuses[playerNumber] += bonus;
}

uint8_t PlayerProgress::GetBonusMultiplier(uint8_t playerNumber) const
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return 0;

    return bonusMultipliers[playerNumber];
}

void PlayerProgress::SetBonusMultiplier(uint8_t playerNumber, uint8_t multiplier)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        bonusMultipliers[playerNumber] = multiplier;
}

uint8_t PlayerProgress::GetModeProgress(uint8_t playerNumber) const
{
    if (playerNumber >= PLAYER_PROGRESS_MAX_PLAYERS)
        return 0;

    return modeProgress[playerNumber];
}

void PlayerProgress::SetModeProgress(uint8_t playerNumber, uint8_t progress)
{
    if (playerNumber < PLAYER_PROGRESS_MAX_PLAYERS)
        modeProgress[playerNumber] = progress;
}
