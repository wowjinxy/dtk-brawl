#include <cm/cm_camera_controller.h>
#include <cm/cm_controller_photo.h>
#include <gf/gf_camera_controller.h>
#include <so/so_photo_call_back.h>
#include <ut/ut_list.h>

void soPhotoCallBack::addCallback() {
    gfCameraController* cm = !CameraController::getInstance() ? 0 :
        CameraController::getInstance()->getCameraController(CameraController::Photo);
    if (cm) {
        static_cast<cmPhotoController*>(cm)->addCallBack(this);
    }
}

void soPhotoCallBack::removeCallBack() {
    gfCameraController* cm = !CameraController::getInstance() ? 0 :
        CameraController::getInstance()->getCameraController(CameraController::Photo);
    if (cm) {
        static_cast<cmPhotoController*>(cm)->removeCallBack(this);
    }
}

bool soPhotoController::setFreezeMode() {
    gfCameraController* cm = CameraController::getInstance()->getCameraController(CameraController::Photo);
    if (!cm) {
        return false;
    }
    static_cast<cmPhotoController*>(cm)->setFreezeMode(this);
    return true;
}
