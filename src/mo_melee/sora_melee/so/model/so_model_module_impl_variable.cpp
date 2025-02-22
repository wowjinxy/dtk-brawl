#include <mt/mt_matrix.h>
#include <mt/mt_vector.h>
#include <so/model/so_model_module_impl.h>
#include <so/so_module_accesser.h>
#include <types.h>

soModelModuleImplVariable::soModelModuleImplVariable(soModuleAccesser* p1, void* p2, u32 p3, u32 p4, void* p5, float f1)
    : soModelModuleImpl(p1, p2, p3, p4, p5, f1), unkCC(0) { }

soModelModuleImplVariable::~soModelModuleImplVariable() { }

static inline u32 minus400(u32 p1) { return p1 - 400; }

u32 soModelModuleImplVariable::getRealNodeId(u32 p1) {
    if (p1 < 400 || p1 == 0xFFFFFFFF) {
        return p1;
    }

    return (!unkCC) ? minus400(p1) : unkCC[0][minus400(p1)];
}

void soModelModuleImplVariable::setUpMtx(u32* p1, s32 p2) {
    u32 ids[200];
    for (s32 i = 0; i < p2; i++) {
        ids[i] = getRealNodeId(p1[i]);
    }
    soModelModuleImpl::setUpMtx(ids, p2);
}

void soModelModuleImplVariable::clearNodeSRT(u32 p1) {
    soModelModuleImpl::clearNodeSRT(getRealNodeId(p1));
}

void soModelModuleImplVariable::setNodeSRT(u32 p1, Vec3f* scale, Vec3f* rot, Vec3f* xlate) {
    soModelModuleImpl::setNodeSRT(getRealNodeId(p1), scale, rot, xlate);
}

Vec3f soModelModuleImplVariable::getNodeScale(u32 p1) {
    return soModelModuleImpl::getNodeScale(getRealNodeId(p1));
}

void soModelModuleImplVariable::setNodeScale(u32 p1, Vec3f* scale) {
    return soModelModuleImpl::setNodeScale(getRealNodeId(p1), scale);
}

Vec3f soModelModuleImplVariable::getNodeRotate(u32 p1) {
    return soModelModuleImpl::getNodeRotate(getRealNodeId(p1));
}

void soModelModuleImplVariable::setNodeRotateX(u32 p1, float x) {
    return soModelModuleImpl::setNodeRotateX(getRealNodeId(p1), x);
}

void soModelModuleImplVariable::setNodeRotateY(u32 p1, float y) {
    return soModelModuleImpl::setNodeRotateY(getRealNodeId(p1), y);
}

void soModelModuleImplVariable::setNodeRotateZ(u32 p1, float z) {
    return soModelModuleImpl::setNodeRotateZ(getRealNodeId(p1), z);
}

void soModelModuleImplVariable::setNodeRotate(u32 p1, Vec3f* rot) {
    return soModelModuleImpl::setNodeRotate(getRealNodeId(p1), rot);
}

Vec3f soModelModuleImplVariable::getNodeTranslate(u32 p1) {
    return soModelModuleImpl::getNodeTranslate(getRealNodeId(p1));
}

void soModelModuleImplVariable::setNodeTranslate(u32 p1, Vec3f* xlate) {
    return soModelModuleImpl::setNodeTranslate(getRealNodeId(p1), xlate);
}

u32 soModelModuleImplVariable::getNodeId(const char* nodeName) {
    u32 id = soModelModuleImpl::getNodeId(nodeName);
    return (!unkCC) ? id : unkCC[1][id];
}

u32 soModelModuleImplVariable::getCorrectNodeId(u32 p1) {
    return (p1 >= 400) ? p1 : soModelModuleImpl::getCorrectNodeId(p1);
}

const char* soModelModuleImplVariable::getNodeName(u32 p1) {
    return soModelModuleImpl::getNodeName(getRealNodeId(p1));
}

Vec3f soModelModuleImplVariable::getNodeGlobalPosition(u32 p1, bool p2) {
    return soModelModuleImpl::getNodeGlobalPosition(getRealNodeId(p1), p2);
}

Vec3f soModelModuleImplVariable::getNodeGlobalPosition(u32 p1, Vec3f* p2, u32 p3, bool p4) {
    return soModelModuleImpl::getNodeGlobalPosition(getRealNodeId(p1), p2, p3, p4);
}

Vec3f soModelModuleImplVariable::getNodeLocalPosition(u32 p1, bool p2) {
    return soModelModuleImpl::getNodeLocalPosition(getRealNodeId(p1), p2);
}

void soModelModuleImplVariable::getNodeLocalMtxFromNode(u32 p1, u32 p2, Matrix* p3, bool p4) {
    return soModelModuleImpl::getNodeLocalMtxFromNode(getRealNodeId(p1), getRealNodeId(p2), p3, p4);
}

void soModelModuleImplVariable::getNodeGlobalMtx(u32 p1, Matrix* p2, bool p3) {
    return soModelModuleImpl::getNodeGlobalMtx(getRealNodeId(p1), p2, p3);
}

void soModelModuleImplVariable::setNodeGlobalMtx(u32 p1, Matrix* p2, bool p3) {
    return soModelModuleImpl::setNodeGlobalMtx(getRealNodeId(p1), p2, p3);
}

Vec3f soModelModuleImplVariable::getNodeGlobalRotation(u32 p1, bool p2) {
    return soModelModuleImpl::getNodeGlobalRotation(getRealNodeId(p1), p2);
}

bool soModelModuleImplVariable::isNode(u32 p1) {
    return soModelModuleImpl::isNode(getRealNodeId(p1));
}

Vec3f soModelModuleImplVariable::getResFileNodeTranslate(u32 p1) {
    return soModelModuleImpl::getResFileNodeTranslate(getRealNodeId(p1));
}

Vec3f soModelModuleImplVariable::getResFileNodeRotation(u32 p1) {
    return soModelModuleImpl::getResFileNodeRotation(getRealNodeId(p1));
}

Vec3f soModelModuleImplVariable::getNodeGlobalOffsetFromTop(u32 p1) {
    return soModelModuleImpl::getNodeGlobalOffsetFromTop(getRealNodeId(p1));
}

Vec3f soModelModuleImplVariable::getTopNodeGlobalPosFromNode(u32 p1, Vec3f* p2) {
    return soModelModuleImpl::getTopNodeGlobalPosFromNode(getRealNodeId(p1), p2);
}

void soModelModuleImplVariable::renderNodeAxis(u32 p1, float f1) {
    return soModelModuleImpl::renderNodeAxis(getRealNodeId(p1), f1);
}

void soModelModuleImplVariable::setNodeVisibility(u32 p1, bool p2, bool p3) {
    u32 realId = getRealNodeId(p1);
    if (realId != 0xFFFFFFFF) {
        soModelModuleImpl::setNodeVisibility(realId, p2, p3);
    }
}

void soModelModuleImplVariable::setNodeVisibility(s32 p1, u32* p2, bool p3, bool p4) {
    if (getScnMdl(p4)) {
        for (s32 i = 0; i < p1; i++) {
            u32 realId = getRealNodeId(p2[i]);
            if (realId != 0xFFFFFFFF) {
                soModelModuleImpl::setNodeVisibility(realId, p3, p4);
            }
        }
    }
}

void soModelModuleImplVariable::setNodeConvertData(u32** convertData) {
    unkCC = convertData;
}
