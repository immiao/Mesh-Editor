#ifndef CAMERACONTROLSHELP_H
#define CAMERACONTROLSHELP_H

#include <QWidget>

namespace Ui {
class CameraControlsHelp;
}

class CameraControlsHelp : public QWidget
{
    Q_OBJECT

public:
    explicit CameraControlsHelp(QWidget *parent = 0);
    ~CameraControlsHelp();

private:
    Ui::CameraControlsHelp *ui;
};

#endif // CAMERACONTROLSHELP_H
