#include <ac/ac_cmd_interpreter.h>
#include <ac/ac_anim_cmd_impl.h>
#include <mt/mt_prng.h>
#include <so/so_array.h>
#include <so/so_null.h>
#include <types.h>

typedef void (acCmdInterpreter::*SystemCmdFunc)(void);

const static SystemCmdFunc SystemCmdFuncTable[25] = {
    &acCmdInterpreter::systemCmdFuncEnd,
    &acCmdInterpreter::systemCmdFuncWait,
    &acCmdInterpreter::systemCmdFuncFrame,
    &acCmdInterpreter::systemCmdFuncFrameClr,
    &acCmdInterpreter::systemCmdFuncLoop,
    &acCmdInterpreter::systemCmdFuncPool,
    &acCmdInterpreter::systemCmdFuncBreak,
    &acCmdInterpreter::systemCmdFuncCall,
    &acCmdInterpreter::systemCmdFuncReturn,
    &acCmdInterpreter::systemCmdFuncJump,
    &acCmdInterpreter::systemCmdFuncIf,
    &acCmdInterpreter::systemCmdFuncIfAnd,
    &acCmdInterpreter::systemCmdFuncIfOr,
    &acCmdInterpreter::systemCmdFuncElsIf,
    &acCmdInterpreter::systemCmdFuncElse,
    &acCmdInterpreter::systemCmdFuncEndIf,
    &acCmdInterpreter::systemCmdFuncSwitch,
    &acCmdInterpreter::systemCmdFuncCase,
    &acCmdInterpreter::systemCmdFuncDefault,
    &acCmdInterpreter::systemCmdFuncEndSwitch,
    &acCmdInterpreter::systemCmdFuncWhile,
    &acCmdInterpreter::systemCmdFuncWhileAnd,
    &acCmdInterpreter::systemCmdFuncWhileOr,
    &acCmdInterpreter::systemCmdFuncEndWhile,
    &acCmdInterpreter::systemCmdFuncBreakSwitch
};

acCmdInterpreter::acCmdInterpreter(soArrayVector<acCmdInterpreterStackData, 8>* stk,
        s32 mode, acAnimCmdConv* initCmd, const u8* resetMode, float fixedTimeStep) :
    m_fixedTimeStep(fixedTimeStep),
    m_lastTimeStep(-1.0f),
    m_currentCmd(initCmd),
    m_copyCurrentCmd(initCmd),
    m_stack(stk),
    m_currentFrame(0.0f),
    unk28(-1.0f),
    m_overshoot(0.0f),
    m_interpretMode(mode),
    unk30Field2(0),
    m_dontAdvance(false),
    unk32(0),
    m_reset(*resetMode) { }

acCmdInterpreter::~acCmdInterpreter() { }

inline bool acCmdInterpreter::testCurrentCmd() const {
    return m_currentCmd != 0;
}

inline void acCmdInterpreter::advanceCurrentCmd() {
    if (m_currentCmd) {
        m_currentCmd++;
    }
}

inline const acAnimCmd* acCmdInterpreter::updateCurrentCmd() {
    const acAnimCmdConv* currCmd = m_currentCmd;
    if (m_animCmd.getCmdAddress() != currCmd) {
        m_animCmd.m_isNull = false;
        m_animCmd.cmdAddr = currCmd;
    }
    return &m_animCmd;
}

inline s32 acCmdInterpreter::getIdxOfStackData(const acCmdInterpreterStackData::Kind kind) const {
    for (s32 i = m_stack->size() - 1; i >= 0; i--) {
        if (m_stack->at(i).kind == kind) {
            return i;
        }
    }
    return -1;
}

// Pop the stack until its size is newSize
inline void acCmdInterpreter::popStackToSize(const s32 newSize) {
    s32 size = m_stack->size();
    if (newSize >= 0 && newSize < size) {
        if (newSize == size - 1) {
            m_stack->pop();
        } else {
            for (s32 i = 0; i < size - newSize; i++) {
                m_stack->pop();
            }
        }
    }
}

