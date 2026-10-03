
#define LAMP_P_UPPER                   0  // Q14, J1-18
#define LAMP_A_UPPER                   0  // Q14, J1-18
#define LAMP_R_UPPER                   0  // Q14, J1-18
#define LAMP_T_UPPER                   0  // Q14, J1-18
#define LAMP_O_PARTON_UPPER            0  // Q14, J1-18
#define LAMP_N_UPPER                   0  // Q14, J1-18
#define LAMP_P_LOWER                   0  // Q14, J1-18
#define LAMP_A_LOWER                   0  // Q14, J1-18
#define LAMP_R_LOWER                   0  // Q14, J1-18
#define LAMP_T_LOWER                   0  // Q14, J1-18
#define LAMP_O_PARTON_LOWER            0  // Q14, J1-18
#define LAMP_N_LOWER                   0  // Q14, J1-18
#define LAMP_D_UPPER                   0  // Q14, J1-18
#define LAMP_O_DOLLY_UPPER             0  // Q14, J1-18
#define LAMP_L1_UPPER                  0  // Q14, J1-18
#define LAMP_L2_UPPER                  0  // Q14, J1-18
#define LAMP_Y_UPPER                   0  // Q14, J1-18
#define LAMP_D_LOWER                   0  // Q14, J1-18
#define LAMP_O_DOLLY_LOWER             0  // Q14, J1-18
#define LAMP_L1_LOWER                  0  // Q14, J1-18
#define LAMP_L2_LOWER                  0  // Q14, J1-18
#define LAMP_Y_LOWER                   0  // Q14, J1-18
#define LAMP_PARTON_5000               0  // Q14, J1-18
#define LAMP_DOLLY_1000                0  // Q14, J1-18
#define LAMP_22000                     0  // Q14, J1-18
#define LAMP_44000                     0  // Q14, J1-18
#define LAMP_SPECIAL_CENTER            0  // Q14, J1-18
#define LAMP_EXTRABALL                 0  // Q14, J1-18
#define LAMP_20000                     0  // Q14, J1-18
#define LAMP_SPECIAL_DROPS             0  // Q14, J1-18
#define LAMP_SPINNER                   0  // Q14, J1-18
#define LAMP_2X                        0  // Q14, J1-18
#define LAMP_3X                        0  // Q14, J1-18
#define LAMP_5X                        0  // Q14, J1-18
#define LAMP_HEAD_MATCH               41  // Q23, J2-8
#define LAMP_SHOOT_AGAIN              42  // Q40, J2-9
#define LAMP_APRON_CREDIT             43  // Q52, J2-5
#define LAMP_RIGHT_INLANE             44  // Q7, J2-13
#define LAMP_LEFT_INLANE              45  // Q21, J2-12
#define LAMP_RIGHT_OUTLANE            46  // Q39, J2-4
#define LAMP_LEFT_OUTLANE             47  // Q53, J2-3
#define LAMP_BALL_IN_PLAY             48  // Q16, J2-22
#define LAMP_HEAD_HIGH_SCORE          49  // Q15, J2-23
#define LAMP_HEAD_GAME_OVER           50  // Q33, J2-11
#define LAMP_HEAD_TILT                51  // Q47, J2-10
#define LAMP_HEAD_1_PLAYER            52  // Q5, J2-16
#define LAMP_HEAD_2_PLAYERS           53  // Q18, J2-20
#define LAMP_HEAD_3_PLAYERS           54  // Q30, J2-6
#define LAMP_HEAD_4_PLAYERS           55  // Q43, J2-7
#define LAMP_HEAD_PLAYER_1_UP         56  // Q6, J2-14
#define LAMP_HEAD_PLAYER_2_UP         57  // Q19, J2-15
#define LAMP_HEAD_PLAYER_3_UP         58  // Q31, J2-2
#define LAMP_HEAD_PLAYER_4_UP         59  // Q45, J2-1
#define SW_CREDIT_RESET               5

// These may have different switches, or they might be the same
#define SW_TILT                     6
#define SW_PLUMB_TILT               6
#define SW_ROLL_TILT                6

#define SW_OUTHOLE                  7
#define SW_COIN_3                   8
#define SW_COIN_1                   9
#define SW_COIN_2                   10
#define SW_SLAM                     15
#define SW_DROP_1                   16
#define SW_DROP_2                   17
#define SW_DROP_3                   18
#define SW_DROP_4                   18
#define SW_SUPERSTAR                18
#define SW_LEFT_SLING               19
#define SW_RIGHT_SLING              20
#define SW_L_POP_BUMPER             31
#define SW_R_POP_BUMPER             31
#define SW_B_POP_BUMPER             31
#define SW_SAUCER                   32
#define SW_LEFT_OUTLANE             34
#define SW_LEFT_INLANE              35
#define SW_RIGHT_OUTLANE            37
#define SW_RIGHT_INLANE             36
#define SW_RUBBER                   18
#define SW_D                        18
#define SW_O                        18
#define SW_L1                       18
#define SW_L2                       18
#define SW_Y                        18
#define SW_LOWER_LEFT               18
#define SW_LOWER_R_LANE             18
#define SW_SPINNER                  18


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
  { SW_L_POP_BUMPER, SOL_L_POP_BUMPER, 4}
  { SW_R_POP_BUMPER, SOL_R_POP_BUMPER, 4}
  { SW_B_POP_BUMPER, SOL_B_POP_BUMPER, 4}
};
#endif