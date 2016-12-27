#include "verticeswidget.h"
#include "ui_verticeswidget.h"

VerticesWidget::VerticesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::VerticesWidget)
{
    ui->setupUi(this);
}

VerticesWidget::~VerticesWidget()
{
    delete ui;
}