inline void acCmdInterpreter::interpretOneCmd() {
    bool isNonSystemCmd;
    SystemCmdFunc scf;
    if (!m_currentCmd) {
        isNonSystemCmd = true;
    } else if (m_currentCmd->group >= 1) {
        isNonSystemCmd = false;
    } else if (m_currentCmd->type < 0 || m_currentCmd->type >= 25 || m_currentCmd->group < 0) {
        isNonSystemCmd = true;
    } else if (!(scf = SystemCmdFuncTable[m_currentCmd->type])) {
        isNonSystemCmd = true;
    } else {
        (this->*scf)();
        isNonSystemCmd = true;
    }

    if (!isNonSystemCmd) {
        interpretNotSystemCmd();
    }

    if (m_dontAdvance == true) {
        m_dontAdvance = false;
    } else {
        advanceCurrentCmd();
    }
}

void acCmdInterpreter::interpretCmd(float timeStep) {
    const acAnimCmdConv* cmd;
    if (m_interpretMode > -1) {
        unk30Field2 = 0;
        if ((m_reset & 0x1) && 0 != (cmd = m_copyCurrentCmd)) {
            m_currentCmd = m_copyCurrentCmd = cmd;
            m_stack->clear();
            m_dontAdvance = false;
            m_currentFrame = 0.0f;
            m_overshoot = 0.0f;
        }
        if (m_currentCmd) {
            if (m_interpretMode == 0) {
                timeStep = m_fixedTimeStep;
            }
            if (timeStep < 0.0f) {
                return;
            }
            m_lastTimeStep = timeStep;
            if (unk32 == 1) {
                unk32 = 0;
            } else {
                float f = m_currentFrame;
                m_currentFrame = f + static_cast<float>(static_cast<double>(timeStep));
            }
            acCmdInterpreterStackData* top = getLastSystemStackData();
            if (top && top->kind == acCmdInterpreterStackData::knd0) { // Wait
                top->floatData -= m_lastTimeStep;
            }

            // Execute the ACMD script
            while (m_currentCmd && !isBreakInterpret()) {
                interpretOneCmd();
            }
        }
    }
}

void acCmdInterpreter::interpretNotSystemCmd() {

}

// Direct interpreter at new command provided
void acCmdInterpreter::resetInterpreter(float frame, const acAnimCmdConv *initCmd) {
    m_currentCmd = m_copyCurrentCmd = initCmd;
    m_stack->clear();
    m_dontAdvance = false;
    const float zero = 0;
    m_overshoot = m_currentFrame = 0;
    if (frame > 0 && !(frame <= 0)) {
        unk30Field2 = 2;
        unk28 = frame;
        m_currentFrame = zero + frame;
        // NOTE: similar to skipCmd
        if (m_interpretMode <= -1) {
            ;
        } else {
            acCmdInterpreterStackData* top;
            s32 size;
            // TODO: check for inlined function
            while (0 != (top = (size = m_stack->size(),
                (size > 0) ? &m_stack->at(size - 1) : 0))) {
                if (!isSleepStackData(top->kind)) {
                    break;
                }
                popSystemStack();
            }
            while (!isSkipEnd()) {
                interpretOneCmd();
            }

            for (s32 i = m_stack->size() - 1; i >= 0; i--) {
                if (m_stack->at(i).kind == acCmdInterpreterStackData::knd7) {
                    m_stack->erase(i);
                }
            }
        }
        unk28 = -1.0f;
    }
}

bool acCmdInterpreter::isBreakInterpret() {
    if (!m_currentCmd) {
        return true;
    }

    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top) {
        return false;
    }

    switch (top->kind) {
        case acCmdInterpreterStackData::knd0: { // Wait
            if (top->floatData <= 0.0f) {
                // Done waiting
                m_overshoot = -top->floatData;
                popSystemStack();
                return false;
            } else {
                return true;
            }
            break;
        }
        case acCmdInterpreterStackData::Frame: { // Frame
            float waitFrame = top->floatData;
            if (m_currentFrame >= waitFrame) {
                // Reached the desired frame
                m_overshoot = m_currentFrame - waitFrame;
                popSystemStack();
                return false;
            } else {
                return true;
            }
            break;
        }
        default: {
            break;
        }
    }
    return false;
}

