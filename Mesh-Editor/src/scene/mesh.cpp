#include "mesh.h"
#include "listwidgetitem.h"
#include "QProgressDialog"
#include "QApplication"
#include <algorithm>

#define MAX_CREAT_VERT 8000
#define MAX_VERT 3000000
#define MAX_FACE 3000000
#define MAX_HALFEDGE 3000000
#define INFLUCE_NUM_JOINT 2
int vstd[MAX_CREAT_VERT][MAX_CREAT_VERT];

struct dis
{
    float distance;
    int jointId;
};

bool cmp(const dis &a, const dis &b)
{
    return a.distance < b.distance;
}

void Mesh::skinning(Skeleton &skeleton)
{
    mesh_weights.clear();
    mesh_jointIndices.clear();

    int size = skeleton.vDrawJoint.size();
    int renderingPosSize = mesh_pos.size();
    //int idxSize = mesh_idx.size();
    //printf("pos Size:%d\n", renderingPosSize);
    for (int i = 0; i < renderingPosSize; i++)
    {
        std::vector<dis> disArray;
        int idx = mesh_idx[i];
        for (int j = 0; j < size; j++)
        {
            Joint *crtJoint = skeleton.vDrawJoint[j]->joint;
            dis d;
            d.distance = glm::length(glm::vec3(mesh_pos[i].p[0], mesh_pos[i].p[1], mesh_pos[i].p[2]) - crtJoint->GetWorldPos());
//            if (i==1 && (j==5 || j==10))
//            {
//            printf("%f %f %f %f %f %f\n", mesh_pos[i].p[0], mesh_pos[i].p[1], mesh_pos[i].p[2],
//                    crtJoint->GetWorldPos().x, crtJoint->GetWorldPos().y, crtJoint->GetWorldPos().z);
//            printf("TEST dis:%f\n", d.distance);
//            }
            d.jointId = j;

            disArray.push_back(d);

//            if (i==0)
//            {
//                printf("START\n");
//                print(crtJoint->GetOverallTransformation());
//                printf("---\n");
//                print(crtJoint->bindMatrix);
//                printf("---\n");
//                print(glm::inverse(crtJoint->bindMatrix));
//                printf("---\n");
//            }
        }
        std::sort(disArray.begin(), disArray.end(), cmp);
        float totalDistance = 0.0f;
        for (int j = 0; j < INFLUCE_NUM_JOINT; j++)
            totalDistance += disArray[j].distance;
    //printf("TEST TOTAL:%f\n", totalDistance);

        for (int j = 0; j < INFLUCE_NUM_JOINT; j++)
        {
            mesh_weights.push_back(disArray[j].distance / totalDistance);
            mesh_jointIndices.push_back(disArray[j].jointId);
            //if (i==0)
//            {

//            printf("%f %f %f\n", mesh_pos[i].p[0], mesh_pos[i].p[1], mesh_pos[i].p[2]);
//            printf("%f %d ", disArray[j].distance / totalDistance, disArray[j].jointId);
//            qDebug() << skeleton.vDrawJoint[disArray[j].jointId]->joint->name;
//            }
        }
        //printf("\n");
    }
//float test[24][2]={ {0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},
//                    {0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},
//                    {0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},
//                    {0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7},{0.3,0.7}};
//float test1[48] = {0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,
//                  0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,
//                  0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,
//                  0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7,0.3,0.7};
//for (int i = 0; i < 2; i++)
//    print(skeleton.vDrawJoint[i]->joint->GetOverallTransformation());
    generateWeights();
//    for (int i = 0; i < mesh_weights.size(); i++)
//    {
//        for (int j = 0; j < mesh_weights[i].size(); j++)
//        {
//            if (j==0)
//                mesh_weights[i][j] = 0.3f;
//            else
//                mesh_weights[i][j] = 0.7f;
//        }
//    }
    context->glBindBuffer(GL_ARRAY_BUFFER, bufWeights); //mesh_weights.size() * INFLUCE_NUM_JOINT * sizeof(float)
    context->glBufferData(GL_ARRAY_BUFFER, mesh_weights.size() * sizeof(float), &mesh_weights[0], GL_STATIC_DRAW);
    //context->glBufferData(GL_ARRAY_BUFFER, 48 * sizeof(float), test1, GL_STATIC_DRAW);
    //printf("%p %p %p\n", &mesh_weights[0][0], &mesh_weights[0][1], &mesh_weights[0][2]);
//    for (int i = 0; i < mesh_weights.size(); i+=2)
//    {
//        printf("%f %f\n", mesh_weights[i], mesh_weights[i + 1]);
//    }
//    printf("%d %d %d\n", mesh_weights.size() * INFLUCE_NUM_JOINT, 48 * sizeof(float), sizeof(test));
    generateJointIndices();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufJointIndices);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_jointIndices.size() * sizeof(int), &mesh_jointIndices[0], GL_STATIC_DRAW);

//    for (int i = 0; i < mesh_jointIndices.size(); i+=2)
//    {
//            printf("%d %d\n", mesh_jointIndices[i], mesh_jointIndices[i+1]);
//        //printf("\n");
//    }
    //printf("SIZE:%d %d\n", mesh_weights[0].size(), mesh_jointIndices[1000].size());
}

Mesh::Mesh(GLWidget277 *context) : Drawable(context)
{
    count = 0;
    Sharpness = 0.0f;
}

void Mesh::create()
{
    count = mesh_idx.size();
//printf("count:%d\n", count);
    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
//    printf("1st:%d\n", bufIdx);
//    generateIdx();
//    printf("2nd:%d\n", bufIdx);
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_idx.size() * sizeof(GLuint), &mesh_idx[0], GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_pos.size() * sizeof(vec4), &mesh_pos[0], GL_STATIC_DRAW);

    generateNor();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_nor.size() * sizeof(glm::vec4), &mesh_nor[0], GL_STATIC_DRAW);
    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_col.size() * sizeof(vec4), &mesh_col[0], GL_STATIC_DRAW);
}

void Mesh::meshClear()
{
    mesh_pos.clear();
    mesh_tex.clear();
    mesh_nor.clear();
    mesh_idx.clear();
    mesh_col.clear();

    mesh_vertices.clear();
    mesh_halfEdges.clear();
    mesh_faces.clear();

    mesh_vertBufferIdx.clear();
    mesh_objPos.clear();
    mesh_objTex.clear();
    mesh_objNor.clear();
    mesh_objFace.clear();
    sharpMap.clear();
    VertexSharpMap.clear();

    mesh_weights.clear();
    mesh_jointIndices.clear();

    heVerticesCounter = 0;
    heEdgeCounter = 0;
    heFaceCounter = 0;
}

void Mesh::meshClearExceptObj()
{
    mesh_pos.clear();
    mesh_tex.clear();
    mesh_nor.clear();
    mesh_idx.clear();
    mesh_col.clear();
    mesh_vertBufferIdx.clear();
    mesh_faceBufferIdx.clear();
//    mesh_square_idx.clear();
//    mesh_vertices.clear();
//    mesh_halfEdges.clear();
//    mesh_faces.clear();

//    mesh_vertBufferIdx.clear();
}

void Mesh::meshSubdivisionClear()
{
    mesh_pos.clear();
    mesh_tex.clear();
    mesh_nor.clear();
    mesh_idx.clear();
    mesh_col.clear();

    mesh_vertBufferIdx.clear();
    mesh_faceBufferIdx.clear();

    mesh_vertices.clear();
    mesh_halfEdges.clear();
    mesh_faces.clear();

    heVerticesCounter = 0;
    heEdgeCounter = 0;
    heFaceCounter = 0;
//    mesh_vertBufferIdx.clear();
}

