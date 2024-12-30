#include <em/em_info.h>
#include <em/enemy.h>
#include <em/em_resource_module_impl.h>
#include <types.h>
#include <so/resource/so_resource_module_impl.h>

emInfo g_emInfo;

const u32 emInfo::NodeNumTable[NumEnemies] = {
    0x21, 0x17, 0x34, 0x9, 0x25, 0xE, 0x10, 0x1A, 0x1C, 0x16,
    0x17, 0x16, 0x21, 0x13, 0x11, 0x27, 0xC, 0x21, 0x20, 0x20,
    0xC, 0x1F, 0x1C, 0x2C, 0x40, 0x22, 0x21, 0x1D, 0x2C, 0x25,
    0x1B, 0x2C, 0x2C, 0x2C, 0x2C, 0x2C, 0x18, 0x36, 0x36, 0x36,
    0x36, 0x17, 0x2B, 0x18, 0x18, 0x18, 0x1F, 0x13, 0x13, 0x13,
    0x13, 0x5C, 0x73, 0x4C, 0x41, 0x3D, 0x6A, 0x19, 0x92, 0x24,
    0x26,
};

const u32 emInfo::MotionNumTable[NumEnemies] = {
    0x10, 0x0C, 0x0E, 0x01, 0x0B, 0x08, 0x08, 0x0D, 0x14, 0x0B,
    0x10, 0x11, 0x0E, 0x07, 0x19, 0x16, 0x14, 0x0F, 0x17, 0x0C,
    0x17, 0x0C, 0x0C, 0x5D, 0x0C, 0x12, 0x11, 0x0C, 0x5D, 0x14,
    0x14, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x07, 0x1D, 0x1D, 0x1D,
    0x1D, 0x12, 0x20, 0x0A, 0x0A, 0x0A, 0x0C, 0x06, 0x06, 0x06,
    0x06, 0x20, 0x14, 0x15, 0x27, 0x1B, 0x12, 0x0B, 0x22, 0x28,
    0x25,
};

const u32 emInfo::StatusNumTable[NumEnemies] = {
    0x12, 0x10, 0x0E, 0x05, 0x10, 0x0E, 0x0D, 0x13, 0x15, 0x0F,
    0x12, 0x16, 0x11, 0x0E, 0x1D, 0x12, 0x18, 0x16, 0x19, 0x11,
    0x20, 0x0D, 0x13, 0x5E, 0x10, 0x1E, 0x18, 0x15, 0x5E, 0x19,
    0x1C, 0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x0D, 0x15, 0x14, 0x0E,
    0x14, 0x17, 0x21, 0x11, 0x11, 0x11, 0x11, 0x0A, 0x0A, 0x0A,
    0x0A, 0x30, 0x1E, 0x22, 0x23, 0x29, 0x1E, 0x0D, 0x2B, 0x39,
    0x37,
};

const u32 emInfo::KineticEnergyNumTable[NumEnemies] = {
    0x0B, 0x0F, 0x07, 0x08, 0x0A, 0x0A, 0x09, 0x0E, 0x0A, 0x05,
    0x0C, 0x11, 0x0B, 0x09, 0x16, 0x09, 0x1C, 0x11, 0x0B, 0x07,
    0x1D, 0x08, 0x0A, 0x36, 0x08, 0x16, 0x14, 0x0B, 0x36, 0x24,
    0x0A, 0x36, 0x36, 0x36, 0x36, 0x36, 0x03, 0x05, 0x06, 0x05,
    0x0E, 0x0E, 0x16, 0x09, 0x09, 0x09, 0x04, 0x06, 0x06, 0x06,
    0x06, 0x0F, 0x13, 0x0C, 0x0B, 0x15, 0x15, 0x0C, 0x0B, 0x2C,
    0x26,
};

const u32 emInfo::KineticEnergyParamAryPtrGetIdTable[NumEnemies] = {
    0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0,
    0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0,
    0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0,
    0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0,
    0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0,
    0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0, 0xABE0,
    0xABE0,
};

