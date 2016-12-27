#ifndef FACESWIDGET_H
#define FACESWIDGET_H

#include <QWidget>

namespace Ui {
class FacesWidget;
}

class FacesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit FacesWidget(QWidget *parent = 0);
    ~FacesWidget();

private:
    Ui::FacesWidget *ui;
};

#endif // FACESWIDGET_H
