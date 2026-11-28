#include "GameplayManager.h"
#include "RPU.h"
#include "DollyParton.h"
#include "DisplayHandler.h"
#include "AudioHandler.h"
#include "PlayerProgress.h"
#include "SoundIDs.h"
#include "LampAnimationAPI.h"

enum GameplayModuleMode
{
    GAME_MODE_SKILL_SHOT,
    GAME_MODE_UNSTRUCTURED_PLAY,
    GAME_MODE_SONG
};

static const uint32_t BALL_SAVE_GRACE_PERIOD = 2000;
static const uint32_t skillShotDurationMs = 10000;
static const uint32_t songAwardCountdownInterval = 100;
static const uint32_t songAwardScoreTick = 200;
static const uint32_t showSongDisplayTimeMs = 4000;
static const uint8_t maxSongBonusesAllowed = 3;

static uint8_t currentPlayer = 0;
static GameplayModuleMode currentMode = GAME_MODE_SKILL_SHOT;
static uint32_t currentModeStartTime = 0;

static SongMode activeSong = SONG_MODE_NONE;
static uint32_t songMusicStartTime = 0;
static uint32_t showNextSongDisplayEnd = 0;
static uint32_t accumulatedSongBonus = 0;
static uint8_t songBonusX = 1;
static uint8_t songBonusesAwarded = 0;
static bool songBonusAwarding = false;
static uint32_t nextSongBonusAwardTime = 0;

static uint32_t ballSaveDurationMs = 15000;
static uint32_t ballSaveEndTime = 0;
static uint8_t numberOfBallSavesAllowed = 0xFF;
static uint8_t numberOfBallSavesRemaining = 0;
static uint8_t playfieldMultiplier = 1;
static bool samePlayerShootsAgain = false;
static bool spinnerIsLit = false;

static uint32_t lastSwitchHitTime = 0;
static uint32_t ballFirstSwitchHitTime = 0;
static bool ballDollyStandupProgress[DOLLY_LETTERS_COUNT] = {};
static uint8_t ballBonusX = 1;

static AudioHandler *audio = 0;

static void SetMode(GameplayModuleMode newMode, uint32_t currentTime);
static void ShowShootAgainLamp(uint32_t currentTime);
static void ShowSpinnerLamp();
static void ShowSaucerLamps(uint32_t currentTime);
static void ShowDollyTargetLamps();
static void ShowDollyBonusLamps();
static void LightPartonBonusLamps();
static void ShowPartonBonusLamps();
static void ShowBonusXLamps();
static void AddToCurrentScore(uint32_t points, bool useMultiplier = true);
static void AddToSongBonus(uint32_t points, bool useMultiplier = true);
static void ValidateAndRegisterPlayfieldSwitch(uint32_t currentTime);
static void PlaySound(unsigned short soundIndex);
static void SpotDollyStandupLetter(DollyLetter letter);
static void AdvanceBonusX();
static void AdvanceSongBonusX();
static void StartSongMode(uint32_t currentTime);
static bool SongModeIsActive();


/************************************************************** 
* 
*    Private (static) functions
*
*/

void Gameplay_Initialize(AudioHandler &audioHandler)
{
    audio = &audioHandler;
}

static void SetMode(GameplayModuleMode newMode, uint32_t currentTime)
{
    currentMode = newMode;
    currentModeStartTime = currentTime;
}

static void PlaySound(unsigned short soundIndex)
{
    if (audio != 0)
        audio->PlaySound(soundIndex, AUDIO_PLAY_TYPE_WAV_TRIGGER);
}

void Gameplay_GetCurrentPlayer(uint8_t playerNumber)
{
    currentPlayer = playerNumber;
}

void StartBallSave(uint32_t currentTime)
{
    ballSaveEndTime = currentTime + ballSaveDurationMs;
    numberOfBallSavesRemaining = 1;
}

bool Gameplay_BallSaveIsActive(uint32_t currentTime)
{
    return ballSaveEndTime && currentTime < ballSaveEndTime + BALL_SAVE_GRACE_PERIOD;
}

void Gameplay_StartBallSave(uint32_t currentTime)
{
    ballSaveEndTime = currentTime + ballSaveDurationMs;
    numberOfBallSavesRemaining = 1;
}

