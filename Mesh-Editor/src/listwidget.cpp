#include "listwidget.h"
#include "listwidgetitem.h"

ListWidget::ListWidget(QWidget *parent):QListWidget(parent)
{

}

void ListWidget::NotifyMyGL(QListWidgetItem *item)
{
    //printf("NOTIYGFF\n");
    if (item == NULL)
        return;
    //crtItem = item;
    ListWidgetItem* pItem = dynamic_cast<ListWidgetItem*>(item);
    //printf("%d\n", pItem->id);
    if (pItem->type() == 3)
    {
        pMyGL->HighLightVertex(pMyGL->ObjMesh.mesh_vertices[pItem->id]);
        pBoxX->setValue(pMyGL->ObjMesh.mesh_vertices[pItem->id].pos[0]);
        pBoxY->setValue(pMyGL->ObjMesh.mesh_vertices[pItem->id].pos[1]);
        pBoxZ->setValue(pMyGL->ObjMesh.mesh_vertices[pItem->id].pos[2]);
        pMyGL->geom_vertex.SetDraw(true);
    }
    else if (pItem->type() == 4)
    {
        pMyGL->geom_face.color = 1.0f - pMyGL->ObjMesh.mesh_faces[pItem->id].color;
        pMyGL->HighLightFace(pMyGL->ObjMesh.mesh_faces[pItem->id]);
        pBoxR->setValue(pMyGL->ObjMesh.mesh_faces[pItem->id].color.p[0]);
        pBoxG->setValue(pMyGL->ObjMesh.mesh_faces[pItem->id].color.p[1]);
        pBoxB->setValue(pMyGL->ObjMesh.mesh_faces[pItem->id].color.p[2]);
    //printf("%f %f %f %f\n", pMyGL->geom_face.color.p[0], pMyGL->geom_face.color.p[1], pMyGL->geom_face.color.p[2], pMyGL->geom_face.color.p[3]);
        pMyGL->geom_face.SetDraw(true);
    }
    else if (pItem->type() == 5)
    {
        pMyGL->HighLightEdge(pMyGL->ObjMesh.mesh_halfEdges[pItem->id]);
        pMyGL->geom_edge.SetDraw(true);
    }
}

void ListWidget::SetupConnection()
{
    connect(this, SIGNAL(itemClicked(QListWidgetItem*)), this, SLOT(NotifyMyGL(QListWidgetItem*)));
    //printf("setup\n");
}
