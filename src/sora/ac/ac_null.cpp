#include <ac/ac_anim_cmd_impl.h>
#include <so/so_array.h>
#include <types.h>

// TODO: Determine if this data is referenced anywhere
extern const u32 g_80409650[3] = { 0, 2, 11 };

acCmdArg g_acCmdArg;
acAnimCmdNull g_acAnimCmdNull;

acAnimCmdNull::~acAnimCmdNull() { }

u8 acAnimCmdNull::getOption() const {
    return 0;
}

bool acAnimCmdNull::isValid() const {
    return false;
}

bool acAnimCmdNull::isArgEmpty() const {
    return true;
}

const acAnimCmdConv* acAnimCmdNull::getCmdAddress() const {
    return 0;
}

bool acAnimCmdNull::getArg(acCmdArg* arg, s32 index) const {
    *arg = g_acCmdArg;
    return false;
}

soArrayContractibleTable<const acCmdArgConv> acAnimCmdNull::getArgList() {
    return soArrayContractibleTable<const acCmdArgConv>();
}

s32 acAnimCmdNull::getArgNum() const {
    return 0;
}

s8 acAnimCmdNull::getType() const {
    return -1;
}

s8 acAnimCmdNull::getGroup() const {
    return -1;
}
