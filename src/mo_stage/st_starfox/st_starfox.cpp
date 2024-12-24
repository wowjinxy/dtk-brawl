#include <memory.h>
#include <types.h>
#include <ec/ec_mgr.h>
#include <gf/gf_archive.h>
#include <gf/gf_3d_scene.h>
#include <gm/gm_lib.h>
#include <gr/gr_madein.h>
#include <mt/mt_matrix.h>
#include <mt/mt_vector.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <snd/snd_id.h>
#include <st/stage.h>
#include <st/st_melee.h>

#include "st_starfox.h"

stClassInfoImpl<Stages::StarFox, stStarfox> stStarfox::bss_loc_14;

stStarfox* stStarfox::create() {
    return new (Heaps::StageInstance) stStarfox;
}

stStarfox::stStarfox() : stMelee("stStarfox", Stages::StarFox) {
    m_smash_taunt_task = 0;
    m_smash_taunt_kind = 0;
    m_smash_taunt_timing = 0;
    m_smash_taunt_activated = 0;
    m_corneria_phase = 0;
    m_scene_num = SCENE_ASTEROID;
}

stStarfox::~stStarfox() {
    if (m_smash_taunt_task) {
        m_smash_taunt_task->exit();
    }
}

bool stStarfox::loading() {
    return true;
}

void stStarfox::createObj() {
    testStageParamInit(m_fileData, 0xA);
    testStageDataInit(m_fileData, 0x14, 0x38);
    grMadein* stageElement = grMadein::create(0, "", "grStarfoxWarp1", Heaps::StageInstance);
    if (stageElement) {
        addGround(stageElement);
        stageElement->startup(m_secondaryFileData, false, 0);
        stageElement->setStageData(m_stageData);
    }
    stageElement = grMadein::create(1, "", "grStarfoxWarp2", Heaps::StageInstance);
    if (stageElement) {
        addGround(stageElement);
        stageElement->startup(m_secondaryFileData, false, 0);
        stageElement->setStageData(m_stageData);
    }
    stageElement = grMadein::create(2, "", "grStarfoxWarp3", Heaps::StageInstance);
    if (stageElement) {
        addGround(stageElement);
        stageElement->startup(m_secondaryFileData, false, 0);
        stageElement->setStageData(m_stageData);
    }
    stageElement = grMadein::create(0, "", "grStarfoxGdiff", Heaps::StageInstance);
    if (stageElement) {
        addGround(stageElement);
        stageElement->startup(m_fileData, false, 0);
        stageElement->setStageData(m_stageData);
    }
    createCollision(m_fileData, 2, stageElement);
    initCameraParam();
    nw4r::g3d::ResFile posData(m_fileData->getData(Data_Type_Model, 0x64, 0xfffe));
    if (posData.ptr()) {
        nw4r::g3d::ResFile copyPosData = posData;
        createStagePositions(&copyPosData);
    } else {
        // if no stgPos model in pac, use defaults
        createStagePositions();
    }
    createWind2ndOnly();
    m_wind2ndTrigger->setAreaSleep(1);
    for (size_t i = 0; i < getGroundNum(); i++) {
        static_cast<grMadein*>(getGround(i))->initializeEntity();
    }
    static_cast<grMadein*>(getGround(3))->startEntityAutoLoop();
    m_scene_ground_num[0] = 2;
    m_scene_ground_num[1] = 1;
    m_scene_ground_num[2] = 1;
    m_scene_ground_num[3] = 4;
    m_curr_scene_effect.set(0.0f, 0.0f);
    m_scene_lifecycle.set(0.0f, 0.0f);
    m_curr_scene.set(300.0f, 600.0f);
    m_scene_lifecycle.start();
    resetChangeScene();
    m_unused = 0;
    m_not_first_scene = 0;
    loadStageAttrParam(m_fileData, 0x1E);
    initPosPokeTrainer(1, 0);
    createObjPokeTrainer(m_fileData, 0x65, "PokeTrainer00", m_unk, 0);
    m_prev_scene_num = -1;
    m_show_dogfight = 0;
    m_se_player.registId(data_loc_11A0, SndIDTableSize);
    m_se_player.registSeq(0, data_loc_0, SndSeqTable1Size, Heaps::StageInstance);
    m_se_player.registSeq(1, data_loc_190, SndSeqTable2Size, Heaps::StageInstance);
    m_se_player.registSeq(2, data_loc_980, SndSeqTable3Size, Heaps::StageInstance);
    m_se_player.registSeq(3, data_loc_9C0, SndSeqTable4Size, Heaps::StageInstance);
    nw4r::g3d::ResFileData* scn = (nw4r::g3d::ResFileData*) m_secondaryFileData->getData(Data_Type_Scene, 0, 0xFFFE);
    registScnAnim(scn, 0);
    s32 result = m_pleiades_sndgen.prepareSE(snd_se_stage_Starfox_pleiadians_eng, 0);
    if (result != -1) {
        Vec3f pos;
        pos.m_x = 0.0f;
        pos.m_y = 0.0f;
        pos.m_z = 0.0f;
        m_pleiades_sndgen.setPos(&pos);
        m_pleiades_sndgen.startSE(result, 0);
    }
    m_great_fox_engine_se = -1;
    m_slope_rotate.m_x = 0;
    m_slope_rotate.m_y = 0;
    m_slope_rotate.m_z = 0;
    m_smash_taunt_task = IfFoxSmashAppearTask::create(m_secondaryFileData);
}