const u32 emInfo::AreaNumTable[NumEnemies] = {
    0x09, 0x09, 0x0A, 0x07, 0x09, 0x08, 0x0A, 0x0D, 0x0A, 0x0B,
    0x07, 0x09, 0x0F, 0x09, 0x09, 0x0A, 0x0D, 0x09, 0x0B, 0x08,
    0x08, 0x0D, 0x09, 0x11, 0x08, 0x0B, 0x0B, 0x09, 0x11, 0x09,
    0x09, 0x11, 0x11, 0x11, 0x11, 0x11, 0x07, 0x08, 0x0C, 0x08,
    0x0B, 0x0C, 0x0F, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07, 0x07,
    0x07, 0x07, 0x0B, 0x07, 0x0A, 0x09, 0x08, 0x07, 0x07, 0x08,
    0x07,
};

const u32 emInfo::AreaParamAryPtrGetIdTable[NumEnemies] = {
    0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1,
    0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1,
    0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1,
    0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1,
    0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1,
    0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1, 0xABE1,
    0xABE1,
};

const u32 emInfo::TargetSearchNumTable[NumEnemies] = {
    0x02, 0x03, 0x03, 0x01, 0x02, 0x01, 0x02, 0x04, 0x03, 0x04,
    0x01, 0x03, 0x06, 0x02, 0x03, 0x03, 0x07, 0x03, 0x05, 0x02,
    0x02, 0x03, 0x02, 0x0C, 0x02, 0x03, 0x04, 0x03, 0x0C, 0x03,
    0x02, 0x0C, 0x0C, 0x0C, 0x0C, 0x0C, 0x01, 0x01, 0x05, 0x01,
    0x04, 0x05, 0x09, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
    0x01, 0x01, 0x02, 0x01, 0x03, 0x03, 0x02, 0x01, 0x01, 0x02,
    0x01,
};

const u32 emInfo::TargetSearchParamAryPtrGetIdTable[NumEnemies] = {
    0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2,
    0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2,
    0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2,
    0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2,
    0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2,
    0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2, 0xABE2,
    0xABE2,
};

const u32 emInfo::AINumTable[NumEnemies] = {
    0x1D, 0x1A, 0x12, 0x03, 0x10, 0x0D, 0x0F, 0x1C, 0x22, 0x14,
    0x18, 0x1F, 0x1A, 0x0D, 0x27, 0x18, 0x22, 0x1C, 0x28, 0x1D,
    0x25, 0x10, 0x12, 0x5C, 0x1B, 0x21, 0x26, 0x19, 0x5C, 0x1E,
    0x17, 0x5C, 0x5C, 0x5C, 0x5C, 0x5C, 0x0F, 0x17, 0x19, 0x12,
    0x29, 0x20, 0x33, 0x15, 0x15, 0x15, 0x1F, 0x0E, 0x0E, 0x0E,
    0x0E, 0x18, 0x12, 0x12, 0x18, 0x21, 0x16, 0x05, 0x17, 0x18,
    0x18,
};

const u32 emInfo::SEIdTable[NumEnemies] = {
    0x0116, 0x011C, 0x0128, 0x0126, 0x0124, 0x0131, 0x012A, 0x0135, 0x011A, 0x0125,
    0x0121, 0x0120, 0x0134, 0x0119, 0x011E, 0x012B, 0x012E, 0x0132, 0x0122, 0x0118,
    0x011D, 0x0123, 0x012C, 0x0137, 0x012F, 0x0117, 0x012D, 0x011F, 0x0137, 0x0130,
    0x0129, 0x0137, 0x0137, 0x0137, 0x0137, 0x0137, 0x0127, 0x0138, 0x0138, 0x0138,
    0x0138, 0x0133, 0x0136, 0x011B, 0x011B, 0x011B, 0x0139, 0x0127, 0x0127, 0x0127,
    0x0127, 0x013C, 0x013D, 0x013A, 0x013E, 0x013B, 0x013F, 0x0143, 0x0140, 0x0141,
    0x0142,
};

