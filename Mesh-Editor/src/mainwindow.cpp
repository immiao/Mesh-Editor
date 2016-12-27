#include "mainwindow.h"
#include <ui_mainwindow.h>
#include "cameracontrolshelp.h"
#include "QFileDialog"
#include "QDir"
#include "listwidgetitem.h"
#include "QMessageBox"

void MainWindow::skinning()
{
    if (!ui->mygl->skeleton.root) return;

    ui->mygl->ObjMesh.skinning(ui->mygl->skeleton);
    ui->mygl->isSkin = true;
    message2->show();
}

void MainWindow::HighLightTreeWidgetItem(QTreeWidgetItem* crtItem, QTreeWidgetItem* preItem)
{
    JointTreeWidgetItem* preJointItem = dynamic_cast<JointTreeWidgetItem*>(preItem);
    JointTreeWidgetItem* crtJointItem = dynamic_cast<JointTreeWidgetItem*>(crtItem);

    if (preJointItem)
    {
        int id = preJointItem->pJoint->drawJointId;
        DrawJoint *p = ui->mygl->skeleton.vDrawJoint[id];
        //printf("pre:%d\n", id);
        p->Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        p->update();
    }

    if (crtJointItem)
    {
        int id = crtJointItem->pJoint->drawJointId;
        //printf("crt:%d\n", id);
        DrawJoint *p = ui->mygl->skeleton.vDrawJoint[id];
        p->Color = glm::vec4(0.0f, 0.0f, 1.0f, 1.0f);
        p->update();
        glm::vec3 localPos = p->joint->pos;
        glm::vec3 localEulerAngle = glm::eulerAngles(p->joint->rot);
        ui->doubleSpinBox_5->setValue(localPos.x);
        ui->doubleSpinBox_6->setValue(localPos.y);
        ui->doubleSpinBox_7->setValue(localPos.z);
    }
}

JointTreeWidgetItem* MainWindow::ConstructTreeWidget(Joint* root)
{
    if (root == NULL)
    {
        printf("ConstructTreeWidget() ERROR: EMPTY JOINT\n");
        return NULL;
    }

    JointTreeWidgetItem* item = NULL;
    QStringList name;
    name << root->name;
    item = new JointTreeWidgetItem(name, root);
    int size = root->children.size();
    for (int i = 0; i < size; i++)
    {
        JointTreeWidgetItem* childItem = ConstructTreeWidget(root->children[i]);
        item->addChild(childItem);
    }
    return item;
}

void MainWindow::modifyJointDegreePlus()
{
    JointTreeWidgetItem* crtJointItem = dynamic_cast<JointTreeWidgetItem*>(ui->treeWidget->currentItem());
    if (!crtJointItem) return;
    int id = crtJointItem->pJoint->drawJointId;
    //printf("crt:%d\n", id);
    DrawJoint *p = ui->mygl->skeleton.vDrawJoint[id];

    glm::vec3 axis[3] = {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)};
    int idx = ui->comboBox->currentIndex();
    p->joint->rot = glm::rotate(p->joint->rot, 5.0f, axis[idx]);
    ui->mygl->skeleton.update();
    //p->update();
}

void MainWindow::modifyJointDegreeMinus()
{
    JointTreeWidgetItem* crtJointItem = dynamic_cast<JointTreeWidgetItem*>(ui->treeWidget->currentItem());
    if (!crtJointItem) return;
    int id = crtJointItem->pJoint->drawJointId;
    //printf("crt:%d\n", id);
    DrawJoint *p = ui->mygl->skeleton.vDrawJoint[id];
    glm::vec3 axis[3] = {glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)};
    int idx = ui->comboBox->currentIndex();
    p->joint->rot = glm::rotate(p->joint->rot, -5.0f, axis[idx]);
    ui->mygl->skeleton.update();
    //p->update();
}