void stStarfox::setChangeSceneNumber(s32 n) {
    m_scene_num = n;
}

bool stStarfox::checkChangeScene() {
    return m_change_scene;
}

void stStarfox::resetChangeScene() {
    m_change_scene = false;
}

void stStarfox::startScene() {
    switch (m_scene_num) {
        case SCENE_ASTEROID:
            m_curr_scene.set(1000.0f, 1700.0f);
            static_cast<grMadein*>(getGround(3))->endEntity();
            static_cast<grMadein*>(getGround(3))->setMotion(0);
            static_cast<grMadein*>(getGround(3))->startEntityAutoLoop();
            break;
        case SCENE_BATTLESHIP:
            m_curr_scene.set(2900.0f, 2900.0f);
            static_cast<grMadein*>(getGround(3))->endEntity();
            static_cast<grMadein*>(getGround(3))->setMotion(2);
            static_cast<grMadein*>(getGround(3))->startEntityAutoLoop();
            break;
        case SCENE_CORNERIA:
            m_curr_scene.set(2300.0f, 2300.0f);
            static_cast<grMadein*>(getGround(3))->endEntity();
            static_cast<grMadein*>(getGround(3))->setMotion(1);
            static_cast<grMadein*>(getGround(3))->startEntityAutoLoop();
            break;
        case SCENE_SPACE:
            m_curr_scene.set(3000.0f, 3500.0f);
            static_cast<grMadein*>(getGround(3))->endEntity();
            static_cast<grMadein*>(getGround(3))->setMotion(0);
            static_cast<grMadein*>(getGround(3))->startEntityAutoLoop();
            break;
        default:
            break;
    }
    m_curr_scene.start();
    m_curr_scene_effect.start();
    if (!m_show_dogfight && m_scene_num == SCENE_SPACE) {
        m_scene_grounds[1]->endEntity();
        m_scene_grounds[2]->endEntity();
        m_scene_grounds[3]->endEntity();
    }
    m_unused = 0;
}