const u32 emInfo::StandByStatusKindTable[NumEnemies] = {
    0x11, 0x0F, 0x0D, 0x04, 0x0F, 0x0D, 0x0C, 0x12, 0x14, 0x0E,
    0x11, 0x15, 0x10, 0x0D, 0x1C, 0x11, 0x17, 0x15, 0x18, 0x10,
    0x1F, 0x0C, 0x12, 0x5D, 0x0F, 0x1D, 0x17, 0x14, 0x5D, 0x18,
    0x1B, 0x5D, 0x5D, 0x5D, 0x5D, 0x5D, 0x0C, 0x14, 0x13, 0x0D,
    0x13, 0x16, 0x20, 0x10, 0x10, 0x10, 0x10, 0x09, 0x09, 0x09,
    0x09, 0x2F, 0x1D, 0x21, 0x22, 0x28, 0x1D, 0x0C, 0x2A, 0x38,
    0x36,
};

emInfo::CreateEnemyFunc_t emInfo::CreateFuncPtrTable[NumEnemies] = {
    &Enemy::createInstance<emKuribo>,
    &Enemy::createInstance<emPopperam>,
    &Enemy::createInstance<emPacci>,
    &Enemy::createInstance<emJyakeel>,
    &Enemy::createInstance<emAroaros>,
    &Enemy::createInstance<emCymal>,
    &Enemy::createInstance<emDeathpod>,
    &Enemy::createInstance<emBoobas>,
    &Enemy::createInstance<emDekakuribo>,
    &Enemy::createInstance<emBotron>,
    &Enemy::createInstance<emBucyulus>,
    &Enemy::createInstance<emGyraan>,
    &Enemy::createInstance<emSiralamos>,
    &Enemy::createInstance<emKiller>,
    &Enemy::createInstance<emRoada>,
    &Enemy::createInstance<emSpar>,
    &Enemy::createInstance<emMite>,
    &Enemy::createInstance<emTeckin>,
    &Enemy::createInstance<emTautau>,
    &Enemy::createInstance<emHammerbros>,
    &Enemy::createInstance<emBitan>,
    &Enemy::createInstance<emFlows>,
    &Enemy::createInstance<emKokkon>,
    &Enemy::createInstance<emPrim>,
    &Enemy::createInstance<emShelly>,
    &Enemy::createInstance<emPatapata>,
    &Enemy::createInstance<emJdus>,
    &Enemy::createInstance<emBombhead>,
    &Enemy::createInstance<emPrim>,
    &Enemy::createInstance<emNgagog>,
    &Enemy::createInstance<emFaulong>,
    &Enemy::createInstance<emPrim>,
    &Enemy::createInstance<emPrim>,
    &Enemy::createInstance<emPrim>,
    &Enemy::createInstance<emPrim>,
    &Enemy::createInstance<emPrim>,
    &Enemy::createInstance<emGhamgha>,
    &Enemy::createInstance<emRobobeam>,
    &Enemy::createInstance<emRobodistance>,
    &Enemy::createInstance<emRobohoming>,
    &Enemy::createInstance<emRobopunch>,
    &Enemy::createInstance<emCataguard>,
    &Enemy::createInstance<emArman>,
    &Enemy::createInstance<emGalfire>,
    &Enemy::createInstance<emGalfire>,
    &Enemy::createInstance<emGalfire>,
    &Enemy::createInstance<emBosspackun>,
    &Enemy::createInstance<emGhamghabase>,
    &Enemy::createInstance<emGhamghabase>,
    &Enemy::createInstance<emGhamghabase>,
    &Enemy::createInstance<emGhamghabase>,
    &Enemy::createInstance<emGalleom>,
    &Enemy::createInstance<emRidley>,
    &Enemy::createInstance<emRayquaza>,
    &Enemy::createInstance<emDuon>,
    &Enemy::createInstance<emPorky>,
    &Enemy::createInstance<emMetaridley>,
    &Enemy::createInstance<emFalconflyer>,
    &Enemy::createInstance<emTaboo>,
    &Enemy::createInstance<emMasterhand>,
    &Enemy::createInstance<emCrazyhand>
};

