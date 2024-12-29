#include <StaticAssert.h>
#include <gf/gf_archive.h>
#include <gm/gm_lib.h>
#include <gr/gr_madein.h>
#include <it/it_manager.h>
#include <memory.h>
#include <mt/mt_vector.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <st/st_class_info.h>
#include <st/st_melee.h>
#include <types.h>

template<typename T>
class stClassInfoImpl<Stages::OTrain, T> : public stClassInfo {
public:
    stClassInfoImpl() : stClassInfo() {
        setClassInfo(Stages::OTrain, this);
    };

    virtual ~stClassInfoImpl() {
        setClassInfo(Stages::OTrain, 0);
    }

    virtual T* create() {
        return T::create();
    }

    virtual void preload() { }
};

struct bss_loc_8_t {
    u32 unk0;
    s32 unk4;
public:
    bss_loc_8_t(s32 p2) {
        unk0 = 0xFF;
        unk4 = p2;
    }
    bss_loc_8_t() { }
};

namespace {
    const bss_loc_8_t bss_loc_8(0);
    const bss_loc_8_t bss_loc_10(1);
}

class stOnlineTrainning : public stMelee {
    s32 unk1D8;
    u32 unk1DC;
    BaseItem* m_sandbag;
    bool unk1E4;

    // NOTE: inferred inline function
    void createSandbag() {
        Vec3f sandbagStartPos;
        m_sandbag = itManager::getInstance()->createItem(Item_Sandbag, 0, -1, 0, 0, 0xFFFF, 0, 0xFFFF);
        if (m_sandbag) {
            getFighterReStartPos(&sandbagStartPos, 3);
            m_sandbag->warp(&sandbagStartPos);
            m_sandbag->setVanishMode(0);
        }
    }
public:
    stOnlineTrainning(char const* name) : stMelee(name, Stages::OTrain),
      unk1D8(data_loc_0), unk1DC(0), m_sandbag(0), unk1E4(false) {
        setPlayerPositionIndexSerial();
    }
    static stOnlineTrainning* create();

    virtual ~stOnlineTrainning();
    virtual bool loading();
    virtual void createObj();
    virtual void update(float unused);

    virtual int getBgmID() { return 0x26FF; }

    static s32 data_loc_0;
    static stClassInfoImpl<Stages::OTrain, stOnlineTrainning> bss_loc_24;
};

// TODO: If possible, use a pragma-free alternative that will allocate a
// non-const, 0-valued global variable to .data rather than to .bss
#pragma push
#pragma section ".data" ".data"
__declspec(section ".data") s32 stOnlineTrainning::data_loc_0 = 0;
#pragma pop

stClassInfoImpl<Stages::OTrain, stOnlineTrainning> stOnlineTrainning::bss_loc_24;

stOnlineTrainning* stOnlineTrainning::create() {
    return new (Heaps::StageInstance) stOnlineTrainning("stOnlineTrainning");
}

stOnlineTrainning::~stOnlineTrainning() {
    releaseArchive();
}

bool stOnlineTrainning::loading() {
    return true;
}

void stOnlineTrainning::createObj() {
    testStageParamInit(m_fileData, 0xA);
    addGround(grMadein::create(1, "", "", Heaps::StageInstance));
    u32 i = 0;
    u32 groundNum = getGroundNum();
    while (i != groundNum) {
        Ground* gr = getGround(i);
        if (gr) {
            gr->startup(m_fileData, false, 0);
        }
        i++;
    }
    createCollision(m_fileData, 2, 0);
    initCameraParam();
    void* ptr = m_fileData->getData(Data_Type_Model, 0x64, 0xfffe);
    if (ptr) {
        nw4r::g3d::ResFile posData(ptr);
        createStagePositions(&posData);
    } else {
        createStagePositions();
    }
    createWind2ndOnly();
    loadStageAttrParam(m_fileData, 0x1E);
    initPosPokeTrainer(1, 0);
    createObjPokeTrainer(m_fileData, 101, "PokeTrainer00", m_unk, 0);
    nw4r::g3d::ResFileData* scn = (nw4r::g3d::ResFileData*) m_fileData->getData(Data_Type_Scene, 0, 0xFFFE);
    registScnAnim(scn, 0);

    static_cast<grMadein*>(getGround(0))->initializeEntity();
    static_cast<grMadein*>(getGround(0))->startEntityAutoLoop();
    unk1D8 = data_loc_0;
    unk1DC = 0;
    unk1E4 = true;
}

namespace {
    s32 data_loc_2C = 1;
    s32 data_loc_30 = 1;
    s32 data_loc_34 = 1;
    s32 data_loc_38 = 1;
    s32 data_loc_3C = 1;
    s32 data_loc_40 = 1;
    s32 data_loc_44 = 1;
}

void stOnlineTrainning::update(float unused) {
    if (unk1E4) {
        switch (unk1DC) {
            case 0:
                unk1D8 = data_loc_2C;
                unk1DC = 0xC1;
                // FALL-THROUGH
            case 0xC1:
                createSandbag();
                unk1D8 = data_loc_30;
                unk1DC = 0xCD;
                break;
            case 0xCD:
            case 0xE9:
                if (itManager::getInstance()->getItemNum(Item_Sandbag) == 0) {
                    createSandbag();
                    unk1D8 = data_loc_34;
                    unk1DC = 0xD4;
                    break;
                }
                if (m_sandbag) {
                    Vec3f stack34;
                    stack34.m_x = m_sandbag->getParamFloat(3);
                    stack34.m_y = m_sandbag->getParamFloat(4);
                    stack34.m_z = 0.0f;
                    if (stack34.m_y <= -100.f) {
                        itManager::getInstance()->removeItem(m_sandbag);
                        unk1D8 = data_loc_38;
                        unk1DC = 0xE2;
                        break;
                    }
                }
                goto label;
            case 0xE2:
                unk1D8 = data_loc_3C;
                unk1DC = 0xE3;
                break;
            case 0xE3:
                createSandbag();
                unk1D8 = data_loc_40;
                unk1DC = 0xE6;
                break;
            case 0xD4:
            case 0xE6:
            label:
                unk1D8 = data_loc_44;
                unk1DC = 0xE9;
                break;
            default:
                break;
        }
    }
}