void stStarfox::updateScene(float deltaFrame) {
    nw4r::g3d::ResFileData* scn;
    switch(m_scene_lifecycle.getPhase()) {
        case 0: // Ready to start next scene
            if (unk1A8) {
                grMadein* scene_ground;
                m_scene_grounds[3] = 0;
                m_scene_grounds[2] = 0;
                m_scene_grounds[1] = 0;
                m_scene_grounds[0] = 0;
                g_gfSceneRoot->removeResAnmScn();
                for (s32 i = 0; i < m_scene_ground_num[m_scene_num]; i++) {
                    scene_ground = grMadein::create((s16)i, "", "SceneGround", Heaps::StageInstance);
                    if (scene_ground) {
                        addGround(scene_ground);
                        scene_ground->startup(unk1A8, 0, 0);
                        scene_ground->setStageData(m_stageData);
                        scene_ground->initializeEntity();
                        scene_ground->startEntityAutoLoop();
                        m_scene_grounds[(s16)i] = scene_ground;
                    }
                }
                startScene();
                if (m_not_first_scene == 1) {
                    scn = (nw4r::g3d::ResFileData*)
                        m_secondaryFileData->getData(Data_Type_Scene, m_scene_num*3 + 2, 0xFFFE);
                    registScnAnim(scn, 0);
                    static_cast<grMadein*>(getGround(1))->endEntity();
                    static_cast<grMadein*>(getGround(2))->startEntity();
                    m_scene_lifecycle.setPhase(m_scene_lifecycle.getPhase() + 1);
                    playSeBasic(snd_se_stage_Starfox_warp_out, 0.0f);
                } else {
                    m_scene_lifecycle.setPhase(m_scene_lifecycle.getPhase() + 2);
                    scn = (nw4r::g3d::ResFileData*)
                        unk1A8->getData(Data_Type_Scene, 0, 0xFFFE);
                    registScnAnim(scn, 0);
                }
            }
            break;
        case 1: // Check for smash taunt activation
            if (static_cast<grMadein*>(getGround(2))->isEndEntity() == 1u) {
                static_cast<grMadein*>(getGround(2))->endEntity();
                m_scene_lifecycle.setPhase(m_scene_lifecycle.getPhase() + 1);
                g_gfSceneRoot->removeResAnmScn();
                scn = (nw4r::g3d::ResFileData*)
                    unk1A8->getData(Data_Type_Scene, 0, 0xFFFE);
                registScnAnim(scn, 0);
                m_wind2ndTrigger->setAreaSleep(1);
                startAppearCore();
            }
            break;
        case 2: // Scene is in progress, waiting for timer to run out
            if (m_curr_scene.isReadyEnd() == 1u) {
                m_curr_scene.end();
                static_cast<grMadein*>(getGround(0))->startEntity();
                m_scene_lifecycle.setPhase(m_scene_lifecycle.getPhase() + 1);
                m_not_first_scene = 1;
                m_prev_scene_num = m_scene_num;
                g_gfSceneRoot->removeResAnmScn();
                scn = (nw4r::g3d::ResFileData*)
                    m_secondaryFileData->getData(Data_Type_Scene, m_prev_scene_num*3, 0xFFFE);
                registScnAnim(scn, 0);
                playSeBasic(snd_se_stage_Starfox_warp_in, 0.0f);
                m_windAreaData2nd->m_speed = 0.5f;
                m_windAreaData2nd->m_vector = 361.0f;
                m_windAreaData2nd->m_60 = 24.0f;
                m_windAreaData2nd->m_64 = 1.0f;
                m_windAreaData2nd->m_72 = 0x46;
                m_wind2ndTrigger->setWindParam(m_windAreaData2nd, 0);
                m_wind2ndTrigger->setAreaSleep(0);
                static_cast<grMadein*>(getGround(3))->endEntity();
                static_cast<grMadein*>(getGround(3))->setMotion(3);
                static_cast<grMadein*>(getGround(3))->startEntityAutoLoop();
                if (m_great_fox_engine_se != -1) {
                    m_great_fox_sndgen.stopSE(m_great_fox_engine_se, 0);
                    m_great_fox_engine_se = -1;
                }
            }
            break;
        case 3: // Scene transition (warp) in progress?
            if (static_cast<grMadein*>(getGround(0))->isEndEntity() == 1u) {
                static_cast<grMadein*>(getGround(0))->endEntity();
                static_cast<grMadein*>(getGround(1))->startEntityAutoLoop();
                m_scene_lifecycle.setPhase(m_scene_lifecycle.getPhase() + 1);
                g_gfSceneRoot->removeResAnmScn();
                scn = (nw4r::g3d::ResFileData*)
                    m_secondaryFileData->getData(Data_Type_Scene, m_prev_scene_num*3 + 1, 0xFFFE);
                registScnAnim(scn, 0);
            }
            break;
        case 4: // Teardown phase for scene effects and grounds
            m_curr_scene_effect.end();
            // TODO UBFIX: Match this code without reinterpret_cast
            s32 i = 0;
            s32 arrSz = sizeof(m_effects)/sizeof(m_effects[0]);
            u32* m_effects_ptr = reinterpret_cast<u32*>(this);
            for (i = 0; i < arrSz; m_effects_ptr++, i++) {
                if (m_effects_ptr[0xF7] != 0xFFFFFFFF) {
                    g_ecMgr->killEffect(m_effects_ptr[0xF7], 1, 1);
                    m_effects_ptr[0xF7] = 0xFFFFFFFF;
                }
            }
            s32 j = 0;
            grMadein** m_scene_grounds_ptr = reinterpret_cast<grMadein**>(this);
            for (j = 0; j < m_scene_ground_num[m_scene_num]; m_scene_grounds_ptr++, j++) {
                if (m_scene_grounds_ptr[0x100]) {
                    removeGround(m_scene_grounds_ptr[0x100]);
                    (m_scene_grounds_ptr[0x100])->preExit();
                    (m_scene_grounds_ptr[0x100])->exit();
                }
            }
            m_scene_lifecycle.setPhase(m_scene_lifecycle.getPhase() + 1);
            break;
        case 5: // Indicate to user that the scene is ready to change again
            m_change_scene = true;
            m_scene_lifecycle.end();
            m_scene_lifecycle.start();
            if (m_show_dogfight == 1) {
                m_show_dogfight = 0;
            } else {
                m_show_dogfight = 1;
            }
            break;
        default:
            break;
    }
}

