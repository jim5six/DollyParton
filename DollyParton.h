#ifndef DOLLY_PARTON_H
#define DOLLY_PARTON_H

//LAMP DEFINITIONS:
#define LAMP_D_TARGET                 0
#define LAMP_O_TARGET                 1
#define LAMP_L1_TARGET                2
#define LAMP_L2_TARGET                3
#define LAMP_Y_TARGET                 4
#define LAMP_P_SAUCER                 5
#define LAMP_A_SAUCER                 6
#define LAMP_R_SAUCER                 7
#define LAMP_T_SAUCER                 8
#define LAMP_O_SAUCER                 9
#define LAMP_N_SAUCER                 10

#define LAMP_5K_PARTON_SAUCER         13
#define LAMP_1K_DOLLY_TARGET          14
#define LAMP_SPINNER_1K               15
#define LAMP_20K_DROP_TARGET          16
#define LAMP_22K_BONUS                17
#define LAMP_44K_BONUS                18
#define LAMP_SPECIAL_DOLLY_PARTON     19
#define LAMP_R_INLANE                 20
#define LAMP_R_OUTLANE                21
#define LAMP_L_INLANE                 22
#define LAMP_L_OUTLANE                23
#define LAMP_D_CENTER                 24
#define LAMP_DOLLY_O_CENTER           25
#define LAMP_1ST_L_CENTER             26
#define LAMP_2ND_L_CENTER             27
#define LAMP_Y_CENTER                 28
#define LAMP_P_CENTER                 29
#define LAMP_A_CENTER                 30
#define LAMP_R_CENTER                 31
#define LAMP_T_CENTER                 32
#define LAMP_PARTON_O_CENTER          33
#define LAMP_N_CENTER                 34

#define LAMP_EB_DROP_TARGET           38
#define LAMP_SPECIAL_DROP_TARGET      39
#define LAMP_HEAD_SHOOT_AGAIN         40
#define LAMP_MATCH                    41
#define LAMP_SHOOT_AGAIN              42
#define LAMP_CREDIT_LIGHT             43
#define LAMP_2X_BONUS                 44
#define LAMP_3X_BONUS                 45
#define LAMP_5X_BONUS                 46

#define LAMP_BALL_IN_PLAY             48
#define LAMP_HSTD                     49  //  JIM - NEED TO VERIFY
#define LAMP_GAME_OVER                50
#define LAMP_TILT                     51

//Switch Definitions
#define SW_DROP_4                   0
#define SW_DROP_3                   1
#define SW_DROP_2                   2
#define SW_DROP_1                   3
#define SW_SUPERSTAR                4
#define SW_CREDIT_RESET             5
#define SW_TILT                     6
#define SW_PLUMB_TILT               6
#define SW_ROLL_TILT                6
#define SW_OUTHOLE                  7
#define SW_COIN_3                   8
#define SW_COIN_1                   9
#define SW_COIN_2                   10

#define SW_SLAM                     15
#define SW_SPINNER                  16

#define SW_LOWER_L_TARGET           18
#define SW_RIGHT_OUTLANE            19
#define SW_RIGHT_INLANE             20
#define SW_LEFT_INLANE              21
#define SW_LEFT_OUTLANE             22
#define SW_SAUCER                   23

#define SW_Y                        27
#define SW_L2                       28
#define SW_L1                       29
#define SW_O                        30
#define SW_D                        31

#define SW_RUBBER                   33
#define SW_LOWER_R_LANE             34
#define SW_RIGHT_SLING              35
#define SW_LEFT_SLING               36
#define SW_T_POP_BUMPER             37
#define SW_B_POP_BUMPER             38
#define SW_L_POP_BUMPER             39

//Solinoid Definitions
#define SOL_KNOCKER                 5
#define SOL_BALL_TROUGH             6
#define SOL_B_POP_BUMPER            7
#define SOL_L_POP_BUMPER            8
#define SOL_T_POP_BUMPER            9
#define SOL_LEFT_SLING              10
#define SOL_RIGHT_SLING             11
#define SOL_SAUCER                  12

#define SOL_DROP_BANK_RESET         14
#define SOL_COIN_LOCKOUT            15

enum DollyLetter
{
    DOLLY_D,
    DOLLY_O,
    DOLLY_L1,
    DOLLY_L2,
    DOLLY_Y,
    DOLLY_LETTERS_COUNT
};

enum PartonLetter
{
    PARTON_P,
    PARTON_A,
    PARTON_R,
    PARTON_T,
    PARTON_O,
    PARTON_N,
    PARTON_LETTERS_COUNT
};

inline uint8_t GetDollyTargetsLampID(DollyLetter letter)
{
    switch (letter)
    {
        case DOLLY_D: return LAMP_D_TARGET;
        case DOLLY_O: return LAMP_O_TARGET;
        case DOLLY_L1: return LAMP_L1_TARGET;
        case DOLLY_L2: return LAMP_L2_TARGET;
        case DOLLY_Y: return LAMP_Y_TARGET;
        default: return 0xFF; // Invalid
    }
}

inline uint8_t GetPartonSaucerLampID(PartonLetter letter)
{
    switch (letter)
    {
        case PARTON_P: return LAMP_P_SAUCER;
        case PARTON_A: return LAMP_A_SAUCER;
        case PARTON_R: return LAMP_R_SAUCER;
        case PARTON_T: return LAMP_T_SAUCER;
        case PARTON_O: return LAMP_O_SAUCER;
        case PARTON_N: return LAMP_N_SAUCER;
        default: return 0xFF; // Invalid
    }
}

inline uint8_t GetCenterDollyLampID(DollyLetter letter)
{
    switch (letter)
    {
        case DOLLY_D: return LAMP_D_CENTER;
        case DOLLY_O: return LAMP_DOLLY_O_CENTER;
        case DOLLY_L1: return LAMP_1ST_L_CENTER;
        case DOLLY_L2: return LAMP_2ND_L_CENTER;
        case DOLLY_Y: return LAMP_Y_CENTER;
        default: return 0xFF; // Invalid
    }
}

inline uint8_t GetCenterPartonLampID(PartonLetter letter)
{
    switch (letter)
    {
        case PARTON_P: return LAMP_P_CENTER;
        case PARTON_A: return LAMP_A_CENTER;
        case PARTON_R: return LAMP_R_CENTER;
        case PARTON_T: return LAMP_T_CENTER;
        case PARTON_O: return LAMP_PARTON_O_CENTER;
        case PARTON_N: return LAMP_N_CENTER;
        default: return 0xFF; // Invalid
    }
}

#endif