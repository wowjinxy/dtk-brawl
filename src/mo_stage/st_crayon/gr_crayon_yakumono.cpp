#include <gr/gr_gimmick.h>
#include <gr/gr_yakumono.h>
#include <st_crayon/gr_crayon.h>

grCrayonYakumono::grCrayonYakumono(const char* taskName): grYakumono(taskName), unk158(0) { }

grCrayonYakumono::~grCrayonYakumono() { }

void grCrayonYakumono::update(float deltaFrame) {
    unk150 = deltaFrame;
    grGimmick::update(deltaFrame);
}