void Gameplay_StartBall(uint32_t currentTime, uint32_t playerUp)
{
    currentPlayer = playerUp;
        ballFirstSwitchHitTime = 0; // Reset the first switch hit time
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

void Gameplay_EndBall(uint32_t currentTime)
{
    // Do this just so we are sure to leave a song mode
    SetMode(GAME_MODE_UNSTRUCTURED_PLAY, currentTime);
    audio->StopAllMusic();
}

// Get the index of the available display to the left or right.
uint8_t Gameplay_GetAdjacentDisplayIndex(uint8_t currentPlayer)
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
bool Gameplay_ManageGameMode(uint32_t currentTime)
{
    if (!Gameplay_BallSaveIsActive(currentTime))
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

    if (songMusicStartTime && activeSong != SONG_MODE_NONE && currentTime > songMusicStartTime)
    {
        songMusicStartTime = 0;
        uint8_t songNum = static_cast<uint8_t>(activeSong);
        audio->PlayBackgroundSong(SOUND_EFFECT_JOLENE + songNum);
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
                audio->StopAllMusic();
                // TODO: Play a new background song
                audio->QueuePrioritizedNotification(SOUND_EFFECT_SONG_MODE_COMPLETE, 0, 10, currentTime);
                SetMode(GAME_MODE_UNSTRUCTURED_PLAY, currentTime);
                activeSong = SONG_MODE_NONE;
            }
        }
    }

    if (SongModeIsActive())
    {
        Display_OverrideScoreDisplay(Gameplay_GetAdjacentDisplayIndex(currentPlayer), accumulatedSongBonus, 0);
    }
    else
    {
        if (currentTime < showNextSongDisplayEnd)
        {
            uint8_t songNum = static_cast<uint8_t>(Player_GetLitSongMode(currentPlayer));
            Display_OverrideScoreDisplay(Gameplay_GetAdjacentDisplayIndex(currentPlayer), 500 + songNum, 0);
        }
        // Not sure if I should be calling this all the time...but let's do it for now.
        else
        {
            Display_ClearOverride(Gameplay_GetAdjacentDisplayIndex(currentPlayer));
        }
    }

    return true;
}

