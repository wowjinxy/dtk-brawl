#include <gf/gf_archive.h>
#include <nw4r/g3d/g3d_resfile.h>
#include <so/so_archive_db.h>
#include <so/resource/so_resource_module_impl.h>
#include <so/resource/so_resource_id_accesser.h>
#include <types.h>
#include <ut/ut_archive_manager.h>

soResourceModuleImpl::
soResourceModuleImpl(u32 mId, soResourceIdAccesserImpl* rsrcIdAcc, u32 arcGrp) {
    m_managerID = mId;
    m_resourceIdAccesser = rsrcIdAcc;
    m_archiveType1 = arcGrp;
    m_archiveType2 = arcGrp;
    m_archiveType3 = arcGrp;
    m_archiveType4 = arcGrp;
}

soResourceModuleImpl::~soResourceModuleImpl() { }

void soResourceModuleImpl::setGroupNo(u8 unk1, u16 index) {
    if (index == 0xFF) {
        m_archiveType1 = unk1;
        m_archiveType2 = unk1;
        m_archiveType3 = unk1;
        m_archiveType4 = unk1;
        return;
    }
    u8* ptr = (u8*)((u32)this + index);
    ptr[0x14] = unk1;
}

nw4r::g3d::ResFileData* soResourceModuleImpl::getTexFile(u32 p1, u32 p2) {
    if (p2 == 0xFFFF) {
        p2 = m_resourceIdAccesser->getTexResId();
    }
    if (p2 != 0xFFFF) {
        u32 arcId = m_resourceIdAccesser->getTexArchiveId();
        u32 grpNo = getGroupNo(2);
        utArchiveManager* arcManager = soArchiveDb::getManager(m_managerID);
        return arcManager->getResFileFromId(p2, Data_Type_Tex, p1, (u8)grpNo, arcId);
    }
    return 0;
}

nw4r::g3d::ResFileData* soResourceModuleImpl::getMdlFile(u32 p1, u32 p2) {
    if (p2 == 0xFFFF) {
        p2 = m_resourceIdAccesser->getMdlResId();
    }
    if (p2 != 0xFFFF) {
        u32 arcId = m_resourceIdAccesser->getMdlArchiveId();
        u32 grpNo = getGroupNo(1);
        utArchiveManager* arcManager = soArchiveDb::getManager(m_managerID);
        return arcManager->getResFileFromId(p2, Data_Type_Model, p1, (u8)grpNo, arcId);
    }
    return 0;
}

nw4r::g3d::ResFileData* soResourceModuleImpl::getAnmFile(u32 p1, u32 p2, u32 p3) {
    u32 arcId = m_resourceIdAccesser->getAnmArchiveId();
    if (p2 == 0xFFFF) {
        p2 = m_resourceIdAccesser->getAnmResId();
    }
    if (p3 == 1) {
        arcId = -1;
    }
    if (p2 != 0xFFFF) {
        p3 = getGroupNo(3);
        utArchiveManager* arcManager = soArchiveDb::getManager(m_managerID);
        return arcManager->getResFileFromId(p2, Data_Type_Anim, p1, (u8)p3, arcId);
    }
    return 0;
}

nw4r::g3d::ResFileData* soResourceModuleImpl::getBinFile(u32 p1, u32 p2, u32 p3) {
    u32 r31 = p1;
    if (p1 == 0xFFFF) {
        r31 = m_resourceIdAccesser->getBinResId();
    }
    if ((s32)p3 == -1) {
        p3 = m_resourceIdAccesser->getBinArchiveId();
    }
    if (r31 != 0xFFFF) {
        r31 = getGroupNo(0);
        utArchiveManager* arcManager = soArchiveDb::getManager(m_managerID);
        return arcManager->getResFileFromId(p1, Data_Type_Misc, p2, (u8)r31, p3);
    }
    return 0;
}

nw4r::g3d::ResFileData* soResourceModuleImpl::getFile(u32 p1, u32 p2, u32 p3) {
    if (p1 != 0xFFFF) {
        u32 arcId = m_resourceIdAccesser->getBinArchiveId();
        u32 grpNo = getGroupNo(0);
        utArchiveManager* arcManager = soArchiveDb::getManager(m_managerID);
        return arcManager->getResFileFromId(p1, (ARCNodeType)p2, p3, (u8)grpNo, arcId);
    }
    return 0;
}