bool acCmdInterpreter::isSkipEnd() const {
    if (unk30Field2 == 2) {
        return unk28 < 0.0f;
    }
    return false;
}

// Advance m_currentCmd until it is pointing at this if-block's EndIf command
void acCmdInterpreter::skipNextIfGroup(bool skippingIfBlock) {
    if (!m_currentCmd) {
        return;
    }

    // Keep track of If nesting within the skipped block, so
    // a nested EndIf is not confused with this if group's EndIf
    s32 nestedIfBlocks = 0;

    while (true) {
        if (m_currentCmd->group == 0) { // SystemCmd
            s8 type = m_currentCmd->type;
            if (type >= 0x19) {
                return;
            }
            switch (type) {
                case 0: { // End
                    m_dontAdvance = true;
                    return;
                }
                case 0xA: { // If
                    nestedIfBlocks++;
                    break;
                }
                case 0xB: case 0xC: { // IfAnd or IfOr
                    // Only care about IfAnd/IfOr commands if they follow the
                    // outermost If command... not if they appear in an ElsIf
                    if (skippingIfBlock && nestedIfBlocks <= 0) {
                        m_dontAdvance = true;
                        return; // need to evaluate the next AND/OR term
                    }
                    break;
                }
                case 0xF: { // EndIf
                    if (nestedIfBlocks > 0) {
                        nestedIfBlocks--;
                        break;
                    }
                }
                // FALL-THROUGH
                case 0xD: case 0xE: { // ElsIf or Else
                    if (nestedIfBlocks <= 0) {
                        m_dontAdvance = true;
                        return;
                    }
                }
            }
        }
        advanceCurrentCmd();
    }
    m_dontAdvance = true;
}

void acCmdInterpreter::popSystemStack() {
    if (!m_stack->isEmpty())
        m_stack->pop();
}

void acCmdInterpreter::pushSystemStack(const acCmdInterpreterStackData& stackData) {
    m_stack->push(stackData);
}

void acCmdInterpreter::skipNextSwitchGroup(bool doNotSkipCases) {
    if (!testCurrentCmd()) {
        return;
    }

    s32 nestedSwitchBlocks = 0;
    while (true) {
        const acAnimCmd* currCmd = getCurrentCmd();
        if (!currCmd->isNull() && currCmd->getGroup() == 0) {
            s8 type = currCmd->getType();
            if (type >= 0x19) {
                return;
            }
            switch (type) {
                case 0: {
                    m_dontAdvance = true;
                    return;
                }
                case 0x10: {
                    // Switch
                    nestedSwitchBlocks++;
                    break;
                }
                case 0x13: {
                    // EndSwitch
                    if (nestedSwitchBlocks > 0) {
                        nestedSwitchBlocks--;
                    } else {
                        m_dontAdvance = true;
                        return;
                    }
                    break;
                }
                case 0x11: case 0x12: {
                    // Case or Default
                    if (doNotSkipCases && nestedSwitchBlocks <= 0) {
                        m_dontAdvance = true;
                        return;
                    }
                    break;
                }
                default: {
                    break;
                }
            }
        }
        advanceCurrentCmd();
    }
    m_dontAdvance = true;
}

// Skip consecutive Case and Default commands
void acCmdInterpreter::skipNextNotSwitchGroup() {
    if (!m_currentCmd) {
        return;
    }

    while (true) {
        const acAnimCmd* currCmd = getCurrentCmd();
        if (currCmd->isNull() == true || currCmd->getGroup() != 0) {
            m_dontAdvance = true;
            return;
        }
        s8 type = currCmd->getType();
        if (type >= 25) {
            return;
        }
        switch (type) {
            case 0x10:
            case 0:
            case 0x13:
            default: {
                m_dontAdvance = true;
                return;
            }
            case 0x11: case 0x12: {
                // Case or Default
                advanceCurrentCmd();
                break;
            }
        }
    }
}

