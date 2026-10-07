//LAMP DEFINITIONS:

#define LAMP_D_TARGET                 0
#define LAMP_O_TARGET                 1
#define LAMP_1ST_L_TARGET             2
#define LAMP_2ND_L_TARGET             3
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
#define LAMP_DOLLY_0_CENTER           25
#define LAMP_1ST_L_CENTER             26
#define LAMP_2ND_L_CENTER             27
#define LAMP_Y_CENTER                 28
#define LAMP_P_CENTER                 29
#define LAMP_A_CENTER                 30
#define LAMP_R_CENTER                 31
#define LAMP_T_CENTER                 32
#define LAMP_PARTON_0_CENTER          33
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
#define LAMP_GANE_OVER                50
#define LAMP_TILT                     51



// These may have different switches, or they might be the same
#define SW_TILT                     6
#define SW_PLUMB_TILT               6
#define SW_ROLL_TILT                6
#define SW_CREDIT_RESET             5
#define SW_OUTHOLE                  7
#define SW_COIN_3                   8
#define SW_COIN_1                   9
#define SW_COIN_2                   10
#define SW_SLAM                     15
#define SW_DROP_1                   16
#define SW_DROP_2                   17
#define SW_DROP_3                   40
#define SW_DROP_4                   41
#define SW_SUPERSTAR                18
#define SW_LEFT_SLING               19
#define SW_RIGHT_SLING              20
#define SW_L_POP_BUMPER             21
#define SW_R_POP_BUMPER             22
#define SW_B_POP_BUMPER             23
#define SW_SAUCER                   32
#define SW_LEFT_OUTLANE             34
#define SW_LEFT_INLANE              35
#define SW_RIGHT_OUTLANE            37
#define SW_RIGHT_INLANE             36
#define SW_RUBBER                   24
#define SW_D                        25
#define SW_O                        26
#define SW_L1                       27
#define SW_L2                       28
#define SW_Y                        29
#define SW_LOWER_LEFT               30
#define SW_LOWER_R_LANE             35
#define SW_SPINNER                  38


#define SOL_L_POP_BUMPER            6
#define SOL_R_POP_BUMPER            6
#define SOL_B_POP_BUMPER            6
#define SOL_DROP_BANK_RESET         2
#define SOL_KNOCKER                 4
#define SOL_SAUCER                  7
#define SOL_LEFT_SLING              10
#define SOL_RIGHT_SLING             11
#define SOL_BALL_TROUGH             8


// These SolenoidAssociatedSwitches are only
// used for Bally/Stern. On Williams, the "immediate solenoids"
// are activated in hardware so they don't need this
// code
#if (RPU_MPU_ARCHITECTURE<10)
#define NUM_SWITCHES_WITH_TRIGGERS          5 // total number of solenoid/switch pairs
#define NUM_PRIORITY_SWITCHES_WITH_TRIGGERS 6 // This number should match the define above

struct PlayfieldAndCabinetSwitch SolenoidAssociatedSwitches[] = {
  { SW_RIGHT_SLING, SOL_RIGHT_SLING, 4},
  { SW_LEFT_SLING, SOL_LEFT_SLING, 4},
  { SW_L_POP_BUMPER, SOL_L_POP_BUMPER, 4},
  { SW_R_POP_BUMPER, SOL_R_POP_BUMPER, 4},
  { SW_B_POP_BUMPER, SOL_B_POP_BUMPER, 4}
};
#endif