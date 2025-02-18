#include <StaticAssert.h>
#include <gf/gf_application.h>
#include <gf/gf_callback.h>
#include <so/so_dispose_instance_manager.h>
#include <st/module.h>
#include <types.h>

struct bss_loc_8cc0_t {
    u32 unk0;
    s32 unk4;
public:
    bss_loc_8cc0_t(s32 p2) {
        unk0 = 0xFF;
        unk4 = p2;
    }
    bss_loc_8cc0_t() { }
};

namespace {
    const bss_loc_8cc0_t bss_loc_8cc0(0);
    const bss_loc_8cc0_t bss_loc_8cc8(1);
}

class moMeleeDrawDownCallback : public gfCallBack {
public:
    moMeleeDrawDownCallback() : gfCallBack() { }
    virtual void userProc();
    virtual ~moMeleeDrawDownCallback() { }
};
static_assert(sizeof(moMeleeDrawDownCallback) == 0x8, "Class is wrong size!");

void moMeleeDrawDownCallback::userProc() {
    g_soDisposeInstanceManager->notifyDrawDone();
}

moMeleeDrawDownCallback bss_loc_8ce0;

extern "C" {
typedef void (*PFN_voidfunc)();
__declspec(section ".init") extern PFN_voidfunc _ctors[];
__declspec(section ".init") extern PFN_voidfunc _dtors[];

void _prolog();
void _epilog();
void _unresolved();
}

void _prolog() {
    // Run global constructors
    PFN_voidfunc* ctor;
    for (ctor = _ctors; *ctor; ctor++)
    {
        (*ctor)();
    }
    soDisposeInstanceManager::create();
    g_gfApplication->m_114.add(&bss_loc_8ce0);
}

void _epilog() {
    g_gfApplication->m_114.remove(&bss_loc_8ce0);

    // Run global destructors
    PFN_voidfunc* dtor;
    for (dtor = _dtors; *dtor; dtor++)
    {
        (*dtor)();
    }
}

void _unresolved() {
    moUnResolvedMessage("mo_melee.cpp");
}
