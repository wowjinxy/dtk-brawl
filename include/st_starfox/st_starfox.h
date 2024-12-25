#pragma once

#include <memory.h>
#include <types.h>
#include <gf/gf_archive.h>
#include <gm/gm_lib.h>
#include <gr/gr_madein.h>
#include <gr/gr_tengan_event.h>
#include <mt/mt_vector.h>
#include <snd/snd_id.h>
#include <st/se_util.h>
#include <st/stage.h>
#include <st/st_class_info.h>
#include <st/st_melee.h>

ST_CLASS_INFO

class stStarfox : public stMelee {
    grTenganEvent m_scene_lifecycle;
    grTenganEvent m_curr_scene;
    grTenganEvent m_curr_scene_effect;
    u32 m_effects[0x5];
    s32 m_scene_ground_num[0x4];
    grMadein* m_scene_grounds[0x4];
    bool m_change_scene;
    u8 m_smash_taunt_activated;
    IfFoxSmashAppearTask* m_smash_taunt_task;
    u8 m_smash_taunt_kind;
    s32 m_scene_num;
    s32 m_prev_scene_num;
    u32 m_unused;
    u8 m_not_first_scene: 1;
    u8 m_show_dogfight: 1;
    u8 m_smash_taunt_timing: 1;
    StSeUtil::SeSeqInstance<4, 40> m_se_player;
    s32 m_great_fox_engine_se;
    snd3DGenerator m_great_fox_sndgen;
    snd3DGenerator m_pleiades_sndgen;
    Vec3f m_slope_rotate;
    u8 m_corneria_phase;

    static const int SCENE_ASTEROID = 0;
    static const int SCENE_BATTLESHIP = 1;
    static const int SCENE_CORNERIA = 2;
    static const int SCENE_SPACE = 3;

    void startScene();
    void updateScene(float deltaFrame);
    void updateSceneEffectAsteroid(float deltaFrame);
    void updateSceneEffectSpace(float deltaFrame);
    void updateSceneEffectCorneria(float deltaFrame);
    void updateSceneEffectBattleship(float deltaFrame);
    void startAppearCore();
public:
    stStarfox();

    virtual ~stStarfox();
    virtual void createObj();
    virtual bool loading();
    virtual bool startAppear();
    virtual void setAppearKind(u8 kind);
    virtual void endAppear();
    virtual void forceStopAppear();
    virtual bool isAppear();
    virtual s32 isStartAppearTimming();
    virtual void update(float deltaFrame);
    virtual bool checkChangeScene();
    virtual void resetChangeScene();
    virtual void setChangeSceneNumber(s32 n);

    virtual int getFinalTechniqColor() { return 0x14000496; }
    virtual IfSmashAppearTask* getAppearTask() { return m_smash_taunt_task; }
    virtual bool isBamperVector() { return true; }
    virtual int getPokeTrainerDrawLayer() { return 1; }

    static stStarfox* create();
    static stClassInfoImpl<Stages::StarFox, stStarfox> bss_loc_14;
};

