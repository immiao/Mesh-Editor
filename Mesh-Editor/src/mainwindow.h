#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "skeleton.h"
#include <QMainWindow>
#include "QProgressDialog"
#include "QTimer"
#include "QMessageBox"
#include <QJsonDocument>
#include <iostream>
#include "QTreeWidget"
#include "QTreeWidgetItem"
#include "jointtreewidgetitem.h"
namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer timer;
    QMessageBox *message;
    QMessageBox *message2;
    JointTreeWidgetItem* ConstructTreeWidget(Joint*);

private slots:
    void on_actionQuit_triggered();

    void on_actionCamera_Controls_triggered();

    void on_actionOpen_obj_file_triggered();

    void on_actionOpen_json_file_triggered();

    void modifyVertexPos();
    void modifyFaceColor();
    void addVertex();
    void triangulateFace();
    void subdivision();
    void timerUpdate();
    void extrude();
    void setEdgeSharp();
    void setVertexSharp();
    void setFaceSharp();
    void setSharpness();
    void modifyJointPos();
    void modifyJointDegreePlus();
    void modifyJointDegreeMinus();
    void HighLightTreeWidgetItem(QTreeWidgetItem*, QTreeWidgetItem*);
    void skinning();

private:
    Ui::MainWindow *ui;
};


#endif // MAINWINDOW_H
