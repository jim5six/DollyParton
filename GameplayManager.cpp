#include "GameplayManager.h"

GameplayManager &GameplayManager::Instance()
{
    static GameplayManager instance;
    return instance;
}

GameplayManager::GameplayManager()
{
}

void GameplayManager::ResetGame()
{
}

void GameplayManager::ResetForBall(uint32_t currentTime)
{
    (void)currentTime;
}

int GameplayManager::ManageGameMode(int currentState, uint32_t currentTime)
{
    (void)currentTime;
    return currentState;
}
