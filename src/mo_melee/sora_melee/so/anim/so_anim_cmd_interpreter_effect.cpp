#include <ac/ac_anim_cmd_impl.h>
#include <ac/ac_cmd_interpreter.h>
#include <so/effect/so_effect_screen.h>
#include <so/so_module_accesser.h>

class soAnimCmdInterpreterEffect : public acCmdInterpreter {
    soEffectScreen* m_effectScreen;
public:
    virtual void interpretNotSystemCmd();
};

void soAnimCmdInterpreterEffect::interpretNotSystemCmd() {
    const acAnimCmd* currCmd = getCurrentCmd();
    if (currCmd->isNull() != true && currCmd->getGroup() == 0x11) {
        m_effectScreen->notifyEventAnimCmd(currCmd, &g_soModuleAccesserNull);
    }
}
