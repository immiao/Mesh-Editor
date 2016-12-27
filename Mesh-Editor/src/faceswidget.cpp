#include "faceswidget.h"
#include "ui_faceswidget.h"

FacesWidget::FacesWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FacesWidget)
{
    ui->setupUi(this);
}

FacesWidget::~FacesWidget()
{
    delete ui;
}