// Skip to the end of this "Loop" block.
// Note that Pool marks the end of a Loop block. The command pointer,
// m_currentCmd, will be advanced to the command immediately
// following the matching Pool because m_dontAdvance is not set
void acCmdInterpreter::skipNextLoopGroup() {
    if (!m_currentCmd) {
        return;
    }

    s32 nestedLoopBlocks = 0;
    while (true) {
        const acAnimCmd* currCmd = getCurrentCmd();
        if (!currCmd->isNull() && currCmd->getGroup() == 0) {
            s8 type = currCmd->getType();
            if (type >= 0x19) {
                return;
            }
            switch (type) {
                case 0x5: {
                    if (nestedLoopBlocks > 0) {
                        nestedLoopBlocks--;
                        break;
                    }
                    return;
                }
                case 0x4: {
                    nestedLoopBlocks++;
                }
            }
        }
        advanceCurrentCmd();
    }
}

void acCmdInterpreter::skipNextWhileGroup() {
    if (!m_currentCmd) {
        return;
    }

    s32 nestedWhileBlocks = 0;
    while (true) {
        const acAnimCmd* currCmd = getCurrentCmd();
        if (!currCmd->isNull() && currCmd->getGroup() == 0) {
            s8 type = currCmd->getType();
            if (type >= 0x19)
                return;
            switch (type) {
                case 0: {
                    m_dontAdvance = true;
                    return;
                }
                case 0x14: {
                    // While
                    nestedWhileBlocks++;
                    break;
                }
                case 0x15: case 0x16: {
                    if (nestedWhileBlocks <= 0) {
                        m_dontAdvance = true;
                        return;
                    }
                    break;
                }
                case 0x17: {
                    // EndWhile
                    if (nestedWhileBlocks > 0) {
                        nestedWhileBlocks--;
                        break;
                    } else {
                        m_dontAdvance = true;
                        return;
                    }
                }
            }
        }
        advanceCurrentCmd();
    }
}

const acAnimCmd* acCmdInterpreter::getCurrentCmd() {
    if (!testCurrentCmd()) {
        return &g_acAnimCmdNull;
    }
    return updateCurrentCmd();
}

acCmdInterpreterStackData* acCmdInterpreter::getLastSystemStackData() {
    s32 size = m_stack->size();
    return (size <= 0) ? 0 : &m_stack->at(size - 1);
}

s32 acCmdInterpreter::dispatchCmdSwitch(const acAnimCmd* cmd) const {
    acCmdArg firstArg;
    if (!cmd->getArg(&firstArg, 0) || firstArg.getArgType() != 0) {
        return 0;
    }

    switch (firstArg.getIntData()) {
        default: {
            break;
        }
        case 0: {
            acCmdArg secondArg;
            if (cmd->getArg(&secondArg, 1) == true || !secondArg.isNull()) {
                return randi(secondArg.getIntData());
            }
            break;
        }
    }
    return 0;
}

void acCmdInterpreter::skipCmd() {
    if (m_interpretMode <= -1) {
        return;
    } else {
        acCmdInterpreterStackData* top;
        s32 size;
        while (0 != (top = (size = m_stack->size(),
            (size > 0) ? &m_stack->at(size - 1) : 0))) {
            if (!isSleepStackData(top->kind)) {
                break;
            }
            popSystemStack();
        }
        while (!isSkipEnd()) {
            interpretOneCmd();
        }

        for (s32 i = m_stack->size() - 1; i >= 0; i--) {
            if (m_stack->at(i).kind == acCmdInterpreterStackData::knd7) {
                m_stack->erase(i);
            }
        }
    }
}

bool acCmdInterpreter::isSleepStackData(s32 kind) const {
    return kind == acCmdInterpreterStackData::knd0 || kind == acCmdInterpreterStackData::Frame;
}

void acCmdInterpreter::systemCmdFuncWait() {
    if (testCurrentCmd()) {
        const acAnimCmd* currCmd = getCurrentCmd();
        float waitTime = 0.0f;
        if (currCmd->getArgNum() > 0) {
            acCmdArg arg;
            if (!currCmd->getArg(&arg, 0) || arg.getArgType() != 1) {
                return;
            }
            waitTime = arg.getFloatData();
        }
        systemCmdFuncWaitSub(waitTime);
    }
}