void Gameplay_HandleSwitches(uint8_t switchHit, uint32_t currentTime)
{
    switch (switchHit)
    {

    case SW_T_POP_BUMPER:
        AddToCurrentScore(10);
        PlaySound(SOUND_EFFECT_SNARE);
        if (activeSong == SONG_MODE_JOLENE)
        {
            AddToSongBonus(200);
        }
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_L_POP_BUMPER:
        AddToCurrentScore(20);
        PlaySound(SOUND_EFFECT_BASS);
        if (activeSong == SOUND_EFFECT_BASS)
        {
            AddToSongBonus(200);
        }
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_B_POP_BUMPER:
        AddToCurrentScore(30);
        PlaySound(SOUND_EFFECT_HIGHHAT);
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
        PlaySound(SOUND_EFFECT_LOW_E_STRING);
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
        PlaySound(SOUND_EFFECT_LOW_E_STRING);
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
        PlaySound(SOUND_EFFECT_BASS);
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
        PlaySound(SOUND_EFFECT_BASS);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_LOWER_L_TARGET:
        AddToCurrentScore(500);
        PlaySound(SOUND_EFFECT_G_STRING);
        if (!SongModeIsActive())
        {
            Player_CycleLitSongMode(currentPlayer);
            showNextSongDisplayEnd = currentTime + showSongDisplayTimeMs;
            audio->QueuePrioritizedNotification(SOUND_EFFECT_SONG_MODE_CHANGED, 0, 10, currentTime);
        }
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_LOWER_R_LANE:
        AddToCurrentScore(5000);
        if (SongModeIsActive()) 
        { 
            songBonusAwarding = true; 
            nextSongBonusAwardTime = currentTime + songAwardCountdownInterval;
        }
        else
        {
            PlaySound(SOUND_EFFECT_HIGH_E_STRING);
        }
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_LEFT_SLING:
        AddToCurrentScore(100);
        PlaySound(SOUND_EFFECT_A_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_RIGHT_SLING:
        AddToCurrentScore(100);
        PlaySound(SOUND_EFFECT_B_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_DROP_1:
        AddToCurrentScore(500);
        spinnerIsLit = true;
        PlaySound(SOUND_EFFECT_G_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_DROP_2:
        AdvanceBonusX();
        if (SongModeIsActive()) { AdvanceSongBonusX(); }
        AddToCurrentScore(500);
        PlaySound(SOUND_EFFECT_A_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_DROP_3:
        AdvanceBonusX();
        if (SongModeIsActive()) { AdvanceSongBonusX(); }
        AddToCurrentScore(500);
        PlaySound(SOUND_EFFECT_B_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_DROP_4:
        AdvanceBonusX();
        if (SongModeIsActive()) { AdvanceSongBonusX(); }
        AddToCurrentScore(500);
        PlaySound(SOUND_EFFECT_HIGH_E_STRING);
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_SUPERSTAR:
        AddToCurrentScore(5000);
        PlaySound(SOUND_EFFECT_SONG_MODE_START);
        RPU_PushToTimedSolenoidStack(SOL_DROP_BANK_RESET, 10, currentTime + 5000, true);

        if (!SongModeIsActive())
        {
            StartSongMode(currentTime);
        }
        break;

    case SW_RUBBER:
        AddToCurrentScore(30);
        PlaySound(SOUND_EFFECT_SNARE);
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
        PlaySound(SOUND_EFFECT_LOW_E_STRING);
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
        PlaySound(SOUND_EFFECT_G_STRING);
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
        PlaySound(SOUND_EFFECT_A_STRING);
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
        PlaySound(SOUND_EFFECT_B_STRING);
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
        PlaySound(SOUND_EFFECT_HIGH_E_STRING);
        SpotDollyStandupLetter(DOLLY_Y);

        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;

    case SW_SPINNER:
        if (spinnerIsLit)
        {
            AddToCurrentScore(1000);
            PlaySound(SOUND_EFFECT_HIGH_E_STRING);
        }
        else
        {
            AddToCurrentScore(100);
            PlaySound(SOUND_EFFECT_LOW_E_STRING);
        }
        if (activeSong == SONG_MODE_JOSHUA)
        {
            AddToSongBonus(200);
        }
    
        ValidateAndRegisterPlayfieldSwitch(currentTime);
        break;
        
    case SW_SAUCER:
        if (activeSong == SONG_MODE_COAT_OF_MANY_COLORS)
        {
            AddToSongBonus(3000);
            PlaySound(SOUND_EFFECT_LOW_E_STRING);
            RPU_PushToTimedSolenoidStack(SOL_SAUCER, 10, currentTime + 1000, true);
        }
        if (currentMode != GAME_MODE_SKILL_SHOT)
        {
            AddToCurrentScore(1000);
            PlaySound(SOUND_EFFECT_SNARE);
            RPU_PushToTimedSolenoidStack(SOL_SAUCER, 10, currentTime + 1000, true);
        }
        else
        {
            AddToCurrentScore(1000);
            PlaySound(SOUND_EFFECT_LOW_E_STRING);
            RPU_PushToTimedSolenoidStack(SOL_SAUCER, 10, currentTime + 1000, true);
        }

        if (RPU_ReadLampState(LAMP_P_SAUCER))
        {
            Player_SpotPartonLetter(currentPlayer, PARTON_P);
        }
        else if (RPU_ReadLampState(LAMP_A_SAUCER))
        {
            Player_SpotPartonLetter(currentPlayer, PARTON_A);
        }
        else if (RPU_ReadLampState(LAMP_R_SAUCER))
        {
            Player_SpotPartonLetter(currentPlayer, PARTON_R);
        }
        else if (RPU_ReadLampState(LAMP_T_SAUCER))
        {
            Player_SpotPartonLetter(currentPlayer, PARTON_T);
        }
        else if (RPU_ReadLampState(LAMP_O_SAUCER))
        {
            Player_SpotPartonLetter(currentPlayer, PARTON_O);
        }
        else if (RPU_ReadLampState(LAMP_N_SAUCER))
        {
            Player_SpotPartonLetter(currentPlayer, PARTON_N);
        }

        ValidateAndRegisterPlayfieldSwitch(currentTime);

        break;
    }
}

void Gameplay_ShowLamps(uint32_t currentTime)
{
    ShowShootAgainLamp(currentTime);
    ShowSaucerLamps(currentTime);
    ShowSpinnerLamp();
    ShowDollyTargetLamps();
    ShowDollyBonusLamps();
    ShowBonusXLamps();
    ShowPartonBonusLamps();

    ShowLampAnimation(2, 144, currentTime, 23, false, false);
}

void Gameplay_ConfigureBallSave(uint32_t durationMs, uint8_t numberOfSaves)
{
    ballSaveDurationMs = durationMs;
    numberOfBallSavesAllowed = numberOfSaves;
}

void Gameplay_EndBallSave()
{
    ballSaveEndTime = 0;
    numberOfBallSavesRemaining = 0;
}

bool Gameplay_PlayfieldValidated()
{
    return (ballFirstSwitchHitTime != 0);
}

uint8_t Gameplay_GetActiveBonusX()
{
    return ballBonusX;
}

/************************************************************** 
* 
*    Private (static) functions
*
*/

static void ShowShootAgainLamp(uint32_t currentTime)
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

static void ShowSpinnerLamp()
{
    if (activeSong == SONG_MODE_JOSHUA)
    {
        RPU_SetLampState(LAMP_SPINNER_1K, 1, 0, 250);
    }
    else
    {
        RPU_SetLampState(LAMP_SPINNER_1K, spinnerIsLit, 0, 0);
    }
}

static void ShowSaucerLamps(uint32_t currentTime)
{
    if (activeSong == SONG_MODE_COAT_OF_MANY_COLORS)
    {
        RPU_SetLampState(LAMP_P_SAUCER, 1, 0, 250);
        RPU_SetLampState(LAMP_A_SAUCER, 1, 0, 250);
        RPU_SetLampState(LAMP_R_SAUCER, 1, 0, 250);
        RPU_SetLampState(LAMP_T_SAUCER, 1, 0, 250);
        RPU_SetLampState(LAMP_O_SAUCER, 1, 0, 250);
        RPU_SetLampState(LAMP_N_SAUCER, 1, 0, 250);
    }
    else
    {
        ShowLampAnimation(2, 144, currentTime, 23, false, false);
    }
}

static void ShowDollyTargetLamps()
{
    for (uint8_t i = 0; i < DOLLY_LETTERS_COUNT; i++)
    {
        RPU_SetLampState(LAMP_D_TARGET + i, ballDollyStandupProgress[i]);
    }
}

static void ShowDollyBonusLamps()
{
    for (uint8_t i = 0; i < DOLLY_LETTERS_COUNT; i++)
    {
        RPU_SetLampState(LAMP_D_CENTER + i, Player_IsDollyLetterSpotted(currentPlayer, static_cast<DollyLetter>(i)));
    }
}

static void ShowBonusXLamps()
{
    if (SongModeIsActive())
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

static void LightPartonBonusLamps()
{
    if (RPU_ReadLampState(LAMP_P_SAUCER) && RPU_ReadLampState(LAMP_A_SAUCER) && RPU_ReadLampState(LAMP_R_SAUCER) && RPU_ReadLampState(LAMP_T_SAUCER) && RPU_ReadLampState(LAMP_O_SAUCER) && RPU_ReadLampState(LAMP_N_SAUCER))
    {
        Player_AddScore(currentPlayer, 50000);
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

static void ShowPartonBonusLamps()
{
    RPU_SetLampState(LAMP_P_CENTER, Player_IsPartonLetterSpotted(currentPlayer, PARTON_P));
    RPU_SetLampState(LAMP_A_CENTER, Player_IsPartonLetterSpotted(currentPlayer, PARTON_A));
    RPU_SetLampState(LAMP_R_CENTER, Player_IsPartonLetterSpotted(currentPlayer, PARTON_R));
    RPU_SetLampState(LAMP_T_CENTER, Player_IsPartonLetterSpotted(currentPlayer, PARTON_T));
    RPU_SetLampState(LAMP_PARTON_O_CENTER, Player_IsPartonLetterSpotted(currentPlayer, PARTON_O));
    RPU_SetLampState(LAMP_N_CENTER, Player_IsPartonLetterSpotted(currentPlayer, PARTON_N));
}

static void ValidateAndRegisterPlayfieldSwitch(uint32_t currentTime)
{
    lastSwitchHitTime = currentTime;
    if (ballFirstSwitchHitTime == 0)
    {
        ballFirstSwitchHitTime = currentTime;
        StartBallSave(currentTime);
    }
}

static void AddToCurrentScore(uint32_t points, bool useMultiplier)
{
    uint32_t actualPoints = useMultiplier ? points * playfieldMultiplier : points;
    Player_AddScore(currentPlayer, actualPoints);
}

static void AddToSongBonus(uint32_t points, bool useMultiplier)
{
    uint32_t actualPoints = useMultiplier ? points * songBonusX : points;
    accumulatedSongBonus += actualPoints;
}

static void SpotDollyStandupLetter(DollyLetter letter)
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
        Player_SpotNextDollyLetter(currentPlayer);

        for (uint8_t i = 0; i < DOLLY_LETTERS_COUNT; i++)
        {
            ballDollyStandupProgress[i] = false;
        }
    }
}

static void AdvanceBonusX()
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

static void AdvanceSongBonusX()
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

static void StartSongMode(uint32_t currentTime)
{
    showNextSongDisplayEnd = 0;

    SetMode(GAME_MODE_SONG, currentTime);
    activeSong = Player_GetLitSongMode(currentPlayer);

    audio->QueuePrioritizedNotification(SOUND_EFFECT_SONG_MODE_START, 0, 10, currentTime);

    songMusicStartTime = currentTime + 1500;
}

static bool SongModeIsActive()
{
    return (activeSong != SONG_MODE_NONE && activeSong != SONG_MODE_COUNT);
}