StSeUtil::UnkStruct data_loc_0[] = {
    {snd_se_stage_Starfox_asteroid_pass_04, -1.000, 80.000, 0.000},
    {snd_se_stage_Starfox_asteroid_pass_01, -0.300, 131.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_03, 0.800, 230.000, 0.000},
    {snd_se_stage_Starfox_asteroid_pass_02, -0.200, 296.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_01, 0.400, 420.000, 0.000},
    {snd_se_stage_Starfox_arwin_roll, 0.500, 502.000, 0.000},
    {snd_se_stage_Starfox_arwin_roll, -0.300, 537.000, 0.000},
    {snd_se_stage_Starfox_arwin_eng, -0.600, 540.000, 750.000},
    {snd_se_stage_Starfox_asteroid_pass_06, 0.500, 600.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, -0.500, 628.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, -0.500, 640.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, -0.500, 655.000, 0.000},
    {snd_se_stage_Starfox_asteroid_crash, -0.500, 675.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, -0.500, 688.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, -0.500, 698.000, 0.000},
    {snd_se_stage_Starfox_asteroid_crash, -0.500, 711.000, 0.000},
    {snd_se_stage_Starfox_asteroid_pass_03, -0.200, 739.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_04, -0.400, 750.000, 0.000},
    {snd_se_stage_Starfox_asteroid_pass_02, -0.600, 870.000, 0.000},
    {snd_se_stage_Starfox_asteroid_pass_05, 0.700, 1020.000, 0.000},
    {snd_se_stage_Starfox_asteroid_pass_01, 0.200, 1077.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_02, -0.400, 1216.000, 0.000},
    {snd_se_stage_Starfox_asteroid_pass_03, -0.500, 1235.000, 0.000},
    {snd_se_stage_Starfox_asteroid_pass_04, 0.300, 1392.000, 0.000},
    {snd_se_stage_Starfox_asteroid_pass_06, -0.200, 1562.000, 0.000}
};

StSeUtil::UnkStruct data_loc_190[] = {
    {snd_se_stage_Starfox_friendship_eng, 0.000, 40.000, 110.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.800, 180.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.700, 240.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire, -0.500, 364.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire, -0.400, 491.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.800, 540.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.400, 546.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.600, 572.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.600, 615.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire, 0.300, 649.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.150, 675.000, 0.000},
    {snd_se_stage_Starfox_friendship_exp, 0.000, 680.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.700, 798.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.300, 820.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.700, 855.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.800, 908.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 1.000, 920.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.000, 960.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.600, 966.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire, 0.200, 979.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.300, 1019.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.800, 1032.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.800, 1060.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.300, 1118.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.300, 1138.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire, 0.000, 1210.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.600, 1232.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, 0.800, 1272.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire, 0.000, 1296.000, 0.000},
    {snd_se_stage_Starfox_friendship_exp, 0.900, 1304.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.100, 1328.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire, -0.800, 1356.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.500, 1362.000, 0.000},
    {snd_se_stage_Starfox_friendship_exp, 0.100, 1365.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.100, 1369.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.400, 1390.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -0.500, 1400.000, 0.000},
    {snd_se_stage_Starfox_friendship_exp, -0.800, 1406.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -0.700, 1430.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -0.400, 1449.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 1.000, 1460.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -0.100, 1478.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.800, 1484.000, 0.000},
    {snd_se_stage_Starfox_friendship_eng, 0.000, 1484.000, 1582.000},
    {snd_se_stage_Starfox_friendship_damage, 0.300, 1495.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.200, 1498.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.100, 1501.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -1.000, 1508.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 1.000, 1512.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, -0.300, 1522.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 1.000, 1526.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.100, 1537.000, 0.000},
    {snd_se_stage_Starfox_through, 0.000, 1540.000, 1810.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.000, 1572.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.500, 1597.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, 1.000, 1602.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.500, 1624.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.700, 1640.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.500, 1718.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -1.000, 1722.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 1.000, 1752.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -1.000, 1774.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, -0.500, 1778.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.800, 1795.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.800, 1806.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.800, 1811.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.800, 1824.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.650, 1851.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, -0.700, 1857.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire, 0.600, 1860.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 1.000, 1883.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire, -0.200, 1887.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.650, 1909.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.500, 1930.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire, 0.800, 1935.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -0.800, 1949.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.500, 1952.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.800, 1975.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, 1.000, 1986.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 1.000, 2005.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 1.000, 2035.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.750, 2050.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.900, 2064.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, 0.750, 2067.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.500, 2070.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -1.000, 2100.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire, -0.400, 2118.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -0.850, 2115.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, 0.500, 2122.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -1.000, 2135.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -1.000, 2156.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -1.000, 2165.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.800, 2173.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.600, 2175.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, 0.300, 2180.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -1.000, 2222.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.600, 2248.000, 0.000},
    {snd_se_stage_Starfox_enemyship_eng, -0.500, 2260.000, 2320.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -1.000, 2266.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.500, 2314.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, 0.500, 2320.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.000, 2330.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.300, 2355.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -0.700, 2368.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.600, 2390.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.400, 2400.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.800, 2428.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.000, 2440.000, 0.000},
    {snd_se_stage_Starfox_through, 0.000, 2448.000, 2800.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.600, 2468.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 1.000, 2470.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, -0.500, 2478.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.800, 2480.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.700, 2500.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.500, 2508.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.400, 2518.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, 0.500, 2520.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.600, 2539.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, -0.700, 2558.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -0.700, 2720.000, 0.000},
    {snd_se_stage_Starfox_friendship_damage, 0.800, 2765.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -0.400, 2778.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.600, 2790.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, 0.400, 2800.000, 0.000},
    {snd_se_stage_Starfox_friendship_fire_2, -0.700, 2813.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, 0.600, 2831.000, 0.000},
    {snd_se_stage_Starfox_enemyship_fire_2, -0.900, 2880.000, 0.000},
};