void acCmdInterpreter::systemCmdFuncWaitSub(float waitTime) {
    float f31 = waitTime;
    s32 r0 = unk30Field2;
    if (r0 == 2 && unk28 >= 0.0f) {
        s32 i = getIdxOfStackData(acCmdInterpreterStackData::knd7);
        float f2 = 0.0f;
        if (i >= 0) {
            f2 = m_stack->at(i).floatData;
            float diff = (m_currentFrame - unk28);
            f2 -= diff;
            if (f2 < 0.0f) {
                f2 = 0.0f;
            }
        }
        f2 = unk28 - waitTime - f2;
        if (f2 >= 0.0f) {
            unk28 = f2;
            return;
        }
        unk28 = -1.0f;
        acCmdInterpreterStackData data(acCmdInterpreterStackData::knd0, 0, false, 0, -f2);
        pushSystemStack(data);
    } else if (r0 == 0) {
        if (m_overshoot > 0.0f) {
            f31 = waitTime - m_overshoot;
            if (f31 <= 0.0f) {
                m_overshoot = -f31;
                return;
            } else {
                m_overshoot = 0.0f;
            }
        }
        // Push the wait time onto the stack:
        // if m_overshoot <= 0: push waitTime
        // if m_overshoot > 0 and waitTime > m_overshoot, push waitTime-m_overshoot
        // if m_overshoot > 0 and waitTime <= m_overshoot, don't push anything
        acCmdInterpreterStackData data(acCmdInterpreterStackData::knd0, 0, false, 0, f31);
        pushSystemStack(data);
    }
}

void acCmdInterpreter::systemCmdFuncFrame() {
    const acAnimCmd* currCmd = getCurrentCmd();
    if (currCmd->isArgEmpty() != true) {
        acCmdArg arg;
        if (!currCmd->getArg(&arg, 0) || arg.getArgType() != 1) {
            return;
        }
        s32 r0 = unk30Field2;
        if (r0 == 2) {
            if (arg.getFloatData() <= m_currentFrame) {
                s32 i = getIdxOfStackData(acCmdInterpreterStackData::knd7);
                if (i < 0) {
                    // No element, so push one onto the stack
                    acCmdInterpreterStackData data(acCmdInterpreterStackData::knd7, 0, false, 0, arg.getFloatData());
                    pushSystemStack(data);
                } else {
                    // update the pre-existing stack element with
                    // the argument of the Frame command if it is greater than
                    // the frame already on the stack
                    acCmdInterpreterStackData& stackPtr = m_stack->at(i);
                    float f0 = arg.getFloatData();
                    float f1 = stackPtr.floatData;
                    if (f1 < f0) {
                        stackPtr.floatData = arg.getFloatData();
                    }
                }
            } else {
                unk28 = -1.0f;
                acCmdInterpreterStackData data(acCmdInterpreterStackData::Frame, 0, false, 0, arg.getFloatData());
                pushSystemStack(data);
            }
        } else if (r0 == 0) {
            if (arg.getFloatData() <= m_currentFrame) {
                m_overshoot = m_currentFrame - arg.getFloatData();
            } else {
                acCmdInterpreterStackData data(acCmdInterpreterStackData::Frame, 0, false, 0, arg.getFloatData());
                pushSystemStack(data);
                m_overshoot = 0.0f;
            }
        }
    }
}

void acCmdInterpreter::systemCmdFuncFrameClr() {
    m_currentFrame = 0.0f;
}

void acCmdInterpreter::systemCmdFuncLoop() {
    const acAnimCmd* currCmd = getCurrentCmd();
    if (currCmd->isArgEmpty() != 1u) {
        acCmdArg arg;
        if (!currCmd->getArg(&arg, 0) || arg.getArgType() != 0) {
            return;
        }
        // Push loop count and start label
        acCmdInterpreterStackData stackData(acCmdInterpreterStackData::Loop, arg.getIntData(), false, m_currentCmd, 0.0f);
        pushSystemStack(stackData);
    }
}


void acCmdInterpreter::systemCmdFuncPool() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::Loop) {
        return;
    }

    if (top->intData < 0) {
        if (top->addressData) {
            m_currentCmd = top->addressData;
        }
        return;
    }

    if (--top->intData > 0) {
        if (top->addressData) {
            m_currentCmd = top->addressData;
        }
        return;
    }

    popSystemStack();
}

