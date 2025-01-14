#pragma once

#include <types.h>
#include <gf/gf_model.h>
#include <gr/gr_yakumono.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <nw4r/g3d/g3d_scnmdl.h>

class grCrayon : public grYakumono {
  public:
    u8* unk150;
    u8 unk154;
    float unk158;
    u8 unk15C;
    float unk160;
    nw4r::g3d::ScnMdl* unk164;
    u32 unk168;
    gfModelAnimation* unk16C;

    grCrayon(const char* taskName);

    virtual ~grCrayon();
    virtual void update(float deltaFrame);
    virtual void changeNodeAnim(u32 chrIndex, u32 modelAnimIndex);

    void getRotate();
    void setRotate();
};

class grCrayonStatic : public grCrayon {
  public:
    static grCrayonStatic* create(int p1, const char *p2);
};

class grCrayonKumo : public grCrayon {
  public:
    static grCrayonKumo* create(int p1, const char *p2);
};

class grCrayonSeason : public grCrayon {
  public:
    static grCrayonSeason* create(int p1, const char *p2);
};

class grCrayonGimic : public grCrayon {
  public:
    static grCrayonGimic* create(int p1, const char *p2);
};


class grCrayonYakumono : public grYakumono {
    u8 unk150[0xC];
  public:

};
static_assert(sizeof(grCrayonYakumono) == 0x15C, "Class is wrong size!");

struct UnkHeihoStruct {
    float x;
    float y;

    UnkHeihoStruct(float p1, float p2) : x(p1), y(p2) { }
    UnkHeihoStruct() { }
};

class grCrayonYakumonoH : public grCrayonYakumono {
    u8 unk15C[0x58];
    float* m_posLimitWork;
    u8 unk1B8[0xC];
  public:
    virtual void setPosLimitWork(float *posLimitWork) { m_posLimitWork = posLimitWork; }
    void setLinearHeiho(s32 p1, UnkHeihoStruct p2);
    void setFallHeiho(UnkHeihoStruct p1, UnkHeihoStruct p2, s32 p3);
    static grCrayonYakumonoH* create(int p1, const char *p2);
};
static_assert(sizeof(grCrayonYakumonoH) == 0x1C4, "Class is wrong size!");
