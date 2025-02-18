#include <gf/gf_application.h>
#include <sc/sc_scene_init.h>
#include <sq/sq_sequence_init.h>
#include <st/module.h>

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
    for (ctor = _ctors; *ctor; ctor++) {
        (*ctor)();
    }
    scSystemRegisterScene();
    sqRegistSequence();
    switch (g_gfApplication->m_e4 >> 24) {
        case 0:
        case 1:
        case 2:
            sqSystemSetStartSequence(true);
            break;
        default:
            break;
    }
}

void _epilog() {
    // Run global destructors
    PFN_voidfunc* dtor;
    for (dtor = _dtors; *dtor; dtor++) {
        (*dtor)();
    }
}

void _unresolved() {
    moUnResolvedMessage("mo_scene.cpp");
}