emInfo::ModifyExParamFunc_t emInfo::ModifyExParamFuncPtrTable[NumEnemies] = {
    &emResourceModuleImpl::ModifyExParam<emKuriboParam>,
    &emResourceModuleImpl::ModifyExParam<emPopperamParam>,
    &emResourceModuleImpl::ModifyExParam<emPacciParam>,
    &emResourceModuleImpl::ModifyExParam<emJyakeelParam>,
    &emResourceModuleImpl::ModifyExParam<emAroarosParam>,
    &emResourceModuleImpl::ModifyExParam<emCymalParam>,
    &emResourceModuleImpl::ModifyExParam<emDeathpodParam>,
    &emResourceModuleImpl::ModifyExParam<emBoobasParam>,
    &emResourceModuleImpl::ModifyExParam<emDekakuriboParam>,
    &emResourceModuleImpl::ModifyExParam<emBotronParam>,
    &emResourceModuleImpl::ModifyExParam<emBucyulusParam>,
    &emResourceModuleImpl::ModifyExParam<emGyraanParam>,
    &emResourceModuleImpl::ModifyExParam<emSiralamosParam>,
    &emResourceModuleImpl::ModifyExParam<emKillerParam>,
    &emResourceModuleImpl::ModifyExParam<emRoadaParam>,
    &emResourceModuleImpl::ModifyExParam<emSparParam>,
    &emResourceModuleImpl::ModifyExParam<emMiteParam>,
    &emResourceModuleImpl::ModifyExParam<emTeckinParam>,
    &emResourceModuleImpl::ModifyExParam<emTautauParam>,
    &emResourceModuleImpl::ModifyExParam<emHammerbrosParam>,
    &emResourceModuleImpl::ModifyExParam<emBitanParam>,
    &emResourceModuleImpl::ModifyExParam<emFlowsParam>,
    &emResourceModuleImpl::ModifyExParam<emKokkonParam>,
    &emResourceModuleImpl::ModifyExParam<emPrimParam>,
    &emResourceModuleImpl::ModifyExParam<emShellyParam>,
    &emResourceModuleImpl::ModifyExParam<emPatapataParam>,
    &emResourceModuleImpl::ModifyExParam<emJdusParam>,
    &emResourceModuleImpl::ModifyExParam<emBombheadParam>,
    &emResourceModuleImpl::ModifyExParam<emPrimParam>,
    &emResourceModuleImpl::ModifyExParam<emNgagogParam>,
    &emResourceModuleImpl::ModifyExParam<emFaulongParam>,
    &emResourceModuleImpl::ModifyExParam<emPrimParam>,
    &emResourceModuleImpl::ModifyExParam<emPrimParam>,
    &emResourceModuleImpl::ModifyExParam<emPrimParam>,
    &emResourceModuleImpl::ModifyExParam<emPrimParam>,
    &emResourceModuleImpl::ModifyExParam<emPrimParam>,
    &emResourceModuleImpl::ModifyExParam<emGhamghaParam>,
    &emResourceModuleImpl::ModifyExParam<emRobobeamParam>,
    &emResourceModuleImpl::ModifyExParam<emRobodistanceParam>,
    &emResourceModuleImpl::ModifyExParam<emRobohomingParam>,
    &emResourceModuleImpl::ModifyExParam<emRobopunchParam>,
    &emResourceModuleImpl::ModifyExParam<emCataguardParam>,
    &emResourceModuleImpl::ModifyExParam<emArmanParam>,
    &emResourceModuleImpl::ModifyExParam<emGalfireParam>,
    &emResourceModuleImpl::ModifyExParam<emGalfireParam>,
    &emResourceModuleImpl::ModifyExParam<emGalfireParam>,
    &emResourceModuleImpl::ModifyExParam<emBosspackunParam>,
    &emResourceModuleImpl::ModifyExParam<emGhamghabaseParam>,
    &emResourceModuleImpl::ModifyExParam<emGhamghabaseParam>,
    &emResourceModuleImpl::ModifyExParam<emGhamghabaseParam>,
    &emResourceModuleImpl::ModifyExParam<emGhamghabaseParam>,
    &emResourceModuleImpl::ModifyExParam<emGalleomParam>,
    &emResourceModuleImpl::ModifyExParam<emRidleyParam>,
    &emResourceModuleImpl::ModifyExParam<emRayquazaParam>,
    &emResourceModuleImpl::ModifyExParam<emDuonParam>,
    &emResourceModuleImpl::ModifyExParam<emPorkyParam>,
    &emResourceModuleImpl::ModifyExParam<emMetaridleyParam>,
    &emResourceModuleImpl::ModifyExParam<emFalconflyerParam>,
    &emResourceModuleImpl::ModifyExParam<emTabooParam>,
    &emResourceModuleImpl::ModifyExParam<emMasterhandParam>,
    &emResourceModuleImpl::ModifyExParam<emCrazyhandParam>
};

