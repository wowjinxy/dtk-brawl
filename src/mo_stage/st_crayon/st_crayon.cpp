#include <cm/cm_camera_controller.h>
#include <gf/gf_archive.h>
#include <gm/gm_lib.h>
#include <memory.h>
#include <mt/mt_prng.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <snd/snd_id.h>
#include <snd/snd_system.h>
#include <st_crayon/gr_crayon.h>
#include <st/stage.h>
#include <st/st_class_info.h>
#include <st/st_melee.h>
#include <types.h>

#include <st_crayon/st_crayon.h>

stClassInfoImpl<Stages::Crayon, stCrayon> stCrayon::bss_loc_14;

inline stCrayon::stCrayon(char const* name) : stMelee(name, Stages::Crayon) {
    unk1DC = 0.0f;
    unk1D8 = 0;
    unk1E0 = 0.0f;
    unk1E4 = 4;
    unk1E8 = 0.0f;
    unk1EC = 0.0f;
    unk1F0 = 0.0f;
    unk1F4 = 0.0f;
    unk1F8 = 0.0f;
    unk1FC = 0.0f;
}

stCrayon* stCrayon::create() {
    return new (Heaps::StageInstance) stCrayon("stCrayon");
}

stCrayon::~stCrayon() {
    releaseArchive();
}

bool stCrayon::loading() {
    return true;
}

void stCrayon::createObj() {
    testStageParamInit(m_fileData, 0xA);
    testStageDataInit(m_fileData, 0x14, 0x28);
    addGround(grCrayonStatic::create(1, "grCrayonStatic"));
    addGround(grCrayonKumo::create(3, "grCrayonKumom"));
    addGround(grCrayonSeason::create(2, "grCrayonSeason"));
    addGround(grCrayonGimic::create(4, "grCrayonGimmick"));

    u32 i = 0;
    u32 groundNum = getGroundNum();
    for (i; i != groundNum; i++) {
        Ground* gr = getGround(i);
        if (gr) {
            gr->setStageData(m_stageData);
            gr->startup(m_fileData, false, 0);
            static_cast<grCrayon*>(gr)->unk150 = &unk1E4;
        }
    }

    createCollision(m_fileData, 2, 0);
    addGround(grCrayonYakumonoH::create(5, "grCrayonYakumonoH"));
    addGround(grCrayonYakumonoH::create(5, "grCrayonYakumonoH"));
    addGround(grCrayonYakumonoH::create(5, "grCrayonYakumonoH"));
    addGround(grCrayonYakumonoH::create(5, "grCrayonYakumonoH"));
    addGround(grCrayonYakumonoH::create(5, "grCrayonYakumonoH"));

    groundNum = getGroundNum();
    for (int j = i; j != groundNum; j++) {
        Ground* gr = getGround(j);
        if (gr) {
            gr->setStageData(m_stageData);
            gr->startup(m_fileData, false, 0);
            static_cast<grCrayonYakumonoH*>(gr)->setPosLimitWork(&unk1E8);
        }
    }

    initCameraParam();
    nw4r::g3d::ResFile posData(m_fileData->getData(Data_Type_Model, 0x64, 0xFFFE));
    if (posData.ptr()) {
        nw4r::g3d::ResFile copyPosData = posData;
        createStagePositions(&copyPosData);
    } else {
        createStagePositions();
    }
    createWind2ndOnly();
    loadStageAttrParam(m_fileData, 0x1E);
    registScnAnim(static_cast<nw4r::g3d::ResFileData*>(m_fileData->getData(Data_Type_Scene, 0, 0xFFFE)), 0);
    initPosPokeTrainer(1, 0);
    createObjPokeTrainer(m_fileData, 0x65, "PokeTrainer00", m_unk, 0);
}

void stCrayon::update(float deltaFrame) {
    updateLimit(deltaFrame);
    updateHeiho(deltaFrame);
    updateSeason(deltaFrame);
}

void stCrayon::updateLimit(float deltaFrame) {
    CameraController* cm = CameraController::getInstance();
    float f1, f2;
    f2 = cm->unk160;
    f1 = cm->unk158;
    unk1E8 = f1;
    unk1EC = f2;
    unk1F0 = 0.0f;

    float _f1, _f2;
    _f2 = cm->unk164;
    _f1 = cm->unk15C;
    unk1F4 = _f1;
    unk1F8 = _f2;
    unk1FC = 0.0f;
}

void stCrayon::updateHeiho(float deltaFrame) {
    UnkHeihoStruct r1_30;
    UnkHeihoStruct r1_28;
    unk1DC += deltaFrame;
    if (s32(unk1DC) % 2000 == 0) {
        grCrayonYakumonoH* gr = static_cast<grCrayonYakumonoH*>(getGround(4));
        if (gr) {
            gr->setLinearHeiho(s32(8.0f * randf()), UnkHeihoStruct(1.0f, 1.0f));
        }
    }

    if (s32(unk1DC) % 2000 == 1000) {
        r1_30.x = (randf() < 0.5f) ? 0.1f : -0.1f;
        r1_28.x = 70.0f * (randf() - 0.5f);
        r1_28.y = 140.0f;
        s32 i;
        s32 flyGuyIdx = 0;
        for (i = 0; i < s32(5.9f * randf()); i++) {
            grCrayonYakumonoH* gr = static_cast<grCrayonYakumonoH*>(getGround(i + 4));
            if (gr) {
                float f0 = 0.5f * randf();
                r1_30.y = 1.3f + f0;
                f0 = 10.0f * randf();
                r1_28.x -= (10.0f + f0);
                gr->setFallHeiho(r1_30, r1_28, flyGuyIdx);
                flyGuyIdx++;
            }
        }
    }
    if (unk1DC >= 3000.0f) {
        unk1DC -= 2000.0f;
    }
}

