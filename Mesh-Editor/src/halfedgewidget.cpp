#include "halfedgewidget.h"
#include "ui_halfedgewidget.h"

HalfEdgeWidget::HalfEdgeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::HalfEdgeWidget)
{
    ui->setupUi(this);
}

HalfEdgeWidget::~HalfEdgeWidget()
{
    delete ui;
}
