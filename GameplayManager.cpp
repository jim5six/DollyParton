#include "GameplayManager.h"
#include "RPU.h"
#include "DollyParton.h"
#include "DisplayHandler.h"
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
    lastSwitchHitTime = 0;
    ballSaveEndTime = 0;
    SetMode(GAME_MODE_SKILL_SHOT, currentTime);
    numberOfBallSavesAllowed = 0xFF;
    ballBonusX = 1;
    songBonusX = 1;
    spinnerIsLit = false;
    activeSong = SONG_MODE_NONE;
    accumulatedSongBonus = 0;
    songBonusAwarding = false;
    nextSongBonusAwardTime = 0;
    showNextSongDisplayEnd = 0;

    for (uint8_t i = 0; i < DOLLY_LETTERS_COUNT; i++)
    {
        ballDollyStandupProgress[i] = false;
    }
}

void GameplayManager::EndBall(uint32_t currentTime)
{
    // Do this just so we are sure to leave a song mode
    SetMode(GAME_MODE_UNSTRUCTURED_PLAY, currentTime);
    AudioHandler.StopAllMusic();
}

// Get the index of the available display to the left or right.
static byte GameplayManager::GetAdjacentDisplayIndex(byte currentPlayer)
{
    if (currentPlayer == 0)
    {
        return 1;
    }
    else if (currentPlayer == 1)
    {
        return 0;
    }
    else if (currentPlayer == 2)
    {
        return 3;
    }
    else
    {
        return 4;
    }
}

// The main gameplay handler that should be called every cycle while in game mode. 
// Returns TRUE if the ball is still in play
bool GameplayManager::ManageGameMode(uint32_t currentTime)
{
    if (!BallSaveIsActive(currentTime))
    {
        ballSaveEndTime = 0; //TODO: Can do this cleaner, but other code uses end time as 0 to show it's not active
    }

    // End skill shot mode after 10 seconds, if the player hasn't hit a switch yet
    if (currentMode == GAME_MODE_SKILL_SHOT)
    {
        if (currentTime - currentModeStartTime > skillShotDurationMs)
        {
            SetMode(GAME_MODE_UNSTRUCTURED_PLAY, currentTime);
        }
    }

    // Trickle the song award bonus over to score if it's awarding
    if (songBonusAwarding && nextSongBonusAwardTime && currentTime >= nextSongBonusAwardTime && accumulatedSongBonus > 0)
    {
        // TODO: Play a noise here, like applause?

        uint32_t awardAmount = (accumulatedSongBonus >= songAwardScoreTick) ? songAwardScoreTick : accumulatedSongBonus;
        accumulatedSongBonus -= awardAmount;
        AddToCurrentScore(awardAmount, false);
        nextSongBonusAwardTime = currentTime + songAwardCountdownInterval;

        if (accumulatedSongBonus == 0)
        {
            nextSongBonusAwardTime = 0;
            songBonusAwarding = false;
            songBonusesAwarded += 1;

            if (songBonusesAwarded == maxSongBonusesAllowed)
            {

            }
        }
    }

    if (SongModeIsActve())
    {
        Display_OverrideScoreDisplay(GetAdjacentDisplayIndex(currentPlayer), accumulatedSongBonus, 0);
    }
    else
    {
        if (currentTime < showNextSongDisplayEnd)
        {
            uint8_t songNum = static_cast<uint8_t>GetLitSongMode(currentPlayer);
            Display_OverrideScoreDisplay(GetAdjacentDisplayIndex(currentPlayer), accumulatedSongBonus, 500 + songNum);
        }
        // Not sure if I should be calling this all the time...but let's do it for now.
        Display_ClearOverride(GameplayManager::GetAdjacentDisplayIndex(CurrentPlayer));
    }

    return true;
}

