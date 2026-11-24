#include "GameplayManager.h"
#include "RPU.h"
#include "DollyParton.h"
#include "SoundIDs.h"
#include "LampAnimationAPI.h"

GameplayManager &GameplayManager::Instance(AudioHandler &audio)
{
    static GameplayManager instance(audio);
    return instance;
}

GameplayManager::GameplayManager(AudioHandler &audio)
    : audioHandler(audio)
{
}

void GameplayManager::StartBall(uint32_t currentTime, uint32_t playerUp)
{
    currentPlayer = playerUp;
    ballFirstSwitchHitTime = 0;
    ballSaveEndTime = 0;
    SetMode(GAME_MODE_SKILL_SHOT, currentTime);
    numberOfBallSavesAllowed = 0xFF;
    lastSwitchHitTime = 0;
    ballFirstSwitchHitTime = 0;
}

// The main gameplay handler that should be called every cycle while in game mode. 
// Returns TRUE if the ball is still in play
bool GameplayManager::ManageGameMode(uint32_t currentTime)
{
    if (!BallSaveIsActive(currentTime))
    {
        ballSaveEndTime = 0; //TODO: Can do this cleaner, but other code uses end time as 0 to show it's not active
    }

    return true;
}

void GameplayManager::HandleGameplaySwitches(byte switchHit, uint32_t currentTime)
{
    switch (switchHit)
    {

    case SW_T_POP_BUMPER:
        AddToCurrentScore(10);
        PlaySoundEffect(SOUND_EFFECT_POP_SNARE);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_L_POP_BUMPER:
        AddToCurrentScore(20);
        PlaySoundEffect(SOUND_EFFECT_POP_BASS);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_B_POP_BUMPER:
        AddToCurrentScore(30);
        PlaySoundEffect(SOUND_EFFECT_POP_HIGHHAT);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_LEFT_INLANE:
        if (RPU_ReadLampState(LAMP_L_INLANE))
        {
            AddToCurrentScore(5000);
        }
        else
        {
            AddToCurrentScore(600);
        }
        PlaySoundEffect(SOUND_EFFECT_POP_SNARE);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_RIGHT_INLANE:
        if (RPU_ReadLampState(LAMP_R_INLANE))
        {
            AddToCurrentScore(5000);
        }
        else
        {
            AddToCurrentScore(600);
        }
        PlaySoundEffect(SOUND_EFFECT_POP_SNARE);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_LEFT_OUTLANE:
        if (RPU_ReadLampState(LAMP_L_OUTLANE))
        {
            //AwardSpecial();
        }
        else
        {
            AddToCurrentScore(1000);
        }
        PlaySoundEffect(SOUND_EFFECT_POP_BASS);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_RIGHT_OUTLANE:
        if (RPU_ReadLampState(LAMP_R_OUTLANE))
        {
            //AwardSpecial();
        }
        else
        {
            AddToCurrentScore(1000);
        }
        PlaySoundEffect(SOUND_EFFECT_POP_BASS);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_LOWER_L_TARGET:
        AddToCurrentScore(500);
        PlaySoundEffect(SOUND_EFFECT_POP_BASS);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_LOWER_R_LANE:
        AddToCurrentScore(5000);
        PlaySoundEffect(SOUND_EFFECT_POP_SNARE);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_LEFT_SLING:
        AddToCurrentScore(100);
        PlaySoundEffect(SOUND_EFFECT_LEFT_SLING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_RIGHT_SLING:
        AddToCurrentScore(100);
        PlaySoundEffect(SOUND_EFFECT_RIGHT_SLING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_DROP_1:
        AddToCurrentScore(500);
        RPU_SetLampState(LAMP_SPINNER_1K, 1, 0, 0);
        PlaySoundEffect(SOUND_EFFECT_DROP_TARGET_SOUND_1);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_DROP_2:
        AddToCurrentScore(500);
        PlaySoundEffect(SOUND_EFFECT_DROP_TARGET_SOUND_1);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_DROP_3:
        AddToCurrentScore(500);
        PlaySoundEffect(SOUND_EFFECT_DROP_TARGET_SOUND_1);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_DROP_4:
        AddToCurrentScore(500);
        PlaySoundEffect(SOUND_EFFECT_DROP_TARGET_SOUND_1);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_SUPERSTAR:
        AddToCurrentScore(5000);
        PlaySoundEffect(SOUND_EFFECT_DROP_TARGET_COMPLETE);
        RPU_PushToTimedSolenoidStack(SOL_DROP_BANK_RESET, 10, currentTime + 5000, true);
        break;
    case SW_RUBBER:
        AddToCurrentScore(30);
        PlaySoundEffect(SOUND_EFFECT_POP_BASS);
        break;
    case SW_D:
        if (RPU_ReadLampState(LAMP_1K_DOLLY_TARGET))
        {
            AddToCurrentScore(1000);
        }
        else
        {
            AddToCurrentScore(400);
        }
        PlaySoundEffect(SOUND_EFFECT_POP_SNARE);
        RPU_SetLampState(LAMP_D_TARGET, 1, 0, 0);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_O:
        if (RPU_ReadLampState(LAMP_1K_DOLLY_TARGET))
        {
            AddToCurrentScore(1000);
        }
        else
        {
            AddToCurrentScore(400);
        }
        PlaySoundEffect(SOUND_EFFECT_POP_SNARE);
        RPU_SetLampState(LAMP_O_TARGET, 1, 0, 0);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_L1:
        if (RPU_ReadLampState(LAMP_1K_DOLLY_TARGET))
        {
            AddToCurrentScore(1000);
        }
        else
        {
            AddToCurrentScore(400);
        }
        PlaySoundEffect(SOUND_EFFECT_POP_SNARE);
        RPU_SetLampState(LAMP_L1_TARGET, 1, 0, 0);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_L2:
        if (RPU_ReadLampState(LAMP_1K_DOLLY_TARGET))
        {
            AddToCurrentScore(1000);
        }
        else
        {
            AddToCurrentScore(400);
        }
        PlaySoundEffect(SOUND_EFFECT_POP_SNARE);
        RPU_SetLampState(LAMP_L2_TARGET, 1, 0, 0);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_Y:
        if (RPU_ReadLampState(LAMP_1K_DOLLY_TARGET))
        {
            AddToCurrentScore(1000);
        }
        else
        {
            AddToCurrentScore(400);
        }
        PlaySoundEffect(SOUND_EFFECT_POP_SNARE);
        RPU_SetLampState(LAMP_Y_TARGET, 1, 0, 0);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
    case SW_SPINNER:
        if (RPU_ReadLampState(LAMP_SPINNER_1K))
        {
            AddToCurrentScore(1000);
            PlaySoundEffect(SOUND_EFFECT_SPINNER);
        }
        else
        {
            AddToCurrentScore(100);
            PlaySoundEffect(SOUND_EFFECT_SPINNER);
            ValidateAndRegisterPlayfieldSwitch(currentTime);
        }
        break;
    case SW_SAUCER:
        if (currentMode != GAME_MODE_SKILL_SHOT)
        {
            AddToCurrentScore(1000);
            PlaySoundEffect(SOUND_EFFECT_POP_SNARE);
            RPU_PushToTimedSolenoidStack(SOL_SAUCER, 10, currentTime + 1000, true);
            ValidateAndRegisterPlayfieldSwitch(currentTime);
        }
        else
        {
            AddToCurrentScore(1000);
            PlaySoundEffect(SOUND_EFFECT_DROP_TARGET_SOUND_1);
            RPU_PushToTimedSolenoidStack(SOL_SAUCER, 10, currentTime + 1000, true);
            ValidateAndRegisterPlayfieldSwitch(currentTime);
            SetMode(GAME_MODE_UNSTRUCTURED_PLAY, currentTime);
        }
        break;
    }
}

void GameplayManager::ShowGameplayLamps(uint32_t currentTime)
{
    ShowShootAgainLamp(currentTime);
    ShowDollyTargetLamps();
    ShowDollyBonusLamps();
    LightPartonBonusLamps();
    ShowPartonBonusLamps();

    ShowLampAnimation(2, 144, currentTime, 23, false, false);
}

void GameplayManager::ConfigureBallSave(uint32_t durationMs, uint8_t numberOfSaves = 0xFF)
{
    ballSaveDurationMs = durationMs;
    numberOfBallSavesAllowed = numberOfSaves;
}

void GameplayManager::EndBallSave()
{
    ballSaveEndTime = 0;
    numberOfBallSavesRemaining = 0;
}

void GameplayManager::ShowShootAgainLamp(uint32_t currentTime)
{       
    if ((ballFirstSwitchHitTime == 0 && ballSaveDurationMs) || (ballSaveEndTime && currentTime < ballSaveEndTime))
    {
        uint32_t msRemaining = 5000;
        if (ballSaveEndTime != 0)
            msRemaining = ballSaveEndTime - currentTime;
        RPU_SetLampState(LAMP_SHOOT_AGAIN, 1, 0, (msRemaining < 5000) ? 100 : 500);
    }
    else
    {
        RPU_SetLampState(LAMP_SHOOT_AGAIN, samePlayerShootsAgain);
    }
}

void GameplayManager::ShowDollyTargetLamps()
{
    /*
    if (DollyTargetsComplete[currentPlayer] < 5 && RPU_ReadLampState(LAMP_D_TARGET) && RPU_ReadLampState(LAMP_O_TARGET) && RPU_ReadLampState(LAMP_L1_TARGET) && RPU_ReadLampState(LAMP_L2_TARGET) && RPU_ReadLampState(LAMP_Y_TARGET))
    {
        DollyTargetsComplete[currentPlayer] += 1;
        RPU_SetLampState(LAMP_D_TARGET, 0, 0, 0);
        RPU_SetLampState(LAMP_O_TARGET, 0, 0, 0);
        RPU_SetLampState(LAMP_L1_TARGET, 0, 0, 0);
        RPU_SetLampState(LAMP_L2_TARGET, 0, 0, 0);
        RPU_SetLampState(LAMP_Y_TARGET, 0, 0, 0);
        ShowDollyBonusLamps();
    }
    else if (DollyTargetsComplete[currentPlayer] > 5)
    {
        DollyTargetsComplete[currentPlayer] == 5;
    }
        */
}

void GameplayManager::ShowDollyBonusLamps()
{
    /*
    if (DollyTargetsComplete[currentPlayer] == 1)
    {
        RPU_SetLampState(LAMP_D_CENTER, true);
        RPU_SetLampState(LAMP_DOLLY_O_CENTER, false);
        RPU_SetLampState(LAMP_1ST_L_CENTER, false);
        RPU_SetLampState(LAMP_2ND_L_CENTER, false);
        RPU_SetLampState(LAMP_Y_CENTER, false);
        RPU_SetLampState(LAMP_1K_DOLLY_TARGET, true);
    }
    else if (DollyTargetsComplete[currentPlayer] == 2)
    {
        RPU_SetLampState(LAMP_D_CENTER, true);
        RPU_SetLampState(LAMP_DOLLY_O_CENTER, true);
        RPU_SetLampState(LAMP_1ST_L_CENTER, false);
        RPU_SetLampState(LAMP_2ND_L_CENTER, false);
        RPU_SetLampState(LAMP_Y_CENTER, false);
        RPU_SetLampState(LAMP_1K_DOLLY_TARGET, true);
    }
    else if (DollyTargetsComplete[currentPlayer] == 3)
    {
        RPU_SetLampState(LAMP_D_CENTER, true);
        RPU_SetLampState(LAMP_DOLLY_O_CENTER, true);
        RPU_SetLampState(LAMP_1ST_L_CENTER, true);
        RPU_SetLampState(LAMP_2ND_L_CENTER, false);
        RPU_SetLampState(LAMP_Y_CENTER, false);
        RPU_SetLampState(LAMP_1K_DOLLY_TARGET, true);
    }
    else if (DollyTargetsComplete[currentPlayer] == 4)
    {
        RPU_SetLampState(LAMP_D_CENTER, true);
        RPU_SetLampState(LAMP_DOLLY_O_CENTER, true);
        RPU_SetLampState(LAMP_1ST_L_CENTER, true);
        RPU_SetLampState(LAMP_2ND_L_CENTER, true);
        RPU_SetLampState(LAMP_Y_CENTER, false);
        RPU_SetLampState(LAMP_1K_DOLLY_TARGET, true);
    }
    else if (DollyTargetsComplete[currentPlayer] == 5)
    {
        RPU_SetLampState(LAMP_D_TARGET, 1, 0, 500);
        RPU_SetLampState(LAMP_O_TARGET, 1, 0, 500);
        RPU_SetLampState(LAMP_L1_TARGET, 1, 0, 500);
        RPU_SetLampState(LAMP_L2_TARGET, 1, 0, 500);
        RPU_SetLampState(LAMP_Y_TARGET, 1, 0, 500);
        RPU_SetLampState(LAMP_D_CENTER, 1, 0, 500);
        RPU_SetLampState(LAMP_DOLLY_O_CENTER, 1, 0, 500);
        RPU_SetLampState(LAMP_1ST_L_CENTER, 1, 0, 500);
        RPU_SetLampState(LAMP_2ND_L_CENTER, 1, 0, 500);
        RPU_SetLampState(LAMP_Y_CENTER, 1, 0, 500);
        RPU_SetLampState(LAMP_1K_DOLLY_TARGET, true);
        RPU_SetLampState(LAMP_44K_BONUS, true);
    }
    else
    {
        RPU_SetLampState(LAMP_D_CENTER, false);
        RPU_SetLampState(LAMP_DOLLY_O_CENTER, false);
        RPU_SetLampState(LAMP_1ST_L_CENTER, false);
        RPU_SetLampState(LAMP_2ND_L_CENTER, false);
        RPU_SetLampState(LAMP_Y_CENTER, false);
    }
    */
}

void GameplayManager::LightPartonBonusLamps()
{
    if (RPU_ReadLampState(LAMP_P_SAUCER) && RPU_ReadLampState(LAMP_A_SAUCER) && RPU_ReadLampState(LAMP_R_SAUCER) && RPU_ReadLampState(LAMP_T_SAUCER) && RPU_ReadLampState(LAMP_O_SAUCER) && RPU_ReadLampState(LAMP_N_SAUCER))
    {
        playerProgress.AddScore(currentPlayer, 50000);
    }
    else if (RPU_ReadLampState(LAMP_P_SAUCER))
    {
        RPU_SetLampState(LAMP_P_CENTER, true);
    }
    else if (RPU_ReadLampState(LAMP_A_SAUCER))
    {
        RPU_SetLampState(LAMP_A_CENTER, true);
    }
    else if (RPU_ReadLampState(LAMP_R_SAUCER))
    {
        RPU_SetLampState(LAMP_R_CENTER, true);
    }
    else if (RPU_ReadLampState(LAMP_T_SAUCER))
    {
        RPU_SetLampState(LAMP_T_CENTER, true);
    }
    else if (RPU_ReadLampState(LAMP_O_SAUCER))
    {
        RPU_SetLampState(LAMP_PARTON_O_CENTER, true);
    }
    else if (RPU_ReadLampState(LAMP_N_SAUCER))
    {
        RPU_SetLampState(LAMP_N_CENTER, true);
    }
}

void GameplayManager::ShowPartonBonusLamps()
{
    if (RPU_ReadLampState(LAMP_P_SAUCER) && RPU_ReadLampState(LAMP_A_SAUCER) && RPU_ReadLampState(LAMP_R_SAUCER) && RPU_ReadLampState(LAMP_T_SAUCER) && RPU_ReadLampState(LAMP_O_SAUCER) && RPU_ReadLampState(LAMP_N_SAUCER))
    {
        RPU_SetLampState(LAMP_22K_BONUS, true);
    }
}

void GameplayManager::ValidateAndRegisterPlayfieldSwitch(uint32_t currentTime)
{
    lastSwitchHitTime = currentTime;
    if (ballFirstSwitchHitTime == 0)
        ballFirstSwitchHitTime = currentTime;
}

void GameplayManager::AddToCurrentScore(uint32_t points, bool useMultiplier)
{
    uint32_t actualPoints = useMultiplier ? points * playfieldMultiplier : points;
    playerProgress.AddScore(currentPlayer, actualPoints);
}