void stStarfox::updateSceneEffectAsteroid(float deltaFrame) {
    switch (m_curr_scene_effect.getPhase()) {
        case 0:
            m_effects[4] = -1;
            m_effects[3] = -1;
            m_effects[2] = -1;
            m_effects[1] = -1;
            m_effects[0] = -1;
            m_curr_scene_effect.setPhase(m_curr_scene_effect.getPhase() + 1);
            break;
        case 1:
            if (m_scene_grounds[0]->isFrameOffset(674.0f) == 1u) {
                u32 effect = g_ecMgr->setEffect(ef_ptc_stg_starfox_inseki);
                m_effects[0] = effect;
                g_ecMgr->setParent(effect, m_scene_grounds[0]->m_sceneModels[0], "clash_meteo2", 0);
                m_curr_scene_effect.setPhase(m_curr_scene_effect.getPhase() + 1);
            }
            break;
        case 2:
            if (m_scene_grounds[0]->isFrameOffset(710.0f) == 1u) {
                u32 effect = g_ecMgr->setEffect(ef_ptc_stg_starfox_inseki);
                m_effects[1] = effect;
                g_ecMgr->setParent(effect, m_scene_grounds[0]->m_sceneModels[0], "clash_meteo3", 0);
                m_curr_scene_effect.setPhase(m_curr_scene_effect.getPhase() + 1);
            }
            break;
        default:
            break;
    }
}

void stStarfox::updateSceneEffectSpace(float deltaFrame) {
    switch (m_curr_scene_effect.getPhase()) {
        case 0:
            m_effects[4] = -1;
            m_effects[3] = -1;
            m_effects[2] = -1;
            m_effects[1] = -1;
            m_effects[0] = -1;
            u32 effect = g_ecMgr->setEffect(ef_ptc_stg_starfox_space_enkei);
            m_effects[0] = effect;
            g_ecMgr->setParent(effect, m_scene_grounds[0]->m_sceneModels[0], "PPP", 0);
            m_curr_scene_effect.setPhase(m_curr_scene_effect.getPhase() + 1);
            s32 res = m_great_fox_sndgen.prepareSE(snd_se_stage_Starfox_greatfox_eng, 0);
            m_great_fox_engine_se = res;
            if (res != -1) {
                m_great_fox_sndgen.startSE(res, 0);
            }
            break;
        default:
            break;
    }
    if (m_great_fox_engine_se != -1) {
        Vec3f nodePos;
        m_scene_grounds[3]->getNodePosition(&nodePos, 0, "GfoxMove");
        Vec3f sndPos;
        float f0, f1, f2;
        f2 = nodePos.m_z;
        f1 = nodePos.m_y;
        f0 = nodePos.m_x;
        sndPos.m_y = f1;
        sndPos.m_x = f0;
        sndPos.m_z = f2;
        m_great_fox_sndgen.setPos(&sndPos);
    }
}

