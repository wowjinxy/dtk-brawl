#include <ac/ac_anim_cmd_impl.h>
#include <so/so_array.h>
#include <types.h>

s8 acAnimCmdImpl::getGroup() const {
    return cmdAddr->group;
}

s8 acAnimCmdImpl::getType() const {
    return cmdAddr->type;
}

s32 acAnimCmdImpl::getArgNum() const {
    return (!cmdAddr) ? 0 : cmdAddr->argNum;
}

u8 acAnimCmdImpl::getOption() const {
    return (!cmdAddr) ? 0 : cmdAddr->option;
}

bool acAnimCmdImpl::getArg(acCmdArg* arg, s32 index) const {
    if (isArgEmpty() == true || !cmdAddr->args) {
        *arg = g_acCmdArg;
        return false;
    }
    if (index < 0 || index >= getArgNum()) {
        *arg = g_acCmdArg;
        return false;
    }
    arg->setDataPtr(cmdAddr->args + index);
    arg->setNull(false);
    return true;
}

bool acAnimCmdImpl::isArgEmpty() const {
    return !getArgNum();
}

soArrayContractibleTable<const acCmdArgConv> acAnimCmdImpl::getArgList() {
    s32 argNum;
    const acCmdArgConv* args;
    if ((argNum = getArgNum()) <= 0 ||
        (args = cmdAddr->args) == 0) {
        return soArrayContractibleTable<const acCmdArgConv>();
    } else {
        return soArrayContractibleTable<const acCmdArgConv>(args, argNum);
    }
}

bool acAnimCmdImpl::isValid() const {
    if (!cmdAddr || cmdAddr->type < 0 || cmdAddr->group <  0) {
        return false;
    }
    return true;
}

// TODO: inferred deadstripped function to correct function ordering
acCmdArgList acAnimCmdImpl::getEmptyArgList() {
    return acCmdArgList();
}
