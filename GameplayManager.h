#ifndef GAMEPLAY_MANAGER_H
#define GAMEPLAY_MANAGER_H

#include <stdint.h>

class AudioHandler;


void Gameplay_Initialize(AudioHandler &audioHandler);

void Gameplay_StartBall(uint32_t currentTime, uint32_t playerUp);
void Gameplay_EndBall(uint32_t currentTime);
uint8_t Gameplay_GetAdjacentDisplayIndex(uint8_t currentPlayer);
bool Gameplay_ManageGameMode(uint32_t currentTime);
void Gameplay_HandleSwitches(uint8_t switchHit, uint32_t currentTime);
void Gameplay_ShowLamps(uint32_t currentTime);

void Gameplay_SetCurrentPlayer(uint8_t playerNumber);
void Gameplay_ConfigureBallSave(uint32_t durationMs, uint8_t numberOfSaves = 0xFF);
void Gameplay_StartBallSave(uint32_t currentTime);
bool Gameplay_BallSaveIsActive(uint32_t currentTime);
void Gameplay_EndBallSave();
bool Gameplay_PlayfieldValidated();
uint8_t Gameplay_GetActiveBonusX();

#endif