void acCmdInterpreter::systemCmdFuncBreak() {
    s32 i = getIdxOfStackData(acCmdInterpreterStackData::Loop);

    if (i >= 0) {
        m_stack->at(i); // no effect
        advanceCurrentCmd();
        skipNextLoopGroup();
        popStackToSize(i);
    }
}

void acCmdInterpreter::systemCmdFuncCall() {
    const acAnimCmdConv* currCmdAddr = getCurrentCmd()->getCmdAddress();
    acCmdArg arg;
    bool isCallValid;
    if (!getCurrentCmd()->getArg(&arg, 0)) {
        isCallValid = false;
    } else {
        void* callAddr = arg.getAddressData();
        if (!callAddr) {
            isCallValid = false;
        } else {
            if (callAddr) {
                m_currentCmd = (const acAnimCmdConv*) callAddr;
            }
            m_dontAdvance = true;
            isCallValid = true;
        }
    }

    if (isCallValid == true) {
        acCmdInterpreterStackData stackData(acCmdInterpreterStackData::ReturnAddress, 0, false, currCmdAddr, 0.0f);
        pushSystemStack(stackData);
    }
}

void acCmdInterpreter::systemCmdFuncJump() {
    acCmdArg arg;
    if (getCurrentCmd()->getArg(&arg, 0)) {
        void* jumpAddr = arg.getAddressData();
        if (jumpAddr) {
            if (jumpAddr) {
                m_currentCmd = (const acAnimCmdConv*) jumpAddr;
            }
            m_dontAdvance = true;
        }
    }
}

// Note: Equivalent to systemCmdFuncEnd, but generates slightly different asm
// NONMATCHING: r30/r31 regswap
void acCmdInterpreter::systemCmdFuncReturn() {
    const s32 i = getIdxOfStackData(acCmdInterpreterStackData::ReturnAddress);
    if (i < 0) {
        m_currentCmd = 0;
        m_stack->clear();
        m_dontAdvance = false;
    } else {
        const acAnimCmdConv* returnAddr = m_stack->at(i).addressData;
        if (returnAddr) {
            m_currentCmd = returnAddr;
        }
        popStackToSize(i);
    }
}

void acCmdInterpreter::systemCmdFuncIf() {
    if (dispatchCmdIf(getCurrentCmd()) == true) {
        acCmdInterpreterStackData stackData(acCmdInterpreterStackData::IfCondition, 0, true, 0, 0.0f);
        pushSystemStack(stackData);
    } else {
        acCmdInterpreterStackData stackData(acCmdInterpreterStackData::IfCondition, 0, false, 0, 0.0f);
        pushSystemStack(stackData);
        advanceCurrentCmd();
        skipNextIfGroup(true);
    }
}

void acCmdInterpreter::systemCmdFuncIfAnd() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::IfCondition) {
        return;
    }
    if (top->boolData == true) {
        if (!m_stack->isEmpty()) {
            m_stack->pop();
        }
        // equivalent to if (2nd operand)
        systemCmdFuncIf();
    } else {
        // short-circuit
        advanceCurrentCmd();
        skipNextIfGroup(true);
    }
}

void acCmdInterpreter::systemCmdFuncIfOr() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::IfCondition) {
        return;
    }
    if (top->boolData != true) {
        // 1st term is false
        if (!m_stack->isEmpty()) {
            m_stack->pop();
        }
        systemCmdFuncIf();
    }
}

void acCmdInterpreter::systemCmdFuncElsIf() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::IfCondition) {
        return;
    }
    if (top->boolData == true) {
        // 'if' clause evaluated to true -> skip block
        advanceCurrentCmd();
        skipNextIfGroup(false);
    } else { // proceed as normal if statement
        if (!m_stack->isEmpty()) {
            m_stack->pop();
        }
        systemCmdFuncIf();
    }
}

void acCmdInterpreter::systemCmdFuncElse() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::IfCondition) {
        return;
    }
    if (top->boolData == true) {
        advanceCurrentCmd();
        skipNextIfGroup(false);
    }
}

