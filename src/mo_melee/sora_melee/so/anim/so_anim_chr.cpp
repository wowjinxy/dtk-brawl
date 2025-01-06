#include <StaticAssert.h>
#include <nw4r/g3d/g3d_anmchr.h>
#include <so/anim/so_anim_chr.h>
#include <types.h>

soAnimChr::soAnimChr() {
    m_animFrame = 0.0f;
    m_anmChrRes = 0;
    m_frame_ = 0.0f;
    m_frameSpeedModifier = 0.0f;
    m_rate = 1.0f;
    m_frame = 0.0f;
    m_loopFlag = 0;
    m_unkFlag = 0;
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
    m_unkFlag = 0;
    m_frame = 0.0f;
    m_frameSpeedModifier = 0.0f;
    m_frame_ = 0.0f;
    m_animFrame = 0.0f;
    if (m_anmChrRes) {
        if (destroy) {
            m_anmChrRes->Destroy();
        }
        m_anmChrRes = 0;
    }
}

void soAnimChr::reset() {
    reinit(true);
}

void soAnimChr::relinquishAnimObj() {
    reinit(false);
}

void soAnimChr::updateFrame(float p1) {
    const float scaled_p1 = (m_rate * p1);
    m_animFrame += scaled_p1;
    m_frame = m_animFrame + m_rate;
    if (m_anmChrRes) {
        const float endFrame = getEndFrame(m_anmChrRes);
        if (m_animFrame >= endFrame) {
            if (m_loopFlag == 1) {
                m_animFrame -= endFrame;
            } else {
                m_animFrame = endFrame;
            }
        }
        if (m_frame >= endFrame) {
            if (m_loopFlag == 1) {
                m_frame -= endFrame;
            } else {
                m_frame = endFrame;
            }
        }
        if (m_animFrame != m_frame_) {
            if (m_unkFlag == 0) {
                m_anmChrRes->SetFrame(m_animFrame);
            } else {
                m_anmChrRes->SetFrame(endFrame - m_animFrame);
            }
        }
    }
}

void soAnimChr::setFrame(float p1) {
    m_animFrame = m_frame = p1;
    if (m_anmChrRes) {
        if (m_unkFlag == 0) {
            m_anmChrRes->SetFrame((double)p1);
        } else {
            m_anmChrRes->SetFrame(getEndFrame(m_anmChrRes) - (float)(double)p1);
        }
    }
}

bool soAnimChr::isEnd() const {
    if (m_anmChrRes && m_loopFlag == 0 && m_animFrame >= getEndFrame(m_anmChrRes)) {
        return true;
    }
    return false;
}

bool soAnimChr::isLooped() const {
    if (m_loopFlag == 1 && m_animFrame >= 0.0f && m_animFrame < m_rate && m_animFrame < m_frame_) {
        return true;
    }
    return false;
}

void soAnimChr::setAnimObj(float p1, float p2, nw4r::g3d::AnmObjChrRes *p3, u8 p4, u8 p5) {
    m_anmChrRes = p3;
    m_rate = p2;
    m_unkFlag = p5;
    m_loopFlag = p4;
    m_frame = p1;
    m_animFrame = p1;
    if (p3) {
        if (m_unkFlag == 0) {
            p3->SetFrame((double)p1);
        } else {
            p3->SetFrame(getEndFrame(p3) - (float)(double)p1);
        }
    }
    m_frame_ = p1;
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

float soAnimChr::getEndFrame(nw4r::g3d::AnmObjChrRes* p1) const {
    return (s32)p1->m_anmChrFile->m_1C - 1.0f;
}

void soAnimChr::rewind() {
    m_frameSpeedModifier = m_animFrame;
    m_animFrame = m_frame = m_frame_;
    if (m_anmChrRes) {
        if (m_unkFlag == 0) {
            m_anmChrRes->SetFrame(m_frame_);
        } else {
            m_anmChrRes->SetFrame(getEndFrame(m_anmChrRes) - m_frame_);
        }
    }
}

void soAnimChr::turnBack() {
    m_animFrame = m_frame = m_frameSpeedModifier;
    if (m_anmChrRes) {
        if (m_unkFlag == 0) {
            m_anmChrRes->SetFrame(m_frameSpeedModifier);
        } else {
            m_anmChrRes->SetFrame(getEndFrame(m_anmChrRes) - m_frameSpeedModifier);
        }
    }
}