//void MainWindow::modifyJointDegreeY()
//{
//    JointTreeWidgetItem* crtJointItem = dynamic_cast<JointTreeWidgetItem*>(ui->treeWidget->currentItem());
//    if (!crtJointItem) return;
//    int id = crtJointItem->pJoint->drawJointId;
//    //printf("crt:%d\n", id);
//    DrawJoint *p = ui->mygl->skeleton.vDrawJoint[id];
//    p->joint->rot = glm::rotate(p->joint->rot, ui->doubleSpinBox_3->singleStep(), glm::vec3(0.0f, 1.0f, 0.0f));
//    p->update();
//}

//void MainWindow::modifyJointDegreeZ()
//{
//    JointTreeWidgetItem* crtJointItem = dynamic_cast<JointTreeWidgetItem*>(ui->treeWidget->currentItem());
//    if (!crtJointItem) return;
//    int id = crtJointItem->pJoint->drawJointId;
//    //printf("crt:%d\n", id);
//    DrawJoint *p = ui->mygl->skeleton.vDrawJoint[id];
//    p->joint->rot = glm::rotate(p->joint->rot, ui->doubleSpinBox_4->singleStep(), glm::vec3(0.0f, 0.0f, 1.0f));
//    p->update();
//}
void MainWindow::modifyJointPos()
{
    JointTreeWidgetItem* crtJointItem = dynamic_cast<JointTreeWidgetItem*>(ui->treeWidget->currentItem());
    if (!crtJointItem) return;

    int id = crtJointItem->pJoint->drawJointId;
    //printf("crt:%d\n", id);
    DrawJoint *p = ui->mygl->skeleton.vDrawJoint[id];
    glm::vec3 newPos = glm::vec3(ui->doubleSpinBox_5->value(), ui->doubleSpinBox_6->value(), ui->doubleSpinBox_7->value());
    p->joint->pos = newPos;
    ui->mygl->skeleton.update();
    //p->update();
}

void MainWindow::on_actionOpen_json_file_triggered()
{
    ui->mygl->isDrawSkeleton = false;
    Skeleton &skeleton = ui->mygl->skeleton;

    ui->treeWidget->clear();
    skeleton.clear();

    QString filename = QFileDialog::getOpenFileName(0, QString("Load Scene File"), QDir::currentPath().append(QString("../..")), QString("*.json"));
    if (filename.isEmpty())
        return;
    qDebug() << filename;
    //qWarning() << root;
    //qDebug() << (root["children"].toArray())[0].toObject();
    //qDebug() << child;
    //qWarning() << root["children"].toObject();


    ui->mygl->isDrawSkeleton = true;

    skeleton.construct(filename);
    JointTreeWidgetItem *topItem = ConstructTreeWidget(skeleton.root);
    ui->treeWidget->addTopLevelItem(topItem);
    //skeleton.test(skeleton.root);
   // QJsonDocument document = QJsonDocument::FromJson();
}

void MainWindow::modifyVertexPos()
{
    if (!ui->verticesListWidget->currentItem())
    {

        message->show();
        return;
    }
    ListWidgetItem* pItem = dynamic_cast<ListWidgetItem*>(ui->verticesListWidget->currentItem());
    if (pItem != NULL)
    {
        int posBufferIdx;
        int size;
        {
            size = ui->mygl->ObjMesh.mesh_vertBufferIdx[pItem->id].size();
            printf("IN\n");
            for (int i = 0; i < size; i++)
            {
                posBufferIdx = ui->mygl->ObjMesh.mesh_vertBufferIdx[pItem->id][i];
                ui->mygl->ObjMesh.mesh_pos[posBufferIdx].p[0] = ui->doubleSpinBoxX->value();
                ui->mygl->ObjMesh.mesh_pos[posBufferIdx].p[1] = ui->doubleSpinBoxY->value();
                ui->mygl->ObjMesh.mesh_pos[posBufferIdx].p[2] = ui->doubleSpinBoxZ->value();
                ui->mygl->ObjMesh.mesh_vertices[pItem->id].pos[0] = ui->doubleSpinBoxX->value();
                ui->mygl->ObjMesh.mesh_vertices[pItem->id].pos[1] = ui->doubleSpinBoxY->value();
                ui->mygl->ObjMesh.mesh_vertices[pItem->id].pos[2] = ui->doubleSpinBoxZ->value();
                ui->mygl->ObjMesh.mesh_objPos[pItem->id].p[0] = ui->doubleSpinBoxX->value();
                ui->mygl->ObjMesh.mesh_objPos[pItem->id].p[1] = ui->doubleSpinBoxY->value();
                ui->mygl->ObjMesh.mesh_objPos[pItem->id].p[2] = ui->doubleSpinBoxZ->value();
            }
            //ui->mygl->ObjMesh.UpdateBuffer();
        }

        ui->verticesListWidget->NotifyMyGL(ui->verticesListWidget->currentItem());
        ui->facesListWidget->NotifyMyGL(ui->facesListWidget->currentItem());
        ui->halfEdgeListWidget->NotifyMyGL(ui->halfEdgeListWidget->currentItem());

        ui->mygl->ObjMesh.PlanarityTest();
        //printf("update pos\n");
    }
}