void acCmdInterpreter::systemCmdFuncEndIf() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::IfCondition) {
        return;
    }
    if (!m_stack->isEmpty()) {
        m_stack->pop();
    }
}

void acCmdInterpreter::systemCmdFuncSwitch() {
    const acAnimCmd* currCmd = getCurrentCmd();
    if (currCmd->isArgEmpty() != 1u) {
        s32 result = dispatchCmdSwitch(currCmd);
        acCmdInterpreterStackData data(acCmdInterpreterStackData::Switch, result, false, 0, 0.0f);
        pushSystemStack(data);
    }
}

void acCmdInterpreter::systemCmdFuncCase() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::Switch) {
        return;
    }
    const acAnimCmd* currCmd = getCurrentCmd();
    if (currCmd->isArgEmpty() == true) {
        advanceCurrentCmd();
        skipNextSwitchGroup(true);
    } else {
        acCmdArg arg;
        if (!currCmd->getArg(&arg, 0)) {
            advanceCurrentCmd();
            skipNextSwitchGroup(true);
        } else if (top->intData == arg.getIntData()) {
            top->boolData = true;
            advanceCurrentCmd();
            skipNextNotSwitchGroup();
        } else {
            advanceCurrentCmd();
            skipNextSwitchGroup(true);
        }
    }
}

void acCmdInterpreter::systemCmdFuncDefault() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::Switch) {
        return;
    }
    if (top->boolData == true) {
        advanceCurrentCmd();
        skipNextSwitchGroup(true);
    }
}

void acCmdInterpreter::systemCmdFuncEndSwitch() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::Switch) {
        return;
    }
    if (!m_stack->isEmpty()) {
        m_stack->pop();
    }
}

void acCmdInterpreter::systemCmdFuncBreakSwitch() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::Switch) {
        return;
    }
    advanceCurrentCmd();
    skipNextSwitchGroup(false);
}

void acCmdInterpreter::systemCmdFuncWhile() {
    if (dispatchCmdIf(getCurrentCmd()) == true) {
        acCmdInterpreterStackData data(acCmdInterpreterStackData::While, 0, true, m_currentCmd, 0.0f);
        pushSystemStack(data);
    } else {
        acCmdInterpreterStackData data(acCmdInterpreterStackData::While, 0, false, m_currentCmd, 0.0f);
        pushSystemStack(data);
        advanceCurrentCmd();
        skipNextWhileGroup();
    }
}

void acCmdInterpreter::systemCmdFuncWhileAnd() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::While) {
        return;
    }
    if (top->boolData == true) {
        if (dispatchCmdIf(getCurrentCmd()) != true) {
            // skip body, and do not jump back
            top->boolData = false;
            advanceCurrentCmd();
            skipNextWhileGroup();
        }
    } else {
        advanceCurrentCmd();
        skipNextWhileGroup();
    }
}

void acCmdInterpreter::systemCmdFuncWhileOr() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::While) {
        return;
    }
    if (top->boolData != true) {
        if (dispatchCmdIf(getCurrentCmd()) == true) {
            top->boolData = true;
        } else {
            advanceCurrentCmd();
            skipNextWhileGroup();
        }
    }
}

void acCmdInterpreter::systemCmdFuncEndWhile() {
    acCmdInterpreterStackData* top = getLastSystemStackData();
    if (!top || top->kind != acCmdInterpreterStackData::While) {
        return;
    }
    const acAnimCmdConv* jumpAddr = top->addressData;
    if (!m_stack->isEmpty()) {
        m_stack->pop();
    }
    if (top->boolData == true) {
        if (jumpAddr) {
            m_currentCmd = jumpAddr;
        }
        m_dontAdvance = true;
    }
}

void acCmdInterpreter::systemCmdFuncEnd() {
    const s32 i = getIdxOfStackData(acCmdInterpreterStackData::ReturnAddress);
    if (i < 0) {
        m_currentCmd = 0;
        m_stack->clear();
        m_dontAdvance = false;
    } else {
        const acAnimCmdConv* returnAddr = m_stack->at(i).addressData;
        if (returnAddr) {
            m_currentCmd = returnAddr;
        }
        popStackToSize(i);
    }
}
