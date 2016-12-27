#ifndef LISTWIDGET_H
#define LISTWIDGET_H

#include <QListWidget>
#include <QSpinBox>
#include "mygl.h"

class ListWidget : public QListWidget
{
    Q_OBJECT
public:
    ListWidget(QWidget *parent);
    MyGL *pMyGL;
    void SetupConnection();
    QDoubleSpinBox *pBoxX;
    QDoubleSpinBox *pBoxY;
    QDoubleSpinBox *pBoxZ;
    QDoubleSpinBox *pBoxR;
    QDoubleSpinBox *pBoxG;
    QDoubleSpinBox *pBoxB;
    //QListWidgetItem *crtItem;
public slots:
    void NotifyMyGL(QListWidgetItem *item);
};

#endif // LISTWIDGET_H