void MainWindow::modifyFaceColor()
{
    if (!ui->facesListWidget->currentItem())
    {
        message->show();
        return;
    }
    ListWidgetItem* pItem = dynamic_cast<ListWidgetItem*>(ui->facesListWidget->currentItem());
    if (pItem != NULL)
    {
        //printf("IMM\n");
        //ui->mygl->setFocus();
        int posBufferIdx;
        {
            ui->mygl->ObjMesh.mesh_faces[pItem->id].color.p[0] = ui->doubleSpinBoxR->value();
            ui->mygl->ObjMesh.mesh_faces[pItem->id].color.p[1] = ui->doubleSpinBoxG->value();
            ui->mygl->ObjMesh.mesh_faces[pItem->id].color.p[2] = ui->doubleSpinBoxB->value();

            int vertSize = ui->mygl->ObjMesh.mesh_faceBufferIdx[pItem->id].size();
            for (int i = 0; i < vertSize; i++)
            {
                posBufferIdx = ui->mygl->ObjMesh.mesh_faceBufferIdx[pItem->id][i];
                ui->mygl->ObjMesh.mesh_col[posBufferIdx].p[0] = ui->doubleSpinBoxR->value();
                ui->mygl->ObjMesh.mesh_col[posBufferIdx].p[1] = ui->doubleSpinBoxG->value();
                ui->mygl->ObjMesh.mesh_col[posBufferIdx].p[2] = ui->doubleSpinBoxB->value();
            }
//            HalfEdge *currentEdge = ui->mygl->ObjMesh.mesh_faces[pItem->id].start_edge;
//            do
//            {
//                //printf("vert:%d\n", currentEdge->vert->id);

//                size = ui->mygl->ObjMesh.mesh_vertBufferIdx[currentEdge->vert->id].size();
//                for (int i = 0; i < size; i++)
//                {
//                    posBufferIdx = ui->mygl->ObjMesh.mesh_vertBufferIdx[currentEdge->vert->id][i];
//                    ui->mygl->ObjMesh.mesh_col[posBufferIdx].p[0] = ui->doubleSpinBoxR->value();
//                    ui->mygl->ObjMesh.mesh_col[posBufferIdx].p[1] = ui->doubleSpinBoxG->value();
//                    ui->mygl->ObjMesh.mesh_col[posBufferIdx].p[2] = ui->doubleSpinBoxB->value();
//                }

//                currentEdge = currentEdge->next;
//            } while (currentEdge != ui->mygl->ObjMesh.mesh_faces[pItem->id].start_edge);

            ui->mygl->ObjMesh.UpdateBuffer();
        }

        ui->verticesListWidget->NotifyMyGL(ui->verticesListWidget->currentItem());
        ui->facesListWidget->NotifyMyGL(ui->facesListWidget->currentItem());
        ui->halfEdgeListWidget->NotifyMyGL(ui->halfEdgeListWidget->currentItem());
        //printf("update color\n");
    }
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    message = new QMessageBox(QMessageBox::Warning,"Information","Please select an item first.",QMessageBox::Ok,NULL);

    message2 = new QMessageBox(QMessageBox::Information,"Information","Skinning succeeded!",QMessageBox::Ok,NULL);
    ui->setupUi(this);
    ui->mygl->setFocus();
    ui->mygl->setFixedWidth(750);

    ui->mygl->ObjMesh.vListWidget = ui->verticesListWidget;
    ui->mygl->ObjMesh.fListWidget = ui->facesListWidget;
    ui->mygl->ObjMesh.eListWidget = ui->halfEdgeListWidget;

    ui->verticesListWidget->pMyGL = ui->mygl;
    ui->facesListWidget->pMyGL = ui->mygl;
    ui->halfEdgeListWidget->pMyGL = ui->mygl;

    ui->verticesListWidget->pBoxX = ui->doubleSpinBoxX;
    ui->verticesListWidget->pBoxY = ui->doubleSpinBoxY;
    ui->verticesListWidget->pBoxZ = ui->doubleSpinBoxZ;
    ui->facesListWidget->pBoxR = ui->doubleSpinBoxR;
    ui->facesListWidget->pBoxG = ui->doubleSpinBoxG;
    ui->facesListWidget->pBoxB = ui->doubleSpinBoxB;

    ui->verticesListWidget->SetupConnection();
    ui->facesListWidget->SetupConnection();
    ui->halfEdgeListWidget->SetupConnection();

    connect(ui->doubleSpinBoxX, SIGNAL(editingFinished()), this, SLOT(modifyVertexPos()));
    connect(ui->doubleSpinBoxY, SIGNAL(editingFinished()), this, SLOT(modifyVertexPos()));
    connect(ui->doubleSpinBoxZ, SIGNAL(editingFinished()), this, SLOT(modifyVertexPos()));
    connect(ui->doubleSpinBoxR, SIGNAL(editingFinished()), this, SLOT(modifyFaceColor()));
    connect(ui->doubleSpinBoxG, SIGNAL(editingFinished()), this, SLOT(modifyFaceColor()));
    connect(ui->doubleSpinBoxB, SIGNAL(editingFinished()), this, SLOT(modifyFaceColor()));
    connect(ui->doubleSpinBox, SIGNAL(editingFinished()), this, SLOT(setSharpness()));

    connect(ui->plus, SIGNAL(clicked()), this, SLOT(modifyJointDegreePlus()));
    connect(ui->minus, SIGNAL(clicked()), this, SLOT(modifyJointDegreeMinus()));
    connect(ui->doubleSpinBox_5, SIGNAL(editingFinished()), this, SLOT(modifyJointPos()));
    connect(ui->doubleSpinBox_6, SIGNAL(editingFinished()), this, SLOT(modifyJointPos()));
    connect(ui->doubleSpinBox_7, SIGNAL(editingFinished()), this, SLOT(modifyJointPos()));

    connect(ui->pushButton, SIGNAL(clicked()), this, SLOT(addVertex()));
    connect(ui->pushButton_2, SIGNAL(clicked()), this, SLOT(triangulateFace()));
    connect(ui->pushButton_3, SIGNAL(clicked()), this, SLOT(subdivision()));
    connect(ui->pushButton_4, SIGNAL(clicked()), this, SLOT(extrude()));
    connect(ui->pushButton_5, SIGNAL(clicked()), this, SLOT(setEdgeSharp()));
    connect(ui->pushButton_6, SIGNAL(clicked()), this, SLOT(setVertexSharp()));
    connect(ui->pushButton_7, SIGNAL(clicked()), this, SLOT(setFaceSharp()));
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));

    connect(ui->treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(HighLightTreeWidgetItem(QTreeWidgetItem*, QTreeWidgetItem*)));

    connect(ui->Skinning, SIGNAL(clicked()), this, SLOT(skinning()));
    timer.start(16);
}

