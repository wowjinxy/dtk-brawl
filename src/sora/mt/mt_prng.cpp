#include <mt/mt_prng.h>
#include <revolution/OS/OSContext.h>
#include <types.h>

void mtRand::init(s32 p1) {
    this->seed = p1;
}

s32 mtRand::generate() {
    return this->seed = (this->seed * MultVal + AddVal) & mtRand::getMax();
}

s32 mtRand::randi() {
    return generate();
}

static mtRand g_mtRand(0);
extern mtPrngLogManager g_mtPrngLogManager;

// [0, 1] random float
float mtRand::randf() {
    return randi() / 2.147483647e9;
}

namespace unused {
    float emitU32ToFloatConstant(u32 x) {
        return x;
    }
}

void srandi(s32 seed) {
    g_mtRand.setSeed(seed);
}

s32 randseed() {
    return g_mtRand.getSeed();
}

// [0, 1) random float
float randf() {
    mtRand* ptr = &g_mtRand;
    u32 val = ptr->generate();
    mtPrngLog log(val, OSGetStackPointer());
    g_mtPrngLogManager.addLog(&log);
    return (val >> 8) / 8388608.0f;
}

// [-1, 1) random float
float randaf() {
    return 2.0f * (randf() - 0.5f);
}

// [0, max) random int
s32 randi(s32 max) {
    return max * randf();
}

s32 mtRand::getMax() {
    return 0x7fffffff;
}
