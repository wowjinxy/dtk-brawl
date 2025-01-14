#pragma once

#include <gm/gm_lib.h>
#include <gr/gr_tengan_event.h>
#include <memory.h>
#include <st/st_class_info.h>
#include <st/st_melee.h>
#include <types.h>

template<typename T>
class stClassInfoImpl<Stages::Crayon, T> : public stClassInfo {
public:
    stClassInfoImpl() : stClassInfo() {
        setClassInfo(Stages::Crayon, this);
    };

    virtual ~stClassInfoImpl() {
        setClassInfo(Stages::Crayon, 0);
    }

    virtual T* create() {
        return T::create();
    }

    virtual void preload() { }
};

class stCrayon : public stMelee {
    u8 unk1D8;
    float unk1DC;
    float unk1E0;
    u8 unk1E4;
    // TODO: float array?
    float unk1E8;
    float unk1EC;
    float unk1F0;
    float unk1F4;
    float unk1F8;
    float unk1FC;

  public:
    stCrayon(char const* name);
    virtual ~stCrayon();
    virtual void createObj();
    virtual bool loading();
    virtual void update(float deltaFrame);
    virtual void updateLimit(float deltaFrame);
    virtual void updateHeiho(float deltaFrame);
    virtual void updateSeason(float deltaFrame);
    virtual bool isBamperVector() { return true; }

    static stCrayon* create();
    static stClassInfoImpl<Stages::Crayon, stCrayon> bss_loc_14;
};
static_assert(sizeof(stCrayon) == 0x200, "Class is wrong size!");