emInfo::DeleteExParamFunc_t emInfo::DeleteExParamFuncPtrTable[NumEnemies] = {
    &emResourceModuleImpl::DeleteExParam<emKuriboParam>,
    &emResourceModuleImpl::DeleteExParam<emPopperamParam>,
    &emResourceModuleImpl::DeleteExParam<emPacciParam>,
    &emResourceModuleImpl::DeleteExParam<emJyakeelParam>,
    &emResourceModuleImpl::DeleteExParam<emAroarosParam>,
    &emResourceModuleImpl::DeleteExParam<emCymalParam>,
    &emResourceModuleImpl::DeleteExParam<emDeathpodParam>,
    &emResourceModuleImpl::DeleteExParam<emBoobasParam>,
    &emResourceModuleImpl::DeleteExParam<emDekakuriboParam>,
    &emResourceModuleImpl::DeleteExParam<emBotronParam>,
    &emResourceModuleImpl::DeleteExParam<emBucyulusParam>,
    &emResourceModuleImpl::DeleteExParam<emGyraanParam>,
    &emResourceModuleImpl::DeleteExParam<emSiralamosParam>,
    &emResourceModuleImpl::DeleteExParam<emKillerParam>,
    &emResourceModuleImpl::DeleteExParam<emRoadaParam>,
    &emResourceModuleImpl::DeleteExParam<emSparParam>,
    &emResourceModuleImpl::DeleteExParam<emMiteParam>,
    &emResourceModuleImpl::DeleteExParam<emTeckinParam>,
    &emResourceModuleImpl::DeleteExParam<emTautauParam>,
    &emResourceModuleImpl::DeleteExParam<emHammerbrosParam>,
    &emResourceModuleImpl::DeleteExParam<emBitanParam>,
    &emResourceModuleImpl::DeleteExParam<emFlowsParam>,
    &emResourceModuleImpl::DeleteExParam<emKokkonParam>,
    &emResourceModuleImpl::DeleteExParam<emPrimParam>,
    &emResourceModuleImpl::DeleteExParam<emShellyParam>,
    &emResourceModuleImpl::DeleteExParam<emPatapataParam>,
    &emResourceModuleImpl::DeleteExParam<emJdusParam>,
    &emResourceModuleImpl::DeleteExParam<emBombheadParam>,
    &emResourceModuleImpl::DeleteExParam<emPrimParam>,
    &emResourceModuleImpl::DeleteExParam<emNgagogParam>,
    &emResourceModuleImpl::DeleteExParam<emFaulongParam>,
    &emResourceModuleImpl::DeleteExParam<emPrimParam>,
    &emResourceModuleImpl::DeleteExParam<emPrimParam>,
    &emResourceModuleImpl::DeleteExParam<emPrimParam>,
    &emResourceModuleImpl::DeleteExParam<emPrimParam>,
    &emResourceModuleImpl::DeleteExParam<emPrimParam>,
    &emResourceModuleImpl::DeleteExParam<emGhamghaParam>,
    &emResourceModuleImpl::DeleteExParam<emRobobeamParam>,
    &emResourceModuleImpl::DeleteExParam<emRobodistanceParam>,
    &emResourceModuleImpl::DeleteExParam<emRobohomingParam>,
    &emResourceModuleImpl::DeleteExParam<emRobopunchParam>,
    &emResourceModuleImpl::DeleteExParam<emCataguardParam>,
    &emResourceModuleImpl::DeleteExParam<emArmanParam>,
    &emResourceModuleImpl::DeleteExParam<emGalfireParam>,
    &emResourceModuleImpl::DeleteExParam<emGalfireParam>,
    &emResourceModuleImpl::DeleteExParam<emGalfireParam>,
    &emResourceModuleImpl::DeleteExParam<emBosspackunParam>,
    &emResourceModuleImpl::DeleteExParam<emGhamghabaseParam>,
    &emResourceModuleImpl::DeleteExParam<emGhamghabaseParam>,
    &emResourceModuleImpl::DeleteExParam<emGhamghabaseParam>,
    &emResourceModuleImpl::DeleteExParam<emGhamghabaseParam>,
    &emResourceModuleImpl::DeleteExParam<emGalleomParam>,
    &emResourceModuleImpl::DeleteExParam<emRidleyParam>,
    &emResourceModuleImpl::DeleteExParam<emRayquazaParam>,
    &emResourceModuleImpl::DeleteExParam<emDuonParam>,
    &emResourceModuleImpl::DeleteExParam<emPorkyParam>,
    &emResourceModuleImpl::DeleteExParam<emMetaridleyParam>,
    &emResourceModuleImpl::DeleteExParam<emFalconflyerParam>,
    &emResourceModuleImpl::DeleteExParam<emTabooParam>,
    &emResourceModuleImpl::DeleteExParam<emMasterhandParam>,
    &emResourceModuleImpl::DeleteExParam<emCrazyhandParam>
};