void Mesh::createObjMesh(const char *filename)
{
    meshClear();

    FILE *fp = fopen(filename, "r");
    char str[1024];
    vec4 v;
    vec2 v2;
    int3 f;
    while (fscanf(fp, "%s", str) != EOF)
    {
        if (!strcmp(str, "v"))
        {
            fscanf(fp, "%f %f %f", &v.p[0], &v.p[1], &v.p[2]);
            v.p[3] = 1.0f;
            mesh_objPos.push_back(v);
        }
        else if (!strcmp(str, "vt"))
        {
            fscanf(fp, "%f %f", &v2.p[0], &v2.p[1]);
            mesh_objTex.push_back(v2);
        }
        else if (!strcmp(str, "vn"))
        {
            fscanf(fp, "%f %f %f", &v.p[0], &v.p[1], &v.p[2]);
            v.p[3] = 0.0f;
            mesh_objNor.push_back(v);
        }
        else if (!strcmp(str, "f"))
        {
            std::vector<int3> newFace;
            while (1)
            {
                if (fgetc(fp) == '\n')
                    break;
                fscanf(fp, "%d/%d/%d", &f.p[0], &f.p[1], &f.p[2]);
                f.p[0]--;
                f.p[1]--;
                f.p[2]--;
                newFace.push_back(f);
            }
            mesh_objFace.push_back(newFace);
        }
        else
        {
            fgets(str, 1024, fp);
        }
    }
    fclose(fp);

    int objFaceSize = mesh_objFace.size();
    mesh_vertBufferIdx.resize(mesh_objPos.size());
    mesh_faceBufferIdx.resize(objFaceSize);
    for (int i = 0; i < objFaceSize; i++)
    {
        int vertexNum = mesh_objFace[i].size();
        int idxSize = mesh_pos.size();
        std::vector<int> newVector;
        for (int j = 0; j < vertexNum; j++)
        {
            int posIdx = mesh_objFace[i][j].p[0];
            int texIdx = mesh_objFace[i][j].p[1];
            int norIdx = mesh_objFace[i][j].p[2];

            mesh_vertBufferIdx[posIdx].push_back(mesh_pos.size());
            mesh_faceBufferIdx[i].push_back(mesh_pos.size());
            mesh_pos.push_back(mesh_objPos[posIdx]);
            mesh_tex.push_back(mesh_objTex[texIdx]);
            mesh_nor.push_back(mesh_objNor[norIdx]);
            mesh_col.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        for (int j = 0; j < vertexNum - 2; j++)
        {
            mesh_idx.push_back(idxSize);
            mesh_idx.push_back(idxSize + j + 1);
            mesh_idx.push_back(idxSize + j + 2);
        }
    }
//    for (int i = 0 ; i < mesh_idx.size(); i++)
//    {
//        printf("%d:%f %f %f\n",mesh_idx[i], mesh_pos[mesh_idx[i]].p[0], mesh_pos[mesh_idx[i]].p[1], mesh_pos[mesh_idx[i]].p[2]);
//    }
//    for (int i = 0; i < mesh_objFace.size(); i++)
//    {
//        for (int j = 0; j < mesh_objFace[i].size(); j++)
//            printf("%d/%d/%d ", mesh_objFace[i][j].p[0], mesh_objFace[i][j].p[1], mesh_objFace[i][j].p[2]);
//        printf("\n");
//    }

    mesh_vertices.resize(MAX_VERT);
    // mesh_vertices's indices are equal to mesh_objPos
    heVerticesCounter = mesh_objPos.size();
    for (int i = 0; i < mesh_objPos.size(); i++)
    {
        mesh_vertices[i] = Vertex(mesh_objPos[i].p[0], mesh_objPos[i].p[1], mesh_objPos[i].p[2], 1.0f);
        mesh_vertices[i].id = i;
    }
    memset(vstd, -1, sizeof(vstd));

    mesh_halfEdges.resize(MAX_HALFEDGE);
    mesh_faces.resize(MAX_FACE);
    for (int i = 0; i < mesh_objFace.size(); i++)
    {
        std::vector<int> idx;
        for (int j = 0; j < mesh_objFace[i].size(); j++)
            idx.push_back(mesh_objFace[i][j].p[0]);
        idx.push_back(mesh_objFace[i][0].p[0]);

        int a = idx[0];
        int b = idx[1];

        if (vstd[a][b] == -1)
        {
            int edge_id = heEdgeCounter;

            HalfEdge &h = mesh_halfEdges[edge_id];
            HalfEdge &hSym = mesh_halfEdges[edge_id + 1];

            h.sym = &hSym;
            h.id = edge_id;
            //h.face = &f;
            h.vert = &mesh_vertices[b];
            //printf("%d:%f %f %f\n", b, h.vert->pos[0], h.vert->pos[1], h.vert->pos[2]);
            mesh_vertices[b].edge = &h;

            hSym.sym = &h;
            hSym.id = edge_id + 1;
            hSym.face = NULL;
            hSym.vert = &mesh_vertices[a];
            //printf("%d:%f %f %f\n", a, hSym.vert->pos[0], hSym.vert->pos[1], hSym.vert->pos[2]);
            mesh_vertices[a].edge = &hSym;

            vstd[a][b] = edge_id;
            vstd[b][a] = edge_id + 1;

            heEdgeCounter += 2;
            //printf("s:%d %d\n",edge_id, edge_id + 1);
        }


        int face_id = heFaceCounter;
        Face &f = mesh_faces[face_id];
        f.id = face_id;
        f.color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        mesh_halfEdges[vstd[a][b]].face = &f;
        f.start_edge = &mesh_halfEdges[vstd[a][b]];
        HalfEdge &startEdge = mesh_halfEdges[vstd[a][b]];
        //printf("startEdge:%d vert:%d\n", vstd[a][b], mesh_halfEdges[vstd[a][b]].vert->id);
        heFaceCounter++;

        for (int j = 1; j < mesh_objFace[i].size(); j++)
        {
            HalfEdge &preEdge = mesh_halfEdges[vstd[a][b]];
            a = idx[j];
            b = idx[j + 1];
            if (vstd[a][b] == -1)
            {
                int edge_id = heEdgeCounter;

                HalfEdge &h = mesh_halfEdges[edge_id];
                HalfEdge &hSym = mesh_halfEdges[edge_id + 1];

                h.sym = &hSym;
                h.id = edge_id;
                h.vert = &mesh_vertices[b];
                mesh_vertices[b].edge = &h;

                hSym.sym = &h;
                hSym.id = edge_id + 1;
                hSym.face = NULL;
                hSym.vert = &mesh_vertices[a];
                mesh_vertices[a].edge = &hSym;

                vstd[a][b] = edge_id;
                vstd[b][a] = edge_id + 1;

                heEdgeCounter += 2;
                //printf("%d %d %d %d %d %d\n",edge_id, edge_id + 1, h.id, hSym.id, mesh_halfEdges[edge_id].id, mesh_halfEdges[edge_id + 1].id);
            }
            mesh_halfEdges[vstd[a][b]].face = &f;

            preEdge.next = &mesh_halfEdges[vstd[a][b]];
        }
        // the last edge points to the start edge
        mesh_halfEdges[vstd[a][b]].next = &startEdge;

//        if (i==0)
//        {
//            HalfEdge *t = &startEdge;
//            for (int k = 0; k < 4; k++)
//            {
//                printf("%d->", t->id);
//                t = t->next;
//            }
//            printf("\n");
//        }
    }
    //printf("edge:%d\n", mesh_faces[0].start_edge->id);
    AddListItem();

    destroy();
    create();
}

void Mesh::clearListItem()
{
   // printf("A");
    vListWidget->clear();
    //printf("A");
    fListWidget->clear();
   // printf("A");
    eListWidget->clear();
    //printf("A");
//    while (1)
//    {
//        QListWidgetItem *p = vListWidget->takeItem(0);
//        if (!p) break;
//        delete p;
//    }
//    //printf("B");
//    //fListWidget->clear();
//    while (1)
//    {
//        QListWidgetItem *p = fListWidget->takeItem(0);
//        if (!p) break;
//        delete p;
//    }
//    //printf("C");
//    while (1)
//    {
//        QListWidgetItem *p = eListWidget->takeItem(0);
//        if (!p) break;
//        delete p;
//    }
    //printf("D");
}

void Mesh::AddListItem()
{
    for (int i = 0; i < heVerticesCounter; i++)
    {
        ListWidgetItem *item = new ListWidgetItem(mesh_vertices[i].id, QString().setNum(mesh_vertices[i].id), vListWidget, 3);
        //ListWidgetItem item(mesh_vertices[i].id, QString().setNum(mesh_vertices[i].id), vListWidget, 3);
    }

    for (int i = 0; i < heFaceCounter; i++)
    {
       ListWidgetItem *item = new ListWidgetItem(mesh_faces[i].id, QString().setNum(mesh_faces[i].id), fListWidget, 4);
        //ListWidgetItem item(mesh_faces[i].id, QString().setNum(mesh_faces[i].id), fListWidget, 4);
    }

    for (int i = 0; i < heEdgeCounter; i++)
    {
        //printf("%d ",mesh_halfEdges[i].id);
        ListWidgetItem *item = new ListWidgetItem(mesh_halfEdges[i].id, QString().setNum(mesh_halfEdges[i].id), eListWidget, 5);
        //ListWidgetItem item(mesh_halfEdges[i].id, QString().setNum(mesh_halfEdges[i].id), eListWidget, 5);
        //printf("%d\n",mesh_halfEdges[i].id);
    }
}
void Mesh::UpdateBuffer()
{
    count = mesh_idx.size();

    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_idx.size() * sizeof(GLuint), &mesh_idx[0], GL_STATIC_DRAW);
//    for (int i = 0; i < mesh_col.size(); i++)
//        printf("%d:%f %f %f\n", i, mesh_col[i].p[0], mesh_col[i].p[1], mesh_col[i].p[2]);
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_pos.size() * sizeof(vec4), &mesh_pos[0], GL_STATIC_DRAW);

    context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_nor.size() * sizeof(glm::vec4), &mesh_nor[0], GL_STATIC_DRAW);

    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_col.size() * sizeof(vec4), &mesh_col[0], GL_STATIC_DRAW);
    //printf("SIZE:%d %d %d %d\n", mesh_idx.size(), mesh_pos.size(), mesh_nor.size(), mesh_col.size());
}

