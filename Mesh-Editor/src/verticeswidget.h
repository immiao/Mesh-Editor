#ifndef VERTICESWIDGET_H
#define VERTICESWIDGET_H

#include <QWidget>

namespace Ui {
class VerticesWidget;
}

class VerticesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit VerticesWidget(QWidget *parent = 0);
    ~VerticesWidget();

private:
    Ui::VerticesWidget *ui;
};

#endif // VERTICESWIDGET_H
