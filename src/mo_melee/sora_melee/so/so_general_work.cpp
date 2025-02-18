#include <StaticAssert.h>
#include <so/so_array.h>
#include <so/so_general_flag.h>
#include <so/so_null.h>
#include <so/work/so_general_work_abstract.h>
#include <so/work/so_general_work.h>
#include <types.h>

soGeneralWork g_soGeneralWork(&g_s32ArrayNull, &g_floatArrayNull, &g_s32GeneralFlagArrayNull);

namespace {
    // This class and function are present only to hoist the inline
    // definition of ~soGeneralWorkAbstract to the beginning
    // of this file's .text section
    class UnusedClass : public soGeneralWorkAbstract {

    };

    UnusedClass getTestClass() {
        UnusedClass c;
        return c;
    }
}

void soGeneralWork::clearWorkAll() {
    m_ints->set(0, 0, m_ints->size());
    m_floats->set(0, 0, m_floats->size());
    m_flags->set(0, 0, m_flags->size());
}

u32 soGeneralWork::getFlagWorkSize() {
    return m_flags->size();
}

u32 soGeneralWork::turnOffFlag(u32 flag, u32 index) {
    u32 result;
    if ((result = isFlag(flag, index)) == 1) {
        offFlag(flag, index);
    }
    return result;
}

bool soGeneralWork::isFlag(u32 flag, u32 index) const {
    soGeneralFlag<s32>& gflag = m_flags->at(index);
    return gflag.flag & flag;
}

void soGeneralWork::offFlag(u32 flag, u32 index) {
    m_flags->at(index).flag &= ~flag;
}

void soGeneralWork::clearFlag(u32 index) {
    soGeneralFlag<s32>& gflag = m_flags->at(index);
    gflag.flag = 0;
}

void soGeneralWork::onFlag(u32 flag, u32 index) {
    m_flags->at(index).flag |= flag;
}

u32 soGeneralWork::getFloatWorkSize() {
    return m_floats->size();
}

void soGeneralWork::divFloatWork(float value, u32 index) {
    if (0.0f != value) {
        m_floats->set(index, getFloatWork(index) / value, 1);
    }
}

float soGeneralWork::getFloatWork(u32 index) const {
    return m_floats->at(index);
}

void soGeneralWork::mulFloatWork(float value, u32 index) {
    m_floats->set(index, value * getFloatWork(index), 1);
}

void soGeneralWork::subFloatWork(float value, u32 index) {
    m_floats->set(index, getFloatWork(index) - value, 1);
}

void soGeneralWork::addFloatWork(float value, u32 index) {
    m_floats->set(index, value + getFloatWork(index), 1);
}

void soGeneralWork::setFloatWork(float value, u32 index) {
    m_floats->set(index, value, 1);
}

u32 soGeneralWork::getIntWorkSize() {
    return m_ints->size();
}

void soGeneralWork::decIntWork(u32 index) {
    m_ints->set(index, getIntWork(index) - 1, 1);
}

s32 soGeneralWork::getIntWork(u32 index) const {
    return m_ints->at(index);
}

void soGeneralWork::incIntWork(u32 index) {
    m_ints->set(index, getIntWork(index) + 1, 1);
}

void soGeneralWork::divIntWork(s32 value, u32 index) {
    if (0 != value) {
        m_ints->set(index, getIntWork(index) / value, 1);
    }
}

void soGeneralWork::mulIntWork(s32 value, u32 index) {
    m_ints->set(index, value * getIntWork(index), 1);
}

void soGeneralWork::subIntWork(s32 value, u32 index) {
    m_ints->set(index, getIntWork(index) - value, 1);
}

void soGeneralWork::addIntWork(s32 value, u32 index) {
    m_ints->set(index, value + getIntWork(index), 1);
}

void soGeneralWork::setIntWork(s32 value, u32 index) {
    m_ints->set(index, value, 1);
}
