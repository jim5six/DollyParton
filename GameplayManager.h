#ifndef GAMEPLAY_MANAGER_H
#define GAMEPLAY_MANAGER_H

#include <stdint.h>

class GameplayManager {
public:
    static GameplayManager &Instance();

    void ResetGame();
    void ResetForBall(uint32_t currentTime);
    bool ManageGameMode(int currentState, uint32_t currentTime);

private:
    GameplayManager();
    GameplayManager(const GameplayManager &);
    GameplayManager &operator=(const GameplayManager &);
};

#endif
