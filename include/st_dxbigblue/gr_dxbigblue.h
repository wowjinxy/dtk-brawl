#pragma once

#include <StaticAssert.h>
#include <gr/gr_yakumono.h>
#include <types.h>

class grDxBigBlue : public grYakumono {
protected:
    u8 unk150;
    float unk154;
public:
    grDxBigBlue(const char* taskName);
    virtual ~grDxBigBlue();
};
static_assert(sizeof(grDxBigBlue) == 0x158, "Class is the wrong size!");