void MainWindow::setSharpness()
{
    ui->mygl->ObjMesh.Sharpness = ui->doubleSpinBox->value();
}

MainWindow::~MainWindow()
{
    delete message;
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    QApplication::exit();
}

void MainWindow::on_actionCamera_Controls_triggered()
{
    CameraControlsHelp* c = new CameraControlsHelp();
    c->show();
}

void MainWindow::on_actionOpen_obj_file_triggered()
{
    QString filename = QFileDialog::getOpenFileName(0, QString("Load Scene File"), QDir::currentPath().append(QString("../..")), QString("*.obj"));
    ui->mygl->isDrawSkeleton = false;
    ui->mygl->isSkin = false;
    Skeleton &skeleton = ui->mygl->skeleton;

    ui->treeWidget->clear();
    skeleton.clear();

    if (filename.isEmpty())
        return;
    qDebug() << filename;
    ui->mygl->ObjMesh.clearListItem();
    ui->mygl->currentEdge = NULL;
    ui->mygl->currentVertex = NULL;
    ui->mygl->currentFace = NULL;
    ui->mygl->ObjMesh.createObjMesh(filename.toStdString().c_str());
    ui->mygl->geom_vertex.SetDraw(false);
    ui->mygl->geom_face.SetDraw(false);
    ui->mygl->geom_edge.SetDraw(false);
//    ui->mygl->isCubeMesh = false;
//    ui->mygl->geom_vertex.destroy();
//    ui->mygl->geom_face.destroy();
//    ui->mygl->geom_edge.destroy();
}

