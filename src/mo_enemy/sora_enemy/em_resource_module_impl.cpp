#include <em/em_info.h>
#include <em/enemy.h>
#include <em/em_resource_module_impl.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <so/resource/so_resource_id_accesser.h>
#include <so/resource/so_resource_module_impl.h>
#include <types.h>
#include <ut/ut_archive_manager.h>
#include <so/so_archive_db.h>

emResourceModuleImpl::emResourceModuleImpl(
    u32 paramArcId,
    soResourceIdAccesserImpl* resourceIdAccessor,
    u32 emCommonArcId,
    emCreate* create) :
    soResourceModuleImpl(1, resourceIdAccessor, 0) {

    // TODO: Determine if this is a real enum that belongs in the function signatures,
    // or if it should remain localized to this function
    enum unkEnum { UNK_ZERO };

    unk18 = 0;
    unk1C = 0;
    unk20 = 0;
    unk24 = 0;
    unk28 = 0;
    unk2C = 0;
    unk30 = 0;
    unk34 = 0;
    m_kind = create->m_enemyKind;
    if (paramArcId == 0xFFFF) {
        return;
    }
    if (emCommonArcId == 0xFFFF) {
        return;
    }

    utArchiveManager* arcManager = soArchiveDb::getManager(1);
    unkEnum ecArcId = (unkEnum)emCommonArcId;
    const char* name = "commonData";
    unkEnum zero = UNK_ZERO;
    void* addr = arcManager->getPublicAddress(ecArcId, name, 0, zero, -1);
    if (!addr) {
        return;
    }
    unk18 = addr;
    unk1C = *(void**)addr;

    arcManager = soArchiveDb::getManager(1);
    unkEnum pArcId = (unkEnum)paramArcId;
    ecArcId = (unkEnum)emCommonArcId;
    zero = UNK_ZERO;
    arcManager->resolveReference(pArcId, ecArcId, zero, 0, zero, zero);

    arcManager = soArchiveDb::getManager(1);
    pArcId = (unkEnum)paramArcId;
    name = "data";
    zero = UNK_ZERO;
    addr = arcManager->getPublicAddress(pArcId, name, 0, zero, -1);
    if (!addr) {
        return;
    }
    unk20 = addr;
    unk24 = ((void**)addr)[6];
    unk30 = ((void**)addr)[7];
    unk34 = ((void**)addr)[8];

    ModifyParam(create);
    emInfo::getInstance()->getExParamModifyFuncPtr(m_kind)(this, create);
}

emResourceModuleImpl::~emResourceModuleImpl() {
    emInfo::getInstance()->getExParamDeleteFuncPtr(m_kind)(this);
}

void emResourceModuleImpl::ModifyParam(emCreate* create) {
    unk28 = create->m_epbm ? create->m_epbm : unk30;
}

nw4r::g3d::ResFileData* emResourceModuleImpl::getInfoTexFile(u32 p1, u32 p2) {
    const u32 r30 = p1;
    const u32 r31 = p2;
    if (p1 != 0xFFFF) {
        utArchiveManager* arcManager = soArchiveDb::getManager(1);
        return arcManager->getResFileFromId(r30, Data_Type_Tex, r31, 1, -1);
    }
    return 0;
}