void GameplayManager::HandleGameplaySwitches(byte switchHit, uint32_t currentTime)
{
    switch (switchHit)
    {

    case SW_T_POP_BUMPER:
        AddToCurrentScore(10);
        PlaySoundEffect(SOUND_EFFECT_SNARE);
        if (activeSong == SONG_MODE_JOLENE)
        {
            AddToSongBonus(200);
        }
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_L_POP_BUMPER:
        AddToCurrentScore(20);
        PlaySoundEffect(SOUND_EFFECT_BASS);
        if (activeSong == SOUND_EFFECT_BASS)
        {
            AddToSongBonus(200);
        }
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_B_POP_BUMPER:
        AddToCurrentScore(30);
        PlaySoundEffect(SOUND_EFFECT_HIGHHAT);
        if (activeSong == SONG_MODE_JOLENE)
        {
            AddToSongBonus(200);
        }
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
        PlaySoundEffect(SOUND_EFFECT_LOW_E_STRING);
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
        PlaySoundEffect(SOUND_EFFECT_LOW_E_STRING);
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
        PlaySoundEffect(SOUND_EFFECT_BASS);
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
        PlaySoundEffect(SOUND_EFFECT_BASS);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_LOWER_L_TARGET:
        AddToCurrentScore(500);
        PlaySoundEffect(SOUND_EFFECT_G_STRING);
        if (!SongModeIsActve())
        {
            playerProgress.CycleLitSongMode(currentPlayer);
            showNextSongDisplayEnd = currentTime + showSongDisplayTimeMs;
        }
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_LOWER_R_LANE:
        AddToCurrentScore(5000);
        if (SongModeIsActve) 
        { 
            songBonusAwarding = true; 
            nextSongBonusAwardTime = currentTime + songAwardCountdownInterval;
        }
        else
        {
            PlaySoundEffect(SOUND_EFFECT_HIGH_E_STRING);
        }
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_LEFT_SLING:
        AddToCurrentScore(100);
        PlaySoundEffect(SOUND_EFFECT_A_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_RIGHT_SLING:
        AddToCurrentScore(100);
        PlaySoundEffect(SOUND_EFFECT_B_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_DROP_1:
        AddToCurrentScore(500);
        spinnerIsLit = true;
        PlaySoundEffect(SOUND_EFFECT_G_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_DROP_2:
        AdvanceBonusX();
        if (SongModeIsActve()) { AdvanceSongBonusX(); }
        AddToCurrentScore(500);
        PlaySoundEffect(SOUND_EFFECT_A_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_DROP_3:
        AdvanceBonusX();
        if (SongModeIsActve()) { AdvanceSongBonusX(); }
        AddToCurrentScore(500);
        PlaySoundEffect(SOUND_EFFECT_B_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_DROP_4:
        AdvanceBonusX();
        if (SongModeIsActve()) { AdvanceSongBonusX(); }
        AddToCurrentScore(500);
        PlaySoundEffect(SOUND_EFFECT_HIGH_E_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_SUPERSTAR:
        AddToCurrentScore(5000);
        PlaySoundEffect(SOUND_EFFECT_SONG_MODE_START);
        RPU_PushToTimedSolenoidStack(SOL_DROP_BANK_RESET, 10, currentTime + 5000, true);

        if (!SongModeIsActve())
        {
            StartSongMode(currentTime);
        }
        break;

    case SW_RUBBER:
        AddToCurrentScore(30);
        PlaySoundEffect(SOUND_EFFECT_SNARE);
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
        PlaySoundEffect(SOUND_EFFECT_LOW_E_STRING);
        SpotDollyStandupLetter(DOLLY_D);

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
        PlaySoundEffect(SOUND_EFFECT_G_STRING);
        SpotDollyStandupLetter(DOLLY_O);

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
        PlaySoundEffect(SOUND_EFFECT_A_STRING);
        SpotDollyStandupLetter(DOLLY_L1);

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
        PlaySoundEffect(SOUND_EFFECT_B_STRING);
        SpotDollyStandupLetter(DOLLY_L2);

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
        PlaySoundEffect(SOUND_EFFECT_HIGH_E_STRING);
        SpotDollyStandupLetter(DOLLY_Y);

        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_SPINNER:
        if (spinnerIsLit)
        {
            AddToCurrentScore(1000);
            PlaySoundEffect(SOUND_EFFECT_HIGH_E_STRING);
        }
        else
        {
            AddToCurrentScore(100);
            PlaySoundEffect(SOUND_EFFECT_LOW_E_STRING);
        }
        if (activeSong == SONG_MODE_JOLENE)
        {
            AddToSongBonus(200);
        }
    
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
        
    case SW_SAUCER:
        if (currentMode != GAME_MODE_SKILL_SHOT)
        {
            AddToCurrentScore(1000);
            PlaySoundEffect(SOUND_EFFECT_SNARE);
            RPU_PushToTimedSolenoidStack(SOL_SAUCER, 10, currentTime + 1000, true);
            ValidateAndRegisterPlayfieldSwitch(currentTime);
        }
        else
        {
            AddToCurrentScore(1000);
            PlaySoundEffect(SOUND_EFFECT_LOW_E_STRING);
            RPU_PushToTimedSolenoidStack(SOL_SAUCER, 10, currentTime + 1000, true);
            ValidateAndRegisterPlayfieldSwitch(currentTime);
            SetMode(GAME_MODE_UNSTRUCTURED_PLAY, currentTime);
        }

        if (RPU_ReadLampState(LAMP_P_SAUCER))
        {
            playerProgress.SpotPartonLetter(currentPlayer, PARTON_P);
        }
        else if (RPU_ReadLampState(LAMP_A_SAUCER))
        {
            playerProgress.SpotPartonLetter(currentPlayer, PARTON_A);
        }
        else if (RPU_ReadLampState(LAMP_R_SAUCER))
        {
            playerProgress.SpotPartonLetter(currentPlayer, PARTON_R);
        }
        else if (RPU_ReadLampState(LAMP_T_SAUCER))
        {
            playerProgress.SpotPartonLetter(currentPlayer, PARTON_T);
        }
        else if (RPU_ReadLampState(LAMP_O_SAUCER))
        {
            playerProgress.SpotPartonLetter(currentPlayer, PARTON_O);
        }
        else if (RPU_ReadLampState(LAMP_N_SAUCER))
        {
            playerProgress.SpotPartonLetter(currentPlayer, PARTON_N);
        }

        break;
    }
}

void GameplayManager::ShowGameplayLamps(uint32_t currentTime)
{
    ShowShootAgainLamp(currentTime);
    ShowSpinnerLamp();
    ShowDollyTargetLamps();
    ShowDollyBonusLamps();
    ShowBonusXLamps();
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

bool GameplayManager::PlayfieldValidated()
{
    return (ballFirstSwitchHitTime != 0);
}

uint8_t GameplayManager::GetActiveBonusX()
{
    return ballBonusX;
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

void GameplayManager::ShowSpinnerLamp()
{
    RPU_SetLampState(LAMP_SPINNER_1K, spinnerIsLit, 0, 0);
}

void GameplayManager::ShowDollyTargetLamps()
{
    for (uint8_t i = 0; i < DOLLY_LETTERS_COUNT; i++)
    {
        RPU_SetLampState(LAMP_D_TARGET + i, ballDollyStandupProgress[i]);
    }
}

void GameplayManager::ShowDollyBonusLamps()
{
    for (uint8_t i = 0; i < DOLLY_LETTERS_COUNT; i++)
    {
        RPU_SetLampState(LAMP_D_CENTER + i, playerProgress.IsDollyLetterSpotted(currentPlayer, static_cast<DollyLetter>(i)));
    }
}

void GameplayManager::ShowBonusXLamps()
{
    if (SongModeIsActve())
    {
        RPU_SetLampState(LAMP_2X_BONUS, songBonusX >= 2 ? 1 : 0, 0, 250);
        RPU_SetLampState(LAMP_3X_BONUS, songBonusX >= 3 ? 1 : 0, 0, 250);
        RPU_SetLampState(LAMP_5X_BONUS, songBonusX >= 5 ? 1 : 0, 0, 250);
    }
    else 
    {
        RPU_SetLampState(LAMP_2X_BONUS, ballBonusX >= 2 ? 1 : 0);
        RPU_SetLampState(LAMP_3X_BONUS, ballBonusX >= 3 ? 1 : 0);
        RPU_SetLampState(LAMP_5X_BONUS, ballBonusX >= 5 ? 1 : 0);
    }
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
    RPU_SetLampState(LAMP_P_CENTER, playerProgress.IsPartonLetterSpotted(currentPlayer, PARTON_P));
    RPU_SetLampState(LAMP_A_CENTER, playerProgress.IsPartonLetterSpotted(currentPlayer, PARTON_A));
    RPU_SetLampState(LAMP_R_CENTER, playerProgress.IsPartonLetterSpotted(currentPlayer, PARTON_R));
    RPU_SetLampState(LAMP_T_CENTER, playerProgress.IsPartonLetterSpotted(currentPlayer, PARTON_T));
    RPU_SetLampState(LAMP_PARTON_O_CENTER, playerProgress.IsPartonLetterSpotted(currentPlayer, PARTON_O));
    RPU_SetLampState(LAMP_N_CENTER, playerProgress.IsPartonLetterSpotted(currentPlayer, PARTON_N));
}

void GameplayManager::ValidateAndRegisterPlayfieldSwitch(uint32_t currentTime)
{
    lastSwitchHitTime = currentTime;
    if (ballFirstSwitchHitTime == 0)
    {
        ballFirstSwitchHitTime = currentTime;
        StartBallSave(currentTime);
    }
}

void GameplayManager::AddToCurrentScore(uint32_t points, bool useMultiplier)
{
    uint32_t actualPoints = useMultiplier ? points * playfieldMultiplier : points;
    playerProgress.AddScore(currentPlayer, actualPoints);
}

void AddToSongBonus(uint32_t points, bool useMultiplier = true)
{
    uint32_t actualPoints = useMultiplier ? points * playfieldMultiplier : points;
    accumulatedSongBonus += actualPoints;
}

void GameplayManager::SpotDollyStandupLetter(DollyLetter letter)
{
    ballDollyStandupProgress[letter] = true;

    // If all letters are lit, award a permanent center Dolly letter
    bool allLettersLit = true;
    for (uint8_t i = 0; i < DOLLY_LETTERS_COUNT; i++)
    {
        if (!ballDollyStandupProgress[i])
        {
            allLettersLit = false;
            break;
        }
    }
    if (allLettersLit)
    {
        playerProgress.SpotNextDollyLetter(currentPlayer);

        for (uint8_t i = 0; i < DOLLY_LETTERS_COUNT; i++)
        {
            ballDollyStandupProgress[i] = false;
        }
    }
}

void GameplayManager::AdvanceBonusX()
{
    if (ballBonusX == 1)
    {
        ballBonusX = 2;
    }
    else if (ballBonusX == 2)
    {
        ballBonusX = 3;
    }
    else
    {
        ballBonusX = 5;
    }
}

void GameplayManager::AdvanceSongBonusX()
{
    if (songBonusX == 1)
    {
        songBonusX = 2;
    }
    else if (songBonusX == 2)
    {
        songBonusX = 3;
    }
    else
    {
        songBonusX = 5;
    }
}

void GameplayManager::StartSongMode(uint32_t currentTime)
{
    showNextSongDisplayEnd = 0;

    SetMode(GAME_MODE_SONG);
    activeSong = playerProgress.GetLitSongMode(currentPlayer);

    //Start the actual song
    //audioHandler.PlayBackgroundSong(SONG_EFFECT_TODO);
}

bool GameplayManager::SongModeIsActve()
{
    return (activeSong != SONG_MODE_NONE && activeSong != SONG_MODE_COUNT);
}