emInfo* emInfo::getInstance() {
    return &g_emInfo;
}

emInfo::emInfo() { }

emInfo::~emInfo() { }

u32 emInfo::isInvalidKind(s32 index) {
    if (index <= -1 || index >= NumEnemies) {
        return 1;
    }
    return 0;
}

u32 emInfo::isPrimKind(s32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    if (index == 23 || index == 28 || (index - 31) <= 4U) {
        return 1;
    }
    return 0;
}

s32 emInfo::getInvalidKind() {
    return -1;
}

u32 emInfo::getNodeNum(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return NodeNumTable[index];
}

u32 emInfo::getMotionNum(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return MotionNumTable[index];
}

u32 emInfo::getStatusNum(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return StatusNumTable[index];
}

u32 emInfo::getKineticEnergyNum(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return KineticEnergyNumTable[index];
}

u32 emInfo::getKineticEnergyParamAryPtrGetId(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return KineticEnergyParamAryPtrGetIdTable[index];
}

u32 emInfo::getAreaNum(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return AreaNumTable[index];
}

u32 emInfo::getAreaParamAryPtrGetId(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return AreaParamAryPtrGetIdTable[index];
}

u32 emInfo::getTargetSearchNum(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return TargetSearchNumTable[index];
}

u32 emInfo::getTargetSearchParamAryPtrGetId(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return TargetSearchParamAryPtrGetIdTable[index];
}

u32 emInfo::getAINum(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return AINumTable[index];
}

emInfo::CreateEnemyFunc_t emInfo::getCreateFuncPtr(s32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return CreateFuncPtrTable[index];
}

emInfo::ModifyExParamFunc_t emInfo::getExParamModifyFuncPtr(s32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return ModifyExParamFuncPtrTable[index];
}

emInfo::DeleteExParamFunc_t emInfo::getExParamDeleteFuncPtr(s32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return DeleteExParamFuncPtrTable[index];
}

u32 emInfo::getSEId(u32 index) {
    if (isInvalidKind(index) == 1) {
        return 0;
    }
    return SEIdTable[index];
}

u32 emInfo::StandByStatusKind(u32 index) {
    if (isInvalidKind(index) == 1) {
        return -1;
    }
    return StandByStatusKindTable[index];
}

const char* emInfo::getNamePtr() {
    return 0;
}