void MainWindow::addVertex()
{
    //ListWidgetItem* pItem;
    //printf("IN\n");
    HalfEdge *crtEdge = ui->mygl->currentEdge;
    if (crtEdge == NULL)
    {
        message->show();
        return;
    }
    ui->mygl->ObjMesh.AddingVertexUpdate(crtEdge);
    ui->mygl->geom_vertex.SetDraw(false);
    ui->mygl->geom_face.SetDraw(false);
    ui->mygl->geom_edge.SetDraw(false);
}

void MainWindow::triangulateFace()
{
    Face *crtFace = ui->mygl->currentFace;
    if (crtFace == NULL)
    {
        message->show();
        return;
    }
    ui->mygl->ObjMesh.AddingFaceUpdate(crtFace);
    ui->mygl->geom_vertex.SetDraw(false);
    ui->mygl->geom_face.SetDraw(false);
    ui->mygl->geom_edge.SetDraw(false);
}

void MainWindow::subdivision()
{
    ui->mygl->ObjMesh.subdivision(this);
    //QLabelDialog
    ui->mygl->geom_vertex.SetDraw(false);
    ui->mygl->geom_face.SetDraw(false);
    ui->mygl->geom_edge.SetDraw(false);
}

void MainWindow::extrude()
{
    if (ui->mygl->currentFace == NULL)
    {
        message->show();
        return;
    }
    ui->mygl->ObjMesh.extrude(ui->mygl->currentFace, this);
    ui->mygl->geom_vertex.SetDraw(false);
    ui->mygl->geom_face.SetDraw(false);
    ui->mygl->geom_edge.SetDraw(false);
}

void MainWindow::setEdgeSharp()
{
    if (ui->mygl->currentEdge == NULL)
    {
        message->show();
        return;
    }
    printf("Set Edge Sharp\n");
    ui->mygl->ObjMesh.mesh_halfEdges[ui->mygl->currentEdge->id].isSharp = true;
    ui->mygl->ObjMesh.mesh_halfEdges[ui->mygl->currentEdge->sym->id].isSharp = true;
}

void MainWindow::setVertexSharp()
{
    if (ui->mygl->currentVertex == NULL)
    {
        message->show();
        return;
    }
    printf("Set Vertex Sharp\n");
    ui->mygl->ObjMesh.mesh_vertices[ui->mygl->currentVertex->id].isSharp = true;
}

void MainWindow::setFaceSharp()
{
    if (ui->mygl->currentFace == NULL)
    {
        message->show();
        return;
    }
    printf("Set Face Sharp\n");
    HalfEdge *startEdge = ui->mygl->ObjMesh.mesh_faces[ui->mygl->currentFace->id].start_edge;
    HalfEdge *currentEdge = startEdge;
    do
    {
        ui->mygl->ObjMesh.mesh_vertices[currentEdge->vert->id].isSharp = true;
        currentEdge = currentEdge->next;
    } while (currentEdge != startEdge);
}

void MainWindow::timerUpdate()
{
//    if (pDialog)
//    {
//        pDialog->setRange(0, 10000);
//        pDialog->setValue(ui->mygl->ObjMesh.progress);
//    }
}
