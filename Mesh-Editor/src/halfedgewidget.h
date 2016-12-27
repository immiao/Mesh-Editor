#ifndef HALFEDGEWIDGET_H
#define HALFEDGEWIDGET_H

#include <QWidget>

namespace Ui {
class HalfEdgeWidget;
}

class HalfEdgeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit HalfEdgeWidget(QWidget *parent = 0);
    ~HalfEdgeWidget();

private:
    Ui::HalfEdgeWidget *ui;
};

#endif // HALFEDGEWIDGET_H