StSeUtil::UnkStruct data_loc_980[] = {
    {snd_se_stage_Starfox_atomos_in, 0.000, 380.000, 830.000},
    {snd_se_stage_Starfox_sky_amb, 0.000, 816.000, 0.000},
    {snd_se_stage_Starfox_atoms_out, 0.000, 1900.000, 2140.000},
    {snd_se_stage_Starfox_space_amb, 0.000, 2100.000, 0.000},
};

StSeUtil::UnkStruct data_loc_9C0[] = {
    {snd_se_stage_Starfox_wolfen_pass_01, -0.800, 44.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_01, -0.600, 58.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, -0.100, 74.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.700, 96.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.600, 131.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.250, 136.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_04, -0.400, 165.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.100, 194.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot, -0.700, 303.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_02, 0.500, 304.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.800, 316.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, 0.200, 335.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.600, 336.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_04, -0.450, 340.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot, -0.700, 369.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.900, 379.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, -0.300, 406.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.450, 412.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_03, -1.000, 440.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.450, 528.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_04, -0.650, 533.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, 0.350, 542.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.850, 560.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 1.000, 564.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.700, 580.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_04, 0.650, 609.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_01, 0.350, 617.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.650, 796.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, -0.250, 818.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_02, -0.400, 827.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, -0.950, 840.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, -0.300, 852.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.700, 892.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.850, 984.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, 0.350, 1001.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.450, 1071.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_01, 0.250, 1074.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.550, 1083.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, 0.300, 1100.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_01, -0.400, 1103.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_04, -0.750, 1116.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.650, 1146.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.950, 1198.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_04, 0.400, 1200.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_03, -0.700, 1260.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.750, 1296.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, -0.100, 1309.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot, -0.400, 1328.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.900, 1332.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.150, 1336.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.750, 1345.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_02, 0.300, 1353.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, -0.500, 1390.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.300, 1394.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_04, 0.600, 1568.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot, 0.750, 1569.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_01, -0.450, 1582.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_01, -0.350, 1593.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, -0.700, 1605.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, 0.300, 1608.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.450, 1610.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_04, -0.800, 1644.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.700, 1727.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_03, -0.900, 1789.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_04, 0.400, 1807.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, -0.500, 1822.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.700, 1835.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_02, 0.450, 1837.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.850, 1850.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, 0.300, 1865.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_01, -0.400, 1874.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot, 0.650, 1909.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.300, 2013.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.900, 2062.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_01, 0.300, 2075.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.750, 2097.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.800, 2111.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_02, 0.400, 2122.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_04, 0.900, 2167.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, 0.350, 2176.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.500, 2186.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, -0.300, 2332.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_04, -0.450, 2347.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_01, -0.550, 2361.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, -0.850, 2371.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, -0.300, 2384.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.950, 2399.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_04, 0.650, 2437.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.500, 2497.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.450, 2498.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.550, 2534.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.100, 2537.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, -0.500, 2558.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.350, 2595.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.800, 2604.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_01, 0.500, 2606.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.800, 2615.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, -0.400, 2632.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, -0.400, 2637.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.700, 2677.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.850, 2740.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.850, 2771.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.650, 2811.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, 0.250, 2812.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.400, 2817.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_03, -0.700, 2843.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot, 0.600, 2858.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.850, 2864.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.600, 2877.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_02, -0.350, 2892.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.500, 2935.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_04, -0.900, 2592.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_01, 0.600, 3037.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, -0.250, 3113.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, -0.650, 3137.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot, -0.300, 3150.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_01, -0.450, 3128.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, 0.600, 3232.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.750, 3293.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, -0.850, 3370.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_02, 0.350, 3375.000, 0.000},
    {snd_se_stage_Starfox_arwin_pass_02, 0.100, 3376.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.750, 3379.000, 0.000},
    {snd_se_stage_Starfox_arwin_shot_2, 0.500, 3386.000, 0.000},
    {snd_se_stage_Starfox_wolfen_pass_03, -0.300, 3404.000, 0.000},
    {snd_se_stage_Starfox_wolfen_shot_2, 0.650, 3442.000, 0.000},
};

