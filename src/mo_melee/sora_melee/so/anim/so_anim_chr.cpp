#include <StaticAssert.h>
#include <nw4r/g3d/g3d_anmchr.h>
#include <so/anim/so_anim_chr.h>
#include <types.h>

soAnimChr::soAnimChr() {
    m_animFrame = 0.0f;
    m_anmChrRes = 0;
    m_startFrame = 0.0f;
    m_saveFrame = 0.0f;
    m_rate = 1.0f;
    m_frameAhead = 0.0f;
    m_loopFlag = 0;
    m_reverseFlag = 0;
}

soAnimChr::~soAnimChr() {
    if (m_anmChrRes) {
        m_anmChrRes->Destroy();
        m_anmChrRes = 0;
    }
}

inline void soAnimChr::reinit(bool destroy) {
    m_rate = 1.0f;
    m_loopFlag = 0;
    m_reverseFlag = 0;
    m_frameAhead = 0.0f;
    m_saveFrame = 0.0f;
    m_startFrame = 0.0f;
    m_animFrame = 0.0f;
    if (m_anmChrRes) {
        if (destroy) {
            m_anmChrRes->Destroy();
        }
        m_anmChrRes = 0;
    }
}

#define INTERNAL_SET_FRAME(frame, endFrame)                         \
    do {                                                            \
        if (m_reverseFlag == 0) {                                   \
            m_anmChrRes->SetFrame(frame);                           \
        } else {                                                    \
            m_anmChrRes->SetFrame((endFrame) - (float)(frame));     \
        }                                                           \
    } while (false)

void soAnimChr::reset() {
    reinit(true);
}

void soAnimChr::relinquishAnimObj() {
    reinit(false);
}

void soAnimChr::updateFrame(float delta) {
    const float increment = (m_rate * delta);
    m_animFrame += increment;
    m_frameAhead = m_animFrame + m_rate;
    if (m_anmChrRes) {
        const float endFrame = getEndFrame(m_anmChrRes);
        if (m_animFrame >= endFrame) {
            if (m_loopFlag == 1) {
                m_animFrame -= endFrame;
            } else {
                m_animFrame = endFrame;
            }
        }
        if (m_frameAhead >= endFrame) {
            if (m_loopFlag == 1) {
                m_frameAhead -= endFrame;
            } else {
                m_frameAhead = endFrame;
            }
        }
        if (m_animFrame != m_startFrame) {
            INTERNAL_SET_FRAME(m_animFrame, endFrame);
        }
    }
}

void soAnimChr::setFrame(float frame) {
    m_animFrame = m_frameAhead = frame;
    if (m_anmChrRes) {
        INTERNAL_SET_FRAME((double)frame, getEndFrame(m_anmChrRes));
    }
}

bool soAnimChr::isEnd() const {
    if (m_anmChrRes && m_loopFlag == 0 && m_animFrame >= getEndFrame(m_anmChrRes)) {
        return true;
    }
    return false;
}

bool soAnimChr::isLooped() const {
    if (m_loopFlag == 1 && m_animFrame >= 0.0f && m_animFrame < m_rate && m_animFrame < m_startFrame) {
        return true;
    }
    return false;
}

void soAnimChr::setAnimObj(float startFrame, float rate, nw4r::g3d::AnmObjChrRes *anmChrRes, u8 loopFlg, u8 reverseFlg) {
    m_anmChrRes = anmChrRes;
    m_rate = rate;
    m_reverseFlag = reverseFlg;
    m_loopFlag = loopFlg;
    m_frameAhead = startFrame;
    m_animFrame = startFrame;
    if (m_anmChrRes) {
        INTERNAL_SET_FRAME((double)startFrame, getEndFrame(anmChrRes));
    }
    m_startFrame = startFrame;
}

void soAnimChr::destroyAnimObj() {
    if (m_anmChrRes) {
        m_anmChrRes->Destroy();
        m_anmChrRes = 0;
    }
}

void soAnimChr::disableNode(u32 id) {
    if (m_anmChrRes) {
        m_anmChrRes->DisableID(id);
    }
}

float soAnimChr::getEndFrame(nw4r::g3d::AnmObjChrRes* anmChrRes) const {
    return (s32)anmChrRes->m_anmChrFile->m_1C - 1.0f;
}

void soAnimChr::rewind() {
    m_saveFrame = m_animFrame;
    m_animFrame = m_frameAhead = m_startFrame;
    if (m_anmChrRes) {
        INTERNAL_SET_FRAME(m_startFrame, getEndFrame(m_anmChrRes));
    }
}

void soAnimChr::turnBack() {
    m_animFrame = m_frameAhead = m_saveFrame;
    if (m_anmChrRes) {
        INTERNAL_SET_FRAME(m_saveFrame, getEndFrame(m_anmChrRes));
    }
}
