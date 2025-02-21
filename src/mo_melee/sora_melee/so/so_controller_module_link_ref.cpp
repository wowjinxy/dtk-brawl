#include <so/controller/so_controller_impl.h>
#include <so/controller/so_controller_module_link_ref.h>
#include <so/link/so_link_module_impl.h>
#include <so/so_external_value_accesser.h>
#include <so/so_module_accesser.h>
#include <so/stageobject.h>
#include <types.h>

void soControllerModuleLinkRef::update(Input* p1, bool p2) {
    m_lkController = m_moAccessor->m_enumerationStart->m_linkModule->getLinkController();
    if (m_lkController && !m_lkController->isNull()) {
        updateClatter(m_lkController);
    }
}

float soControllerModuleLinkRef::getClatterThreshold() { return 0.5f; }

void soControllerModuleLinkRef::resetTrigger() {
    if (m_lkController) {
        m_lkController->resetTrigger();
    }
}

void soControllerModuleLinkRef::setRumble(s32 p1, s32 p2, bool p3, s32 p4) {
    StageObject* r3 = m_moAccessor->m_enumerationStart->m_linkModule->searchParentAttr(4);
    if (r3) {
        soExternalValueAccesser::getControllerModule(r3)->setRumble(p1, p2, p3, p4);
    }
}

void soControllerModuleLinkRef::stopRumbleKind(s32 p1, s32 p2) {
    StageObject* r3 = m_moAccessor->m_enumerationStart->m_linkModule->searchParentAttr(4);
    if (r3) {
        soExternalValueAccesser::getControllerModule(r3)->stopRumbleKind(p1, p2);
    }
}

void soControllerModuleLinkRef::stopRumble(bool p1) {
    StageObject* r3 = m_moAccessor->m_enumerationStart->m_linkModule->searchParentAttr(4);
    if (r3) {
        soExternalValueAccesser::getControllerModule(r3)->stopRumble(p1);
    }
}

void soControllerModuleLinkRef::setRumbleAll(s32 p1, s32 p2, s32 p3) {
    StageObject* r3 = m_moAccessor->m_enumerationStart->m_linkModule->searchParentAttr(4);
    if (r3) {
        soExternalValueAccesser::getControllerModule(r3)->setRumbleAll(p1, p2, p3);
    }
}

void soControllerModuleLinkRef::stopRumbleAll(s32 p1, s32 p2) {
    StageObject* r3 = m_moAccessor->m_enumerationStart->m_linkModule->searchParentAttr(4);
    if (r3) {
        soExternalValueAccesser::getControllerModule(r3)->stopRumbleAll(p1, p2);
    }
}

void soControllerModuleLinkRef::resetFlickBonusLr() { }

void soControllerModuleLinkRef::resetFlickBonus() { }

u8 soControllerModuleLinkRef::getFlickBonusLr() { return 0; }

u8 soControllerModuleLinkRef::getFlickBonus() { return 0; }

float soControllerModuleLinkRef::getLr() { return 0.0f; }

soController* soControllerModuleLinkRef::getController() {
    return (!m_lkController) ? &g_soControllerNull : m_lkController;
}

void soControllerModuleLinkRef::setLogActive(bool p1) { }

s32 soControllerModuleLinkRef::getLogNum() { return 0; }

void soControllerModuleLinkRef::clearLog() { }

void soControllerModuleLinkRef::setPrev(s32 p1) { }

void soControllerModuleLinkRef::setOff(bool p1) { }

ipPadButton soControllerModuleLinkRef::getRelease() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getRelease();
}

ipPadButton soControllerModuleLinkRef::getButton() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getButton();
}

u8 soControllerModuleLinkRef::getTriggerCountPrev(u8 index) {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getTriggerCountPrev(index);
}

u8 soControllerModuleLinkRef::getTriggerCount(u8 index) {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getTriggerCount(index);
}

ipPadButton soControllerModuleLinkRef::getTrigger() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getTrigger();
}

float soControllerModuleLinkRef::getSubStickPrevY() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getSubStickPrevY();
}

float soControllerModuleLinkRef::getSubStickY() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getSubStickY();
}

float soControllerModuleLinkRef::getSubStickPrevX() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getSubStickPrevX();
}

float soControllerModuleLinkRef::getSubStickX() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getSubStickX();
}

u8 soControllerModuleLinkRef::getFlickAfterY() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getFlickAfterY();
}

s8 soControllerModuleLinkRef::getFlickAfterXDir() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getFlickAfterXDir();
}

u8 soControllerModuleLinkRef::getFlickAfterX() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getFlickAfterX();
}

u8 soControllerModuleLinkRef::getFlickNoResetY() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getFlickNoResetY();
}

u8 soControllerModuleLinkRef::getFlickNoResetX() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getFlickNoResetX();
}

s8 soControllerModuleLinkRef::getFlickYDir() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getFlickYDir();
}

u8 soControllerModuleLinkRef::getFlickY() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getFlickY();
}

s8 soControllerModuleLinkRef::getFlickXDir() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getFlickXDir();
}

u8 soControllerModuleLinkRef::getFlickX() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getFlickX();
}

float soControllerModuleLinkRef::getStickPrevY() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getStickPrevY();
}

float soControllerModuleLinkRef::getStickY() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getStickY();
}

float soControllerModuleLinkRef::getStickPrevX() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getStickPrevX();
}

float soControllerModuleLinkRef::getStickX() {
    soController* controller = (!m_lkController) ? &g_soControllerNull : m_lkController;
    return controller->getStickX();
}

void soControllerModuleLinkRef::resetFlickY() { }

void soControllerModuleLinkRef::resetFlickX() { }

void soControllerModuleLinkRef::resetSubStick() { }

void soControllerModuleLinkRef::resetSubStickY() { }

void soControllerModuleLinkRef::resetSubStickX() { }

void soControllerModuleLinkRef::resetMainStick() { }

void soControllerModuleLinkRef::setMainStickY(float p1) { }

void soControllerModuleLinkRef::resetMainStickY() { }

void soControllerModuleLinkRef::setMainStickX(float p1) { }

void soControllerModuleLinkRef::resetMainStickX() { }

void soControllerModuleLinkRef::resetButton() { }

soControllerModuleLinkRef::~soControllerModuleLinkRef() { }

namespace so_controller_module_link_ref {
    extern const float unused_floats[] = { 0.25f, 0.25f, 0.25f };
}