void Mesh::AddingVertexUpdate(HalfEdge *crtEdge)
{
//    HalfEdge *s = crtEdge;
//    HalfEdge *c = s;
//    //geom_face.v.clear();
//    do
//    {
//        //printf("vert:%d\n", currentEdge->vert->id);
//        //geom_face.v.push_back(*currentEdge->vert);
//        printf("%d ", c->id);
//        //mesh_objFace[faceId].push_back(int3(heVerticesCounter - 1, 0, 0));
//        c = c->next;
//    } while (c != s);
//    printf("\n");
    meshClearExceptObj();

    Vertex *v0 = crtEdge->vert;
    Vertex *v1 = crtEdge->sym->vert;
    Vertex v((v0->pos[0] + v1->pos[0]) / 2, (v0->pos[1] + v1->pos[1]) / 2, (v0->pos[2] + v1->pos[2]) / 2, 1.0f);
    v.id = heVerticesCounter;
    mesh_vertices[heVerticesCounter] = v;

    HalfEdge e0(crtEdge->face, crtEdge->vert, crtEdge->next, crtEdge->sym, heEdgeCounter);
    mesh_halfEdges[heEdgeCounter] = e0;
    crtEdge->next = &mesh_halfEdges[heEdgeCounter];
    crtEdge->vert = &mesh_vertices[heVerticesCounter];
    heEdgeCounter++;

    HalfEdge e1(crtEdge->sym->face, crtEdge->sym->vert, crtEdge->sym->next, crtEdge, heEdgeCounter);
    mesh_halfEdges[heEdgeCounter] = e1;
    crtEdge->sym->next = &mesh_halfEdges[heEdgeCounter];
    crtEdge->sym->vert = &mesh_vertices[heVerticesCounter];
    heEdgeCounter++;

    HalfEdge *tempSym = crtEdge->sym;
    crtEdge->sym = &mesh_halfEdges[heEdgeCounter - 1];
    tempSym->sym = &mesh_halfEdges[heEdgeCounter - 2];

    v.edge = &mesh_halfEdges[heEdgeCounter - 1];
    heVerticesCounter++;

    vec4 newPos((v0->pos[0] + v1->pos[0]) / 2, (v0->pos[1] + v1->pos[1]) / 2, (v0->pos[2] + v1->pos[2]) / 2, 1.0f);
    int crtFaceId = crtEdge->face->id;
    int symFaceId = crtEdge->sym->face->id;

    mesh_objPos.push_back(newPos);
    HalfEdge *startEdge;
    HalfEdge *currentEdge;

    // add point to cur face
    std::map<int, int> normalMap;
    int tSize = mesh_objFace[crtFaceId].size();
    for (int i = 0; i < tSize; i++)
        normalMap[mesh_objFace[crtFaceId][i].p[0]] = mesh_objFace[crtFaceId][i].p[2];

{
    int norIdx = mesh_objNor.size();
    int v0 = mesh_objFace[crtFaceId][0].p[2];
    int v1 = mesh_objFace[crtFaceId][1].p[2];
    int v2 = mesh_objFace[crtFaceId][2].p[2];
    vec4 vv0 = mesh_objPos[v1] - mesh_objPos[v0];
    //printf("vv:%f %f %f\n", vv0.p[0], vv0.p[1], vv0.p[2]);
    vec4 vv1 = mesh_objPos[v2] - mesh_objPos[v1];
    mesh_objNor.push_back(normalize(cross(vv0, vv1)));

    mesh_objFace[crtFaceId].clear();
    startEdge = crtEdge;
    currentEdge = startEdge;
    do
    {
        mesh_objFace[crtFaceId].push_back(int3(currentEdge->vert->id, 0, normalMap[norIdx]));
        //printf("%d ", currentEdge->vert->id);
        currentEdge = currentEdge->next;
    } while (currentEdge != startEdge);
}
    //printf("\n");
    // sym face
{
    int norIdx = mesh_objNor.size();
    int v0 = mesh_objFace[symFaceId][0].p[2];
    int v1 = mesh_objFace[symFaceId][1].p[2];
    int v2 = mesh_objFace[symFaceId][2].p[2];
    vec4 vv0 = mesh_objPos[v1] - mesh_objPos[v0];
    //printf("vv:%f %f %f\n", vv0.p[0], vv0.p[1], vv0.p[2]);
    vec4 vv1 = mesh_objPos[v2] - mesh_objPos[v1];
    mesh_objNor.push_back(normalize(cross(vv0, vv1)));

    mesh_objFace[symFaceId].clear();
    startEdge = crtEdge->sym;
    currentEdge = startEdge;
    do
    {
        mesh_objFace[symFaceId].push_back(int3(currentEdge->vert->id, 0, normalMap[norIdx]));
        //printf("%d ", currentEdge->vert->id);
        currentEdge = currentEdge->next;
    } while (currentEdge != startEdge);
}
    //printf("\n");

// --- removed 11/3---
    int objFaceSize = mesh_objFace.size();
    mesh_vertBufferIdx.resize(mesh_objPos.size());
    mesh_faceBufferIdx.resize(objFaceSize);
    for (int i = 0; i < objFaceSize; i++)
    {
        int vertexNum = mesh_objFace[i].size();
        //printf("%d\n", heVerticesCounter);
        int idxSize = mesh_pos.size();
        std::vector<int> newVector;
        for (int j = 0; j < vertexNum; j++)
        {
            int posIdx = mesh_objFace[i][j].p[0];
            int texIdx = mesh_objFace[i][j].p[1];
            int norIdx = mesh_objFace[i][j].p[2];
//            if (i == 5)
//                printf("POSIDX:%f %f %f\n", mesh_objPos[posIdx].p[0], mesh_objPos[posIdx].p[1], mesh_objPos[posIdx].p[2]);
            mesh_vertBufferIdx[posIdx].push_back(mesh_pos.size());
            mesh_faceBufferIdx[i].push_back(mesh_pos.size());
            mesh_pos.push_back(mesh_objPos[posIdx]);
            mesh_tex.push_back(mesh_objTex[texIdx]);
            mesh_nor.push_back(mesh_objNor[norIdx]);
            mesh_col.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        for (int j = 0; j < vertexNum - 2; j++)
        {
            mesh_idx.push_back(idxSize);
            mesh_idx.push_back(idxSize + j + 1);
            mesh_idx.push_back(idxSize + j + 2);
//            if (i == 5)
//            {
//                printf("%d %d %d\n", idxSize, idxSize + j + 1, idxSize + j + 2);
//            printf("%f %f %f\n %f %f %f\n %f %f %f\n\n", mesh_pos[idxSize].p[0], mesh_pos[idxSize].p[1], mesh_pos[idxSize].p[2],
//                    mesh_pos[idxSize + j + 1].p[0], mesh_pos[idxSize + j + 1].p[1], mesh_pos[idxSize + j + 1].p[2],
//                    mesh_pos[idxSize + j + 2].p[0], mesh_pos[idxSize + j + 2].p[1], mesh_pos[idxSize + j + 2].p[2]);
//            }
        }
    }

    //printf("%d\n", mesh_pos.size());
    UpdateBuffer();
    clearListItem();
    AddListItem();
// --- removed 11/3---
}


void Mesh::AddingFaceUpdate(Face *crtFace)
{
    HalfEdge *startEdge = crtFace->start_edge;
    HalfEdge *currentEdge = startEdge;
    int counter = 0;
    do
    {
        counter++;
        currentEdge = currentEdge->next;
    } while (currentEdge != startEdge);

    if (counter < 4) return;
    //printf("HERE0\n");
    meshClearExceptObj();
    std::map<int, int> normalMap;
    int tSize = mesh_objFace[crtFace->id].size();
    for (int i = 0; i < tSize; i++)
        normalMap[mesh_objFace[crtFace->id][i].p[0]] = mesh_objFace[crtFace->id][i].p[2];
    mesh_objFace[crtFace->id].clear();
    while (1)
    {
        std::vector<HalfEdge*> vEdge;
        startEdge = crtFace->start_edge;
        currentEdge = startEdge;
        do
        {
            vEdge.push_back(currentEdge);
            currentEdge = currentEdge->next;
        } while (currentEdge != startEdge);

        int size = vEdge.size();
        //printf("HERE1\n");
        if (size < 4) break;

        HalfEdge *e0 = vEdge[0];
        HalfEdge *e1 = vEdge[1];
        HalfEdge *e2 = vEdge[2];
        HalfEdge *e3 = vEdge[3];

        Face f1;
        f1.start_edge = e1;
        f1.id = heFaceCounter;
        //f1.color = crtFace->color;
        mesh_faces[heFaceCounter++] = f1;

        Face f0;
        f0.start_edge = e0;
        f0.id = heFaceCounter;
        //f0.color = crtFace->color;
        mesh_faces[heFaceCounter++] = f0;

        HalfEdge b0(&mesh_faces[heFaceCounter - 1], e2->vert, e3, NULL, heEdgeCounter);
        mesh_halfEdges[heEdgeCounter] = b0;
        heEdgeCounter++;
        HalfEdge b1(&mesh_faces[heFaceCounter - 2], e0->vert, e1, &mesh_halfEdges[heEdgeCounter - 1], heEdgeCounter);
        mesh_halfEdges[heEdgeCounter] = b1;
        mesh_halfEdges[heEdgeCounter - 1].sym = &mesh_halfEdges[heEdgeCounter];
        heEdgeCounter++;

        int tLast = mesh_objFace.size();
        mesh_objFace.push_back(std::vector<int3>());


        int norIdx = mesh_objNor.size();
        int v0 = e0->vert->id;
        int v1 = e1->vert->id;
        int v2 = e2->vert->id;
        vec4 vv0 = mesh_objPos[v1] - mesh_objPos[v0];
        //printf("vv:%f %f %f\n", vv0.p[0], vv0.p[1], vv0.p[2]);
        vec4 vv1 = mesh_objPos[v2] - mesh_objPos[v1];
        mesh_objNor.push_back(normalize(cross(vv0, vv1)));

        mesh_objFace[tLast].push_back(int3(e0->vert->id, 0, norIdx));
        mesh_objFace[tLast].push_back(int3(e1->vert->id, 0, norIdx));
        mesh_objFace[tLast].push_back(int3(e2->vert->id, 0, norIdx));



        e2->next = &mesh_halfEdges[heEdgeCounter - 1];
        e0->next = &mesh_halfEdges[heEdgeCounter - 2];

        if (size == 4)
        {
            int norIdx = mesh_objNor.size();
            int v0 = e0->vert->id;
            int v1 = b0.vert->id;
            int v2 = e3->vert->id;
            vec4 vv0 = mesh_objPos[v1] - mesh_objPos[v0];
            //printf("vv:%f %f %f\n", vv0.p[0], vv0.p[1], vv0.p[2]);
            vec4 vv1 = mesh_objPos[v2] - mesh_objPos[v1];
            mesh_objNor.push_back(normalize(cross(vv0, vv1)));

            mesh_objFace[crtFace->id].push_back(int3(e0->vert->id, 0, norIdx));
            mesh_objFace[crtFace->id].push_back(int3(b0.vert->id, 0, norIdx));
            mesh_objFace[crtFace->id].push_back(int3(e3->vert->id, 0, norIdx));

            f0.id = crtFace->id;
            mesh_faces[crtFace->id] = f0;
            heFaceCounter--;
            mesh_halfEdges[heEdgeCounter - 2].face = &mesh_faces[crtFace->id];
        }
    }
//    startEdge = mesh_faces[0].start_edge;
//    currentEdge = startEdge;
//    do
//    {
//        printf("%d\n",currentEdge->vert->id);
//        currentEdge = currentEdge->next;
//    } while (currentEdge != startEdge);

    int objFaceSize = mesh_objFace.size();
    mesh_vertBufferIdx.resize(mesh_objPos.size());
    mesh_faceBufferIdx.resize(objFaceSize);
    for (int i = 0; i < objFaceSize; i++)
    {
        int vertexNum = mesh_objFace[i].size();
        //printf("%d\n", heVerticesCounter);
        int idxSize = mesh_pos.size();
        std::vector<int> newVector;
        for (int j = 0; j < vertexNum; j++)
        {
            int posIdx = mesh_objFace[i][j].p[0];
            int texIdx = mesh_objFace[i][j].p[1];
            int norIdx = mesh_objFace[i][j].p[2];
//            if (i == 5)
//                printf("POSIDX:%f %f %f\n", mesh_objPos[posIdx].p[0], mesh_objPos[posIdx].p[1], mesh_objPos[posIdx].p[2]);
            mesh_vertBufferIdx[posIdx].push_back(mesh_pos.size());
            mesh_faceBufferIdx[i].push_back(mesh_pos.size());
//            if (i==0)
//                printf("fb:%d\n", mesh_faceBufferIdx[i].size());
            mesh_pos.push_back(mesh_objPos[posIdx]);
            mesh_tex.push_back(mesh_objTex[texIdx]);
            mesh_nor.push_back(mesh_objNor[norIdx]);
            mesh_col.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        for (int j = 0; j < vertexNum - 2; j++)
        {
            mesh_idx.push_back(idxSize);
            mesh_idx.push_back(idxSize + j + 1);
            mesh_idx.push_back(idxSize + j + 2);
//            if (i == 5)
//            {
//                printf("%d %d %d\n", idxSize, idxSize + j + 1, idxSize + j + 2);
//            printf("%f %f %f\n %f %f %f\n %f %f %f\n\n", mesh_pos[idxSize].p[0], mesh_pos[idxSize].p[1], mesh_pos[idxSize].p[2],
//                    mesh_pos[idxSize + j + 1].p[0], mesh_pos[idxSize + j + 1].p[1], mesh_pos[idxSize + j + 1].p[2],
//                    mesh_pos[idxSize + j + 2].p[0], mesh_pos[idxSize + j + 2].p[1], mesh_pos[idxSize + j + 2].p[2]);
//            }
        }
    }//printf("HERE0\n");
    UpdateBuffer();
    clearListItem();
    AddListItem();
}


void Mesh::subdivision(QWidget *main)
{
    m.clear();
    memset(mid, -1, sizeof(mid));
    memset(cen, 0, sizeof(cen));
    int tVerticesCounter = heVerticesCounter;

    // compute central points
    for (int i = 0; i < heFaceCounter; i++)
    {
        vec4 centralPoint;
        HalfEdge *startEdge = mesh_faces[i].start_edge;
        HalfEdge *currentEdge = startEdge;
        int counter = 0;
        do
        {
            counter++;
            centralPoint = centralPoint + vec4(currentEdge->vert->pos[0], currentEdge->vert->pos[1], currentEdge->vert->pos[2], 1.0f);
//            if(i==1)
//            {

//                printf("cen adding:%f %f %f\n", centralPoint.p[0], centralPoint.p[1], centralPoint.p[2]);
//            }
            currentEdge = currentEdge->next;
        } while (currentEdge != startEdge);
//        if (i==1)
//        printf("cen before:%f %f %f\n", centralPoint.p[0], centralPoint.p[1], centralPoint.p[2]);
        centralPoint = centralPoint / counter;
        cen[i] = mesh_objPos.size();
        mesh_objPos.push_back(centralPoint);
//    if (i==1)
        //printf("cen after:%f %f %f\n", centralPoint.p[0], centralPoint.p[1], centralPoint.p[2]);
        //mesh_vertices[heVerticesCounter++] = Vertex(centralPoint);
    }

    // compute middle points
    for (int i = 0; i < heEdgeCounter; i++)
    {
        vec4 v0(mesh_halfEdges[i].vert->pos[0], mesh_halfEdges[i].vert->pos[1], mesh_halfEdges[i].vert->pos[2], 1.0f);
        vec4 v1(mesh_halfEdges[i].sym->vert->pos[0], mesh_halfEdges[i].sym->vert->pos[1], mesh_halfEdges[i].sym->vert->pos[2], 1.0f);
        int vid0 = mesh_halfEdges[i].vert->id;
        int vid1 = mesh_halfEdges[i].sym->vert->id;
        int eid0 = mesh_halfEdges[i].id;
        int eid1 = mesh_halfEdges[i].sym->id;
        if (mid[eid0] != -1 && mid[eid1] != -1) continue;

        int fid0 = mesh_halfEdges[i].face->id;
        int fid1 = mesh_halfEdges[i].sym->face->id;
        vec4 midpoint;
        int midId = mesh_objPos.size();
        if (!mesh_halfEdges[i].isSharp && !mesh_halfEdges[i].isSharp)
            midpoint = (v0 + v1 + mesh_objPos[cen[fid0]] + mesh_objPos[cen[fid1]]) / 4;
        else if (mesh_halfEdges[i].isSharp && mesh_halfEdges[i].isSharp)
        {
            midpoint = (v0 + v1) / 2;
            std::pair<int, int> pa(midId, vid0);
            std::pair<int, int> pb(vid0, midId);
            std::pair<int, int> pc(midId, vid1);
            std::pair<int, int> pd(vid1, midId);
            sharpMap[pa] = sharpMap[pb] = sharpMap[pc] = sharpMap[pd] = true;
        }
        //printf("midpoint:%f %f %f\n", midpoint.p[0], midpoint.p[1], midpoint.p[2]);


        mid[eid0] = mid[eid1] = midId;
        mesh_objPos.push_back(midpoint);
        //mesh_vertices[heVerticesCounter++] = Vertex(midpoint);
    }

    // compute smoothed vertices
    for (int i = 0; i < tVerticesCounter; i++)
    {
        vec4 sumMidPoint;
        vec4 sumCenPoint;
        HalfEdge *startEdge = mesh_vertices[i].edge;
        HalfEdge *currentEdge = startEdge;
        //printf("ii:%d\n", i);
        int n = 0;
        int sharpCounter = 0;
        vec4 sharpEdgeVertPos[10];
        do
        {
//            if (i==3)
//                printf("%d %d\n", currentEdge->id, currentEdge->next->id);
            n++;
            currentEdge = currentEdge->next;
            if (currentEdge->isSharp)
            {
                sharpEdgeVertPos[sharpCounter++] = vec4(currentEdge->vert->pos[0], currentEdge->vert->pos[1], currentEdge->vert->pos[2], 1.0f);
            }

            int vid0 = currentEdge->vert->id;
            int vid1 = currentEdge->sym->vert->id;
            sumMidPoint = sumMidPoint + mesh_objPos[mid[currentEdge->id]];
            sumCenPoint = sumCenPoint + mesh_objPos[cen[currentEdge->sym->face->id]];
            currentEdge = currentEdge->sym;


        } while (currentEdge != startEdge);
        //printf("N:%d\n", n);
        if (mesh_vertices[i].isSharp)
        {
//            if (i == 3)
//            {
//                printf("Sharpness:%f\n", Sharpness);
//                printf("%f %f %f\n", mesh_objPos[i].p[0], mesh_objPos[i].p[1], mesh_objPos[i].p[2]);
//                printf("%f %f %f\n", (mesh_objPos[i] * (n - 2) / n + sumMidPoint / (n * n) + sumCenPoint / (n * n)).p[0],
//                        (mesh_objPos[i] * (n - 2) / n + sumMidPoint / (n * n) + sumCenPoint / (n * n)).p[1],
//                        (mesh_objPos[i] * (n - 2) / n + sumMidPoint / (n * n) + sumCenPoint / (n * n)).p[2]);
//            }
            mesh_objPos[i] = mesh_objPos[i] * Sharpness + (mesh_objPos[i] * (n - 2) / n + sumMidPoint / (n * n) + sumCenPoint / (n * n)) * (1 - Sharpness);
//            if (i == 3)
//                printf("%f %f %f\n", mesh_objPos[i].p[0], mesh_objPos[i].p[1], mesh_objPos[i].p[2]);
            VertexSharpMap[i] = true;
        }
        else if (sharpCounter < 2)
            mesh_objPos[i] = (mesh_objPos[i] * (n - 2) / n + sumMidPoint / (n * n) + sumCenPoint / (n * n));
        else if (sharpCounter == 2)
            mesh_objPos[i] = (mesh_objPos[i] * 0.75f + (sharpEdgeVertPos[0] + sharpEdgeVertPos[1]) * 0.125f) * Sharpness + (mesh_objPos[i] * (n - 2) / n + sumMidPoint / (n * n) + sumCenPoint / (n * n)) * (1 - Sharpness);
        else
            mesh_objPos[i] = mesh_objPos[i] * Sharpness + (mesh_objPos[i] * (n - 2) / n + sumMidPoint / (n * n) + sumCenPoint / (n * n)) * (1 - Sharpness);
        // do not change if n > 2

        //mesh_vertices[i] = mesh_objPos[i];
    }
//    for (int i = 0; i < tVerticesCounter; i++)
//        printf("%f %f %f\n", mesh_objPos[i].p[0], mesh_objPos[i].p[1], mesh_objPos[i].p[2]);

    // update mesh_objFace
    std::vector<std::vector<int3>> newMeshFaces;
    std::vector<vec4> newMeshNormals;
    for (int i = 0; i < heFaceCounter; i++)
    {
        int verticesNum = mesh_objFace[i].size();
        int cenid = cen[i];
        HalfEdge *startEdge = mesh_faces[i].start_edge;
        HalfEdge *currentEdge = startEdge;
        std::vector<HalfEdge*> edge;
        do
        {
            edge.push_back(currentEdge);
            currentEdge = currentEdge->next;
        } while (currentEdge != startEdge);

        int edgeSize = edge.size();
        //printf("edgesize:%d\n", edgeSize);
        for (int j = 0; j < edgeSize; j++)
        {
            int crtNewFaceId = newMeshFaces.size();
            newMeshFaces.push_back(std::vector<int3>());
            int vid_1 = edge[(j - 1 + edgeSize) % edgeSize]->vert->id;
            int vid0 = edge[j]->vert->id;
            int vid1 = edge[(j + 1) % edgeSize]->vert->id;
            int mid0 = mid[edge[(j + 1) % edgeSize]->id];
            int mid1 = mid[edge[j]->id];

            int nid = newMeshNormals.size();
            newMeshFaces[crtNewFaceId].push_back(int3(vid0, 0, nid));
            newMeshFaces[crtNewFaceId].push_back(int3(mid0, 0, nid));
            newMeshFaces[crtNewFaceId].push_back(int3(cenid, 0, nid));
            newMeshFaces[crtNewFaceId].push_back(int3(mid1, 0, nid));

            vec4 v0 = mesh_objPos[mid0] - mesh_objPos[vid0];
            vec4 v1 = mesh_objPos[cenid] - mesh_objPos[mid0];
            newMeshNormals.push_back(normalize(cross(v0, v1)));
        }
//        for (int j = 0; j < verticesNum; j++)
//        {
//            int crtNewFaceId = newMeshFaces.size();
//            newMeshFaces.push_back(std::vector<int3>());
//            int vid_1 = mesh_objFace[i][(j - 1 + verticesNum) % verticesNum].p[0];
//            int vid0 = mesh_objFace[i][j].p[0];
//            int vid1 = mesh_objFace[i][(j + 1) % verticesNum].p[0];
//            int mid0 = mid[vid0][vid1];
//            int mid1 = mid[vid_1][vid0];

//            int nid = newMeshNormals.size();
//            newMeshFaces[crtNewFaceId].push_back(int3(vid0, 0, nid));
//            newMeshFaces[crtNewFaceId].push_back(int3(mid0, 0, nid));
//            newMeshFaces[crtNewFaceId].push_back(int3(cenid, 0, nid));
//            newMeshFaces[crtNewFaceId].push_back(int3(mid1, 0, nid));

//            vec4 v0 = mesh_objPos[vid0] - mesh_objPos[vid_1];
//            vec4 v1 = mesh_objPos[vid1] - mesh_objPos[vid0];
//            newMeshNormals.push_back(normalize(cross(v0, v1)));

//        }
    }
    mesh_objFace.clear();
    mesh_objFace = newMeshFaces;
    mesh_objNor.clear();
    mesh_objNor = newMeshNormals;
//for (int i = 0; i < mesh_objFace.size(); i++)
//{
//    for (int j = 0; j < mesh_objFace[i].size(); j++)
//        printf("%d/", mesh_objFace[i][j].p[0]);
//    printf("\n");
//}
    meshSubdivisionClear();

    // load data for GL buffer
    int objFaceSize = mesh_objFace.size();
    mesh_vertBufferIdx.resize(mesh_objPos.size());
    mesh_faceBufferIdx.resize(objFaceSize);
    for (int i = 0; i < objFaceSize; i++)
    {
        int vertexNum = mesh_objFace[i].size();
        int idxSize = mesh_pos.size();
        std::vector<int> newVector;
        for (int j = 0; j < vertexNum; j++)
        {
            int posIdx = mesh_objFace[i][j].p[0];
            int texIdx = mesh_objFace[i][j].p[1];
            int norIdx = mesh_objFace[i][j].p[2];

            mesh_vertBufferIdx[posIdx].push_back(mesh_pos.size());
            mesh_faceBufferIdx[i].push_back(mesh_pos.size());
            mesh_pos.push_back(mesh_objPos[posIdx]);
            mesh_tex.push_back(mesh_objTex[texIdx]);
            mesh_nor.push_back(mesh_objNor[norIdx]);
            mesh_col.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        for (int j = 0; j < vertexNum - 2; j++)
        {
            mesh_idx.push_back(idxSize);
            mesh_idx.push_back(idxSize + j + 1);
            mesh_idx.push_back(idxSize + j + 2);
        }
    }

    // compute data for half-edge mesh
    mesh_vertices.resize(MAX_VERT);
    // mesh_vertices's indices are equal to mesh_objPos
    heVerticesCounter = mesh_objPos.size();
    for (int i = 0; i < mesh_objPos.size(); i++)
    {
        mesh_vertices[i] = Vertex(mesh_objPos[i].p[0], mesh_objPos[i].p[1], mesh_objPos[i].p[2], 1.0f);
        mesh_vertices[i].id = i;
        mesh_vertices[i].isSharp = VertexSharpMap[i];
    }
    memset(vstd, -1, sizeof(vstd));
    using namespace std;


    map<pair<int, int>, int>::iterator iter;
//    pair<int, int> ppp(2, 17);
//    m[ppp] = 1;
//    iter = m.find(ppp);
//    if (iter == m.end())
//        printf("not found\n");
//    else
//        printf("found\n");
    mesh_halfEdges.resize(MAX_HALFEDGE);
    mesh_faces.resize(MAX_FACE);

    progress = new QProgressDialog("Subdiving...", 0, 0, mesh_objFace.size(), main);
    progress->setWindowTitle("Waiting...");
    progress->setWindowModality(Qt::WindowModal);
    progress->show();
    for (int i = 0; i < mesh_objFace.size(); i++)
    {

        if (i % 100 == 0)
        {
            progress->setValue(i);
            QCoreApplication::processEvents();
        }
        std::vector<int> idx;
        for (int j = 0; j < mesh_objFace[i].size(); j++)
            idx.push_back(mesh_objFace[i][j].p[0]);
        idx.push_back(mesh_objFace[i][0].p[0]);

        int a = idx[0];
        int b = idx[1];
        pair<int, int> pa(a,b);
        pair<int, int> pb(b,a);
        map<pair<int, int>, int>::iterator iter = m.find(pa);
        bool sharp;
        if (sharpMap.find(pa) == sharpMap.end())
            sharp = false;
        else
            sharp = true;

        int temp;
        if (iter == m.end())
        {
            int edge_id = heEdgeCounter;

            HalfEdge &h = mesh_halfEdges[edge_id];
            HalfEdge &hSym = mesh_halfEdges[edge_id + 1];

            h.sym = &hSym;
            h.id = edge_id;
            //h.face = &f;
            h.vert = &mesh_vertices[b];
            h.isSharp = sharp;
            //printf("%d:%f %f %f\n", b, h.vert->pos[0], h.vert->pos[1], h.vert->pos[2]);
            mesh_vertices[b].edge = &h;

            hSym.sym = &h;
            hSym.id = edge_id + 1;
            hSym.face = NULL;
            hSym.vert = &mesh_vertices[a];
            hSym.isSharp = sharp;
            //printf("%d:%f %f %f\n", a, hSym.vert->pos[0], hSym.vert->pos[1], hSym.vert->pos[2]);
            mesh_vertices[a].edge = &hSym;

//            vstd[a][b] = edge_id;
//            vstd[b][a] = edge_id + 1;

            m[pa] = edge_id;
            m[pb] = edge_id + 1;
            heEdgeCounter += 2;

            temp = m[pa];
            //printf("s:%d %d\n", a, b);
        }
        else
        {
            //printf("HERE:%d %d\n", a, b);
            temp = m[pa];
            //printf("origin:%d\n", sizeof(m));
            m.erase(iter);
            //printf("after:%d\n", sizeof(m));
        }


        int face_id = heFaceCounter;
        Face &f = mesh_faces[face_id];
        f.id = face_id;
        f.color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        mesh_halfEdges[temp].face = &f;
        f.start_edge = &mesh_halfEdges[temp];
        HalfEdge &startEdge = mesh_halfEdges[temp];
        //printf("startEdge:%d vert:%d\n", vstd[a][b], mesh_halfEdges[vstd[a][b]].vert->id);
        heFaceCounter++;

        for (int j = 1; j < mesh_objFace[i].size(); j++)
        {
            HalfEdge &preEdge = mesh_halfEdges[temp];
            a = idx[j];
            b = idx[j + 1];
            pair<int, int> ppa(a,b);
            pair<int, int> ppb(b,a);
            iter = m.find(ppa);
            bool sharp;
            if (sharpMap.find(ppa) == sharpMap.end())
                sharp = false;
            else
                sharp = true;

            if (iter == m.end())
            {
                int edge_id = heEdgeCounter;

                HalfEdge &h = mesh_halfEdges[edge_id];
                HalfEdge &hSym = mesh_halfEdges[edge_id + 1];

                h.sym = &hSym;
                h.id = edge_id;
                h.vert = &mesh_vertices[b];
                h.isSharp = sharp;
                mesh_vertices[b].edge = &h;

                hSym.sym = &h;
                hSym.id = edge_id + 1;
                hSym.face = NULL;
                hSym.vert = &mesh_vertices[a];
                hSym.isSharp = sharp;
                mesh_vertices[a].edge = &hSym;

//                vstd[a][b] = edge_id;
//                vstd[b][a] = edge_id + 1;

                m[ppa] = edge_id;
                m[ppb] = edge_id + 1;

                heEdgeCounter += 2;

                temp = m[ppa];
                //printf("s:%d %d\n", a, b);
                //printf("%d %d %d %d %d %d\n",edge_id, edge_id + 1, h.id, hSym.id, mesh_halfEdges[edge_id].id, mesh_halfEdges[edge_id + 1].id);
            }
            else
            {
                //printf("HERE:%d %d\n", a, b);
                temp = m[ppa];
                m.erase(iter);
            }
            mesh_halfEdges[temp].face = &f;

            preEdge.next = &mesh_halfEdges[temp];
        }
        // the last edge points to the start edge
        mesh_halfEdges[temp].next = &startEdge;

//        if (i==0)
//        {
//            HalfEdge *t = &startEdge;
//            for (int k = 0; k < 4; k++)
//            {
//                printf("%d->", t->id);
//                t = t->next;
//            }
//            printf("\n");
//        }
    }
    //printf("edge:%d\n", mesh_faces[0].start_edge->id);
//    UpdateBuffer();
//    clearListItem();
//    AddListItem();
    PlanarityTest();
    delete progress;
}

void Mesh::extrude(Face *crtFace, QWidget *main)
{
    int crtFaceId = crtFace->id;
    HalfEdge *startEdge = crtFace->start_edge;
    HalfEdge *currentEdge = startEdge;
    std::vector<HalfEdge*> edges;
    std::vector<int> newVertexIndex;
    do
    {
        edges.push_back(currentEdge);

        int curVertId = currentEdge->vert->id;
        int tSize = mesh_objFace[crtFace->id].size();
        int bufNorId;
        int vid = mesh_objPos.size();
        for (int i = 0; i < tSize; i++)
            if (curVertId == mesh_objFace[crtFace->id][i].p[0])
            {
                bufNorId = mesh_objFace[crtFace->id][i].p[2];
                break;
            }

        vec4 newPos(currentEdge->vert->pos[0] + mesh_objNor[bufNorId].p[0], currentEdge->vert->pos[1] + mesh_objNor[bufNorId].p[1], currentEdge->vert->pos[2] + mesh_objNor[bufNorId].p[2], 1.0f);
        //printf("before normal:%f %f %f\n", mesh_objNor[bufNorId].p[0], mesh_objNor[bufNorId].p[1], mesh_objNor[bufNorId].p[2]);
        mesh_objPos.push_back(newPos);
        newVertexIndex.push_back(vid);
        //mesh_vertices[heVerticesCounter++] = Vertex(newPos);
        //printf("newV: %f %f %f\n", mesh_vertices[heVerticesCounter - 1].pos[0], mesh_vertices[heVerticesCounter - 1].pos[1], mesh_vertices[heVerticesCounter - 1].pos[2]);
        currentEdge = currentEdge->next;
    } while (currentEdge != startEdge);


    int tSize = edges.size();
    for (int i = 0; i < tSize; i++)
    {
        int idx = mesh_objFace.size();
        mesh_objFace.push_back(std::vector<int3>());

        int v0 = edges[i]->vert->id;
        int v1 = newVertexIndex[i];
        int v2 = newVertexIndex[(i - 1 + tSize) % tSize];
        int v3 = edges[(i - 1 + tSize) % tSize]->vert->id;

        int norIdx = mesh_objNor.size();
        vec4 vv0 = mesh_objPos[v1] - mesh_objPos[v0];
        vec4 vv1 = mesh_objPos[v2] - mesh_objPos[v1];
        mesh_objNor.push_back(normalize(cross(vv0, vv1)));

        mesh_objFace[idx].push_back(int3(v0, 0, norIdx));
        mesh_objFace[idx].push_back(int3(v1, 0, norIdx));
        mesh_objFace[idx].push_back(int3(v2, 0, norIdx));
        mesh_objFace[idx].push_back(int3(v3, 0, norIdx));

        //printf("%d %d %d %d\n", v0, v1, v2, v3);
    }
//    int p0 = edges[0]->vert->id;
//    int p1 = edges[1]->vert->id;
//    vec4 pp0 = mesh_objPos[p1] - mesh_objPos[p0];
    //printf("pp:%f %f %f\n", pp0.p[0], pp0.p[1], pp0.p[2]);

    int v0 = newVertexIndex[0];
    int v1 = newVertexIndex[1];
    int v2 = newVertexIndex[2];
    vec4 vv0 = mesh_objPos[v1] - mesh_objPos[v0];
    //printf("vv:%f %f %f\n", vv0.p[0], vv0.p[1], vv0.p[2]);
    vec4 vv1 = mesh_objPos[v2] - mesh_objPos[v1];
    mesh_objNor.push_back(normalize(cross(vv0, vv1)));

    mesh_objFace[crtFaceId].clear();
    int norIdx = mesh_objNor.size() - 1;
    //printf("nor:%f %f %f\n", mesh_objNor[norIdx].p[0], mesh_objNor[norIdx].p[1], mesh_objNor[norIdx].p[2]);
    for (int i = 0; i < tSize; i++)
    {
        int vid = newVertexIndex[i];
        mesh_objFace[crtFaceId].push_back(int3(vid, 0, norIdx));
    }
    meshSubdivisionClear();


    m.clear();
    int objFaceSize = mesh_objFace.size();
    mesh_vertBufferIdx.resize(mesh_objPos.size());
    mesh_faceBufferIdx.resize(objFaceSize);
    for (int i = 0; i < objFaceSize; i++)
    {
        int vertexNum = mesh_objFace[i].size();
        int idxSize = mesh_pos.size();
        std::vector<int> newVector;
        for (int j = 0; j < vertexNum; j++)
        {
            int posIdx = mesh_objFace[i][j].p[0];
            int texIdx = mesh_objFace[i][j].p[1];
            int norIdx = mesh_objFace[i][j].p[2];

            mesh_vertBufferIdx[posIdx].push_back(mesh_pos.size());
            mesh_faceBufferIdx[i].push_back(mesh_pos.size());
            mesh_pos.push_back(mesh_objPos[posIdx]);
            mesh_tex.push_back(mesh_objTex[texIdx]);
            mesh_nor.push_back(mesh_objNor[norIdx]);
            mesh_col.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        for (int j = 0; j < vertexNum - 2; j++)
        {
            mesh_idx.push_back(idxSize);
            mesh_idx.push_back(idxSize + j + 1);
            mesh_idx.push_back(idxSize + j + 2);
        }
    }

    // compute data for half-edge mesh
    mesh_vertices.resize(MAX_VERT);
    // mesh_vertices's indices are equal to mesh_objPos
    heVerticesCounter = mesh_objPos.size();
    for (int i = 0; i < mesh_objPos.size(); i++)
    {
        mesh_vertices[i] = Vertex(mesh_objPos[i].p[0], mesh_objPos[i].p[1], mesh_objPos[i].p[2], 1.0f);
        mesh_vertices[i].id = i;
    }
    memset(vstd, -1, sizeof(vstd));
    using namespace std;


    map<pair<int, int>, int>::iterator iter;
//    pair<int, int> ppp(2, 17);
//    m[ppp] = 1;
//    iter = m.find(ppp);
//    if (iter == m.end())
//        printf("not found\n");
//    else
//        printf("found\n");
    mesh_halfEdges.resize(MAX_HALFEDGE);
    mesh_faces.resize(MAX_FACE);

    progress = new QProgressDialog("Extruding...", 0, 0, mesh_objFace.size(), main);
    progress->setWindowTitle("Waiting...");
    progress->setWindowModality(Qt::WindowModal);
    progress->show();
    for (int i = 0; i < mesh_objFace.size(); i++)
    {

        if (i % 100 == 0)
        {
            progress->setValue(i);
            QCoreApplication::processEvents();
        }
        std::vector<int> idx;
        for (int j = 0; j < mesh_objFace[i].size(); j++)
            idx.push_back(mesh_objFace[i][j].p[0]);
        idx.push_back(mesh_objFace[i][0].p[0]);

        int a = idx[0];
        int b = idx[1];
        pair<int, int> pa(a,b);
        pair<int, int> pb(b,a);
        map<pair<int, int>, int>::iterator iter = m.find(pa);
        int temp;
        if (iter == m.end())
        {
            int edge_id = heEdgeCounter;

            HalfEdge &h = mesh_halfEdges[edge_id];
            HalfEdge &hSym = mesh_halfEdges[edge_id + 1];

            h.sym = &hSym;
            h.id = edge_id;
            //h.face = &f;
            h.vert = &mesh_vertices[b];
            //printf("%d:%f %f %f\n", b, h.vert->pos[0], h.vert->pos[1], h.vert->pos[2]);
            mesh_vertices[b].edge = &h;

            hSym.sym = &h;
            hSym.id = edge_id + 1;
            hSym.face = NULL;
            hSym.vert = &mesh_vertices[a];
            //printf("%d:%f %f %f\n", a, hSym.vert->pos[0], hSym.vert->pos[1], hSym.vert->pos[2]);
            mesh_vertices[a].edge = &hSym;

//            vstd[a][b] = edge_id;
//            vstd[b][a] = edge_id + 1;

            m[pa] = edge_id;
            m[pb] = edge_id + 1;
            heEdgeCounter += 2;

            temp = m[pa];
            //printf("s:%d %d\n", a, b);
        }
        else
        {
            //printf("HERE:%d %d\n", a, b);
            temp = m[pa];
            //printf("origin:%d\n", sizeof(m));
            m.erase(iter);
            //printf("after:%d\n", sizeof(m));
        }


        int face_id = heFaceCounter;
        Face &f = mesh_faces[face_id];
        f.id = face_id;
        f.color = vec4(1.0f, 0.0f, 0.0f, 1.0f);
        mesh_halfEdges[temp].face = &f;
        f.start_edge = &mesh_halfEdges[temp];
        HalfEdge &startEdge = mesh_halfEdges[temp];
        //printf("startEdge:%d vert:%d\n", vstd[a][b], mesh_halfEdges[vstd[a][b]].vert->id);
        heFaceCounter++;

        for (int j = 1; j < mesh_objFace[i].size(); j++)
        {
            HalfEdge &preEdge = mesh_halfEdges[temp];
            a = idx[j];
            b = idx[j + 1];
            pair<int, int> ppa(a,b);
            pair<int, int> ppb(b,a);
            iter = m.find(ppa);

            if (iter == m.end())
            {
                int edge_id = heEdgeCounter;

                HalfEdge &h = mesh_halfEdges[edge_id];
                HalfEdge &hSym = mesh_halfEdges[edge_id + 1];

                h.sym = &hSym;
                h.id = edge_id;
                h.vert = &mesh_vertices[b];
                mesh_vertices[b].edge = &h;

                hSym.sym = &h;
                hSym.id = edge_id + 1;
                hSym.face = NULL;
                hSym.vert = &mesh_vertices[a];
                mesh_vertices[a].edge = &hSym;

//                vstd[a][b] = edge_id;
//                vstd[b][a] = edge_id + 1;

                m[ppa] = edge_id;
                m[ppb] = edge_id + 1;

                heEdgeCounter += 2;

                temp = m[ppa];
                //printf("s:%d %d\n", a, b);
                //printf("%d %d %d %d %d %d\n",edge_id, edge_id + 1, h.id, hSym.id, mesh_halfEdges[edge_id].id, mesh_halfEdges[edge_id + 1].id);
            }
            else
            {
                //printf("HERE:%d %d\n", a, b);
                temp = m[ppa];
                m.erase(iter);
            }
            mesh_halfEdges[temp].face = &f;

            preEdge.next = &mesh_halfEdges[temp];
        }
        // the last edge points to the start edge
        mesh_halfEdges[temp].next = &startEdge;

//        if (i==0)
//        {
//            HalfEdge *t = &startEdge;
//            for (int k = 0; k < 4; k++)
//            {
//                printf("%d->", t->id);
//                t = t->next;
//            }
//            printf("\n");
//        }
    }

    //printf("edge:%d\n", mesh_faces[0].start_edge->id);
    UpdateBuffer();
    clearListItem();
    AddListItem();


    delete progress;
}

void print(const char* str, vec4 a)
{
    printf("%s:", str);
    printf("%f %f %f\n", a.p[0], a.p[1], a.p[2]);
}

void Mesh::PlanarityTest()
{
/*    int fSize = heFaceCounter;
    //printf("fsize:%d\n", fSize);
    for (int i = 0; i < fSize; i++)
    {
        bool isPlane = true;
        std::vector<int> vertexId;
        HalfEdge *startEdge = mesh_faces[i].start_edge;
        HalfEdge *currentEdge = startEdge;
        do
        {
            vertexId.push_back(currentEdge->vert->id);
            currentEdge = currentEdge->next;
        } while (currentEdge != startEdge);

        int vSize = vertexId.size();
        vec4 p0 = mesh_objPos[vertexId[vSize - 2]];
        vec4 p1 = mesh_objPos[vertexId[vSize - 1]];
        vec4 p2 = mesh_objPos[vertexId[0]];
        vec4 v0 = p1 - p0;
        vec4 v1 = p2 - p1;
        vec4 preNor = normalize(cross(v0, v1));

        for (int j = 0; j < vSize; j++)
        {
            p0 = mesh_objPos[vertexId[(j - 1 + vSize) % vSize]];
            p1 = mesh_objPos[vertexId[j]];
            p2 = mesh_objPos[vertexId[(j + 1) % vSize]];

            v0 = p1 - p0;
            v1 = p2 - p1;
            vec4 curNor = normalize(cross(v0, v1));
//            if (i == 4)
//            {
//                printf("%d %d %d\n", vertexId[(j - 1 + vSize) % vSize], vertexId[j], vertexId[(j + 1) % vSize]);
//                print("CUR", curNor);
//                print("PRE", preNor);
//            }
            if (!compare(curNor, preNor))
            {

                isPlane = false;
                break;
            }
        }

        if (!isPlane)
            DivideFace(&mesh_faces[i]);
    }

   // meshClearExceptObj();
    // update GL buffer
    int objFaceSize = mesh_objFace.size();
    mesh_vertBufferIdx.resize(mesh_objPos.size());
    mesh_faceBufferIdx.resize(objFaceSize);
    for (int i = 0; i < objFaceSize; i++)
    {
        int vertexNum = mesh_objFace[i].size();
        //printf("%d\n", heVerticesCounter);
        int idxSize = mesh_pos.size();
        std::vector<int> newVector;
        for (int j = 0; j < vertexNum; j++)
        {
            int posIdx = mesh_objFace[i][j].p[0];
            int texIdx = mesh_objFace[i][j].p[1];
            int norIdx = mesh_objFace[i][j].p[2];
//            if (i == 5)
//                printf("POSIDX:%f %f %f\n", mesh_objPos[posIdx].p[0], mesh_objPos[posIdx].p[1], mesh_objPos[posIdx].p[2]);
            mesh_vertBufferIdx[posIdx].push_back(mesh_pos.size());
            mesh_faceBufferIdx[i].push_back(mesh_pos.size());
//            if (i==0)
//                printf("fb:%d\n", mesh_faceBufferIdx[i].size());
            mesh_pos.push_back(mesh_objPos[posIdx]);
            mesh_tex.push_back(mesh_objTex[texIdx]);
            mesh_nor.push_back(mesh_objNor[norIdx]);
            mesh_col.push_back(vec4(1.0f, 0.0f, 0.0f, 1.0f));
        }

        for (int j = 0; j < vertexNum - 2; j++)
        {
            mesh_idx.push_back(idxSize);
            mesh_idx.push_back(idxSize + j + 1);
            mesh_idx.push_back(idxSize + j + 2);
//            if (i == 5)
//            {
//                printf("%d %d %d\n", idxSize, idxSize + j + 1, idxSize + j + 2);
//            printf("%f %f %f\n %f %f %f\n %f %f %f\n\n", mesh_pos[idxSize].p[0], mesh_pos[idxSize].p[1], mesh_pos[idxSize].p[2],
//                    mesh_pos[idxSize + j + 1].p[0], mesh_pos[idxSize + j + 1].p[1], mesh_pos[idxSize + j + 1].p[2],
//                    mesh_pos[idxSize + j + 2].p[0], mesh_pos[idxSize + j + 2].p[1], mesh_pos[idxSize + j + 2].p[2]);
//            }
        }
    }*///printf("HERE0\n");
    UpdateBuffer();
    clearListItem();
    AddListItem();
}

void Mesh::DivideFace(Face *crtFace)
{
    HalfEdge *startEdge = crtFace->start_edge;
    HalfEdge *currentEdge = startEdge;
    int counter = 0;
    do
    {
        counter++;
        currentEdge = currentEdge->next;
    } while (currentEdge != startEdge);

    if (counter < 4) return;
    //printf("HERE0\n");
    meshClearExceptObj();
    std::map<int, int> normalMap;
    int tSize = mesh_objFace[crtFace->id].size();
    for (int i = 0; i < tSize; i++)
        normalMap[mesh_objFace[crtFace->id][i].p[0]] = mesh_objFace[crtFace->id][i].p[2];
    mesh_objFace[crtFace->id].clear();
    while (1)
    {
        std::vector<HalfEdge*> vEdge;
        startEdge = crtFace->start_edge;
        currentEdge = startEdge;
        do
        {
            vEdge.push_back(currentEdge);
            currentEdge = currentEdge->next;
        } while (currentEdge != startEdge);

        int size = vEdge.size();
        //printf("HERE1\n");
        if (size < 4) break;

        HalfEdge *e0 = vEdge[0];
        HalfEdge *e1 = vEdge[1];
        HalfEdge *e2 = vEdge[2];
        HalfEdge *e3 = vEdge[3];

        Face f1;
        f1.start_edge = e1;
        f1.id = heFaceCounter;
        //f1.color = crtFace->color;
        mesh_faces[heFaceCounter++] = f1;

        Face f0;
        f0.start_edge = e0;
        f0.id = heFaceCounter;
        //f0.color = crtFace->color;
        mesh_faces[heFaceCounter++] = f0;

        HalfEdge b0(&mesh_faces[heFaceCounter - 1], e2->vert, e3, NULL, heEdgeCounter);
        mesh_halfEdges[heEdgeCounter] = b0;
        heEdgeCounter++;
        HalfEdge b1(&mesh_faces[heFaceCounter - 2], e0->vert, e1, &mesh_halfEdges[heEdgeCounter - 1], heEdgeCounter);
        mesh_halfEdges[heEdgeCounter] = b1;
        mesh_halfEdges[heEdgeCounter - 1].sym = &mesh_halfEdges[heEdgeCounter];
        heEdgeCounter++;

        int tLast = mesh_objFace.size();
        mesh_objFace.push_back(std::vector<int3>());


        int norIdx = mesh_objNor.size();
        int v0 = e0->vert->id;
        int v1 = e1->vert->id;
        int v2 = e2->vert->id;
        vec4 vv0 = mesh_objPos[v1] - mesh_objPos[v0];
        //printf("vv:%f %f %f\n", vv0.p[0], vv0.p[1], vv0.p[2]);
        vec4 vv1 = mesh_objPos[v2] - mesh_objPos[v1];
        mesh_objNor.push_back(normalize(cross(vv0, vv1)));

        mesh_objFace[tLast].push_back(int3(e0->vert->id, 0, norIdx));
        mesh_objFace[tLast].push_back(int3(e1->vert->id, 0, norIdx));
        mesh_objFace[tLast].push_back(int3(e2->vert->id, 0, norIdx));



        e2->next = &mesh_halfEdges[heEdgeCounter - 1];
        e0->next = &mesh_halfEdges[heEdgeCounter - 2];

        if (size == 4)
        {
            int norIdx = mesh_objNor.size();
            int v0 = e0->vert->id;
            int v1 = b0.vert->id;
            int v2 = e3->vert->id;
            vec4 vv0 = mesh_objPos[v1] - mesh_objPos[v0];
            //printf("vv:%f %f %f\n", vv0.p[0], vv0.p[1], vv0.p[2]);
            vec4 vv1 = mesh_objPos[v2] - mesh_objPos[v1];
            mesh_objNor.push_back(normalize(cross(vv0, vv1)));

            mesh_objFace[crtFace->id].push_back(int3(e0->vert->id, 0, norIdx));
            mesh_objFace[crtFace->id].push_back(int3(b0.vert->id, 0, norIdx));
            mesh_objFace[crtFace->id].push_back(int3(e3->vert->id, 0, norIdx));

            f0.id = crtFace->id;
            mesh_faces[crtFace->id] = f0;
            heFaceCounter--;
            mesh_halfEdges[heEdgeCounter - 2].face = &mesh_faces[crtFace->id];
        }
    }
//    startEdge = mesh_faces[0].start_edge;
//    currentEdge = startEdge;
//    do
//    {
//        printf("%d\n",currentEdge->vert->id);
//        currentEdge = currentEdge->next;
//    } while (currentEdge != startEdge);

}

void Mesh::CaculateWeight()
{

}