void stStarfox::updateSceneEffectCorneria(float deltaFrame) {
    switch (m_curr_scene_effect.getPhase()) {
        case 0:
            m_effects[4] = -1;
            m_effects[3] = -1;
            m_effects[2] = -1;
            m_effects[1] = -1;
            m_effects[0] = -1;
            m_curr_scene_effect.setPhase(m_curr_scene_effect.getPhase() + 1);
            m_corneria_phase = 1;
            break;
        default:
            break;
    }
    switch (m_corneria_phase) {
        case 1:
            if (m_curr_scene.getTimeLeft() < 1900.0f) {
                m_windAreaData2nd->m_speed = 0.5f;
                m_windAreaData2nd->m_vector = 90.0f;
                m_windAreaData2nd->m_60 = 16.0f;
                m_windAreaData2nd->m_64 = 0.9f;
                m_windAreaData2nd->m_72 = 0x46;
                m_wind2ndTrigger->setWindParam(m_windAreaData2nd, 0);
                m_wind2ndTrigger->setAreaSleep(0);
                m_corneria_phase++;
            }
            break;
        case 2:
            if (m_curr_scene.getTimeLeft() < 1500.0f) {
                m_wind2ndTrigger->setAreaSleep(1);
                m_corneria_phase++;
            }
            break;
        case 3:
            if (m_curr_scene.getTimeLeft() < 370.0f) {
                m_windAreaData2nd->m_speed = 0.6f;
                m_windAreaData2nd->m_vector = 270.0f;
                m_windAreaData2nd->m_60 = 24.0f;
                m_windAreaData2nd->m_64 = 0.8f;
                m_windAreaData2nd->m_72 = 0x3C;
                m_wind2ndTrigger->setWindParam(m_windAreaData2nd, 0);
                m_wind2ndTrigger->setAreaSleep(0);
                m_corneria_phase++;
            }
            break;
        case 4:
            if (m_curr_scene.getTimeLeft() < 100.0f) {
                m_wind2ndTrigger->setAreaSleep(1);
                m_corneria_phase++;
            }
            break;
        default:
            break;
    }
}

void stStarfox::updateSceneEffectBattleship(float deltaFrame) {
    switch (m_curr_scene_effect.getPhase()) {
        case 0:
            m_effects[4] = -1;
            m_effects[3] = -1;
            m_effects[2] = -1;
            m_effects[1] = -1;
            m_effects[0] = -1;
            u32 effect = g_ecMgr->setEffect(ef_ptc_stg_starfox_battle_ship_enkei);
            m_effects[4] = effect;
            g_ecMgr->setParent(effect, m_scene_grounds[0]->m_sceneModels[0], "main_rotate", 0);
            m_curr_scene_effect.setPhase(m_curr_scene_effect.getPhase() + 1);
            break;
        case 1:
            if (m_scene_grounds[0]->isFrameOffset(680.0f) == 1u) {
                u32 effect = g_ecMgr->setEffect(ef_ptc_stg_starfox_bakuhatu);
                m_effects[0] = effect;
                g_ecMgr->setParent(effect, m_scene_grounds[0]->m_sceneModels[0], "explosion_01", 0);
                m_curr_scene_effect.setPhase(m_curr_scene_effect.getPhase() + 1);
            }
            break;
        case 2:
            if (m_scene_grounds[0]->isFrameOffset(692.0f) == 1u) {
                u32 effect = g_ecMgr->setEffect(ef_ptc_stg_starfox_bakuhatu);
                m_effects[1] = effect;
                g_ecMgr->setParent(effect, m_scene_grounds[0]->m_sceneModels[0], "explosion_02", 0);
                effect = g_ecMgr->setEffect(ef_ptc_stg_starfox_bakuhatu_b);
                m_effects[3] = effect;
                g_ecMgr->setParent(effect, m_scene_grounds[0]->m_sceneModels[0], "explosion_02", 0);
                m_curr_scene_effect.setPhase(m_curr_scene_effect.getPhase() + 1);
            }
            break;
        case 3:
            if (m_scene_grounds[0]->isFrameOffset(706.0f) == 1u) {
                u32 effect = g_ecMgr->setEffect(ef_ptc_stg_starfox_bakuhatu);
                m_effects[2] = effect;
                g_ecMgr->setParent(effect, m_scene_grounds[0]->m_sceneModels[0], "explosion_03", 0);
                m_curr_scene_effect.setPhase(m_curr_scene_effect.getPhase() + 1);
            }
            break;
        default:
            break;
    }
}

