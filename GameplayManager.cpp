#include "GameplayManager.h"

GameplayManager &GameplayManager::Instance()
{
    static GameplayManager instance;
    return instance;
}

GameplayManager::GameplayManager()
{
}

void GameplayManager::ResetForBall(uint32_t currentTime)
{
    (void)currentTime;
}

// The main gameplay handler that should be called every cycle while in game mode. 
// Returns TRUE if the ball is still in play
bool GameplayManager::ManageGameMode(int currentState, uint32_t currentTime)
{
    (void)currentTime;
    return currentState;
}