void stCrayon::updateSeason(float deltaFrame) {
    float* stageDataFlts = static_cast<float*>(m_stageData);
    if (stageDataFlts) {
        unk1E0 -= deltaFrame;
        if (unk1E0 < 0.0f) {
            unk1E0 = 0.0f;
        }

        switch (unk1D8) {
          case 0:
            unk1E0 = 0.0f;
            const float rnd = randf();
            if (rnd < 0.25f) {
                unk1E4 = 0;
            } else if (rnd < 0.5f) {
                unk1E4 = 1;
            } else if (rnd < 0.75f) {
                unk1E4 = 2;
            } else {
                unk1E4 = 3;
            }

            switch (unk1E4) {
              case 0:
                unk1D8 = 2;
                break;
              case 1:
                unk1D8 = 4;
                break;
              case 2:
                unk1D8 = 6;
                break;
              case 3:
                unk1D8 = 8;
                break;
              default:
                break;
            }

            switch (unk1E4) {
              case 2:
                setBgmChange(0.0f, true, 2);
                break;
              case 3:
                setBgmChange(0.0f, true, 3);
                break;
              default:
                break;
            }

            break;
          case 2:
            if (0.0f == unk1E0) {
                registScnAnim(static_cast<nw4r::g3d::ResFileData*>(
                    m_fileData->getData(Data_Type_Scene, 0, 0xFFFE)), 0);
                unk1E4 = 0;
                unk1E0 = stageDataFlts[9];
                unk1D8 = 3;
            }
            break;
          case 3:
            if (0.0f == unk1E0) {
                setBgmChange(180.0f, true, 1);
                g_sndSystem->playSE(snd_se_stage_Crayon_season01, 0, 0, 0, -1);
                registScnAnim(static_cast<nw4r::g3d::ResFileData*>(
                    m_fileData->getData(Data_Type_Scene, 0, 0xFFFE)), 1);
                unk1D8 = 4;
                unk1E0 = 180.0f;
            }
            break;
          case 4:
            if (0.0f == unk1E0) {
                registScnAnim(static_cast<nw4r::g3d::ResFileData*>(
                    m_fileData->getData(Data_Type_Scene, 0, 0xFFFE)), 2);
                unk1E4 = 1;
                unk1E0 = stageDataFlts[9];
                unk1D8 = 5;
            }
            break;
          case 5:
            if (0.0f == unk1E0) {
                setBgmChange(180.0f, true, 2);
                g_sndSystem->playSE(snd_se_stage_Crayon_season02, 0, 0, 0, -1);
                registScnAnim(static_cast<nw4r::g3d::ResFileData*>(
                    m_fileData->getData(Data_Type_Scene, 0, 0xFFFE)), 3);
                unk1D8 = 6;
                unk1E0 = 180.0f;
            }
            break;
          case 6:
            if (0.0f == unk1E0) {
                registScnAnim(static_cast<nw4r::g3d::ResFileData*>(
                    m_fileData->getData(Data_Type_Scene, 0, 0xFFFE)), 4);
                unk1E4 = 2;
                unk1E0 = stageDataFlts[9];
                unk1D8 = 7;
            }
            break;
          case 7:
            if (0.0f == unk1E0) {
                setBgmChange(180.0f, true, 3);
                g_sndSystem->playSE(snd_se_stage_Crayon_season03, 0, 0, 0, -1);
                registScnAnim(static_cast<nw4r::g3d::ResFileData*>(
                    m_fileData->getData(Data_Type_Scene, 0, 0xFFFE)), 5);
                unk1D8 = 8;
                unk1E0 = 180.0f;
            }
            break;
          case 8:
            if (0.0f == unk1E0) {
                registScnAnim(static_cast<nw4r::g3d::ResFileData*>(
                    m_fileData->getData(Data_Type_Scene, 0, 0xFFFE)), 6);
                unk1E4 = 3;
                unk1E0 = stageDataFlts[9];
                unk1D8 = 9;
            }
            break;
          case 9:
            if (0.0f == unk1E0) {
                setBgmChange(180.0f, true, 0);
                g_sndSystem->playSE(snd_se_stage_Crayon_season04, 0, 0, 0, -1);
                registScnAnim(static_cast<nw4r::g3d::ResFileData*>(
                    m_fileData->getData(Data_Type_Scene, 0, 0xFFFE)), 7);
                unk1D8 = 2;
                unk1E0 = 180.0f;
            }
            break;
          default:
            break;
        }
    }
}
