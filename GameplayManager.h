#ifndef GAMEPLAY_MANAGER_H
#define GAMEPLAY_MANAGER_H

#include <stdint.h>
#include "PlayerProgress.h"

class GameplayManager {
public:
    static GameplayManager &Instance();

    void ResetForBall(uint32_t currentTime);
    bool ManageGameMode(int currentState, uint32_t currentTime);

private:
    GameplayManager();
    GameplayManager(const GameplayManager &);
    GameplayManager &operator=(const GameplayManager &);

    PlayerProgressManager &playerProgress = PlayerProgressManager::Instance();
};

#endif
