#include "cameracontrolshelp.h"
#include "ui_cameracontrolshelp.h"

CameraControlsHelp::CameraControlsHelp(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CameraControlsHelp)
{
    ui->setupUi(this);
}

CameraControlsHelp::~CameraControlsHelp()
{
    delete ui;
}