void stStarfox::update(float deltaFrame) {
    g_ecMgr->setDrawPrio(1);
    updateScene(deltaFrame);
    if (m_curr_scene_effect.isEvent() == 1u) {
        if (m_show_dogfight || m_scene_num != SCENE_SPACE) {
            m_se_player.playFrame(m_scene_num, m_scene_grounds[0]->getEntityFrame());
        }
        switch (m_scene_num) {
            case SCENE_ASTEROID:
                updateSceneEffectAsteroid(deltaFrame);
                break;
            case SCENE_BATTLESHIP:
                updateSceneEffectBattleship(deltaFrame);
                break;
            case SCENE_CORNERIA:
                updateSceneEffectCorneria(deltaFrame);
                break;
            case SCENE_SPACE:
                updateSceneEffectSpace(deltaFrame);
                break;
            default:
                break;
        }
        Matrix nodeMtx(true);
        m_scene_grounds[0]->getNodeMatrix(&nodeMtx, 0, "Slope");
        nodeMtx.getRotate(&m_slope_rotate);
        float f1, f2, f3;
        f1 = m_slope_rotate.m_x;
        f2 = m_slope_rotate.m_y;
        f1 = 57.29578f * f1;
        f3 = m_slope_rotate.m_z;
        f2 = 57.29578f * f2;
        f3 = 57.29578f * f3;
        m_slope_rotate.m_x = f1;
        m_slope_rotate.m_y = f2;
        m_slope_rotate.m_z = f3;
    } else {
        m_slope_rotate.m_x *= 0.9f;
        m_slope_rotate.m_y *= 0.9f;
        m_slope_rotate.m_z *= 0.9f;
    }
    static_cast<grGimmick*>(getGround(3))->setRot(&m_slope_rotate);
    m_curr_scene.update(deltaFrame);
    m_curr_scene_effect.update(deltaFrame);
    m_scene_lifecycle.update(deltaFrame);
    g_ecMgr->setDrawPrio(-1);
}

void stStarfox::setAppearKind(u8 kind) {
    m_smash_taunt_kind = kind;
}

void stStarfox::startAppearCore() {
    if (m_smash_taunt_activated) {
        u32 id = 0;
        switch (m_scene_num) {
            case SCENE_ASTEROID:
                id = 3;
                break;
            case SCENE_BATTLESHIP:
                id = 4;
                break;
            case SCENE_CORNERIA:
                id = 2;
                break;
            case SCENE_SPACE:
                id = m_show_dogfight;
                break;
            default:
                break;
        }
        id += m_smash_taunt_kind*5;
        if (m_smash_taunt_task) {
            m_smash_taunt_task->start(id);
        }
        m_smash_taunt_activated = 0;
    }
}

bool stStarfox::startAppear() {
    m_smash_taunt_activated = 1;
    m_smash_taunt_timing = 1;
    return true;
}

void stStarfox::endAppear() {
    if (m_smash_taunt_task && isAppear()) {
        m_smash_taunt_task->stop(0);
    }
}

s32 stStarfox::isStartAppearTimming() {
    return m_smash_taunt_timing ^ 1;
}

bool stStarfox::isAppear() {
    return m_smash_taunt_task->isPlaying();
}

void stStarfox::forceStopAppear() {
    if (m_smash_taunt_task) {
        m_smash_taunt_task->stop(1);
    }
}