SndID data_loc_11A0[] = {
    snd_se_stage_Starfox_enemyship_fire,
    snd_se_stage_Starfox_friendship_fire,
    snd_se_stage_Starfox_atomos_in,
    snd_se_stage_Starfox_atoms_out,
    snd_se_stage_Starfox_asteroid_pass_01,
    snd_se_stage_Starfox_asteroid_pass_02,
    snd_se_stage_Starfox_asteroid_pass_03,
    snd_se_stage_Starfox_asteroid_pass_04,
    snd_se_stage_Starfox_asteroid_pass_05,
    snd_se_stage_Starfox_asteroid_pass_06,
    snd_se_stage_Starfox_through,
    snd_se_stage_Starfox_friendship_damage,
    snd_se_stage_Starfox_friendship_exp,
    snd_se_stage_Starfox_arwin_eng,
    snd_se_stage_Starfox_arwin_pass_01,
    snd_se_stage_Starfox_arwin_pass_02,
    snd_se_stage_Starfox_arwin_pass_03,
    snd_se_stage_Starfox_arwin_pass_04,
    snd_se_stage_Starfox_arwin_shot,
    snd_se_stage_Starfox_arwin_roll,
    snd_se_stage_Starfox_asteroid_crash,
    snd_se_stage_Starfox_friendship_eng,
    snd_se_stage_Starfox_enemyship_eng,
    snd_se_stage_Starfox_wolfen_shot,
    snd_se_stage_Starfox_wolfen_pass_01,
    snd_se_stage_Starfox_wolfen_pass_02,
    snd_se_stage_Starfox_wolfen_pass_03,
    snd_se_stage_Starfox_wolfen_pass_04,
    snd_se_stage_Starfox_sky_amb,
    snd_se_stage_Starfox_space_amb,
    snd_se_stage_Starfox_friendship_fire_2,
    snd_se_stage_Starfox_enemyship_fire_2,
    snd_se_stage_Starfox_arwin_shot_2,
    snd_se_stage_Starfox_wolfen_shot_2
};

static const size_t SndSeqTable1Size = sizeof(data_loc_0)/sizeof(data_loc_0[0]);
static const size_t SndSeqTable2Size = sizeof(data_loc_190)/sizeof(data_loc_190[0]);
static const size_t SndSeqTable3Size = sizeof(data_loc_980)/sizeof(data_loc_980[0]);
static const size_t SndSeqTable4Size = sizeof(data_loc_9C0)/sizeof(data_loc_9C0[0]);
static const size_t SndIDTableSize = sizeof(data_loc_11A0)/sizeof(data_loc_11A0[0]);
