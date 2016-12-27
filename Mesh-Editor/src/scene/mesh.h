#ifndef MESH_H
#define MESH_H

#include "drawable.h"
#include "vertex.h"
#include "halfedge.h"
#include "face.h"
#include <QListWidget>
#include "utils.h"
#include "QProgressDialog"
#include "skeleton.h"

class Mesh: public Drawable
{
public:
    std::vector<vec4> mesh_pos;
    std::vector<vec2> mesh_tex;
    std::vector<vec4> mesh_nor;
    std::vector<GLuint> mesh_idx;
    std::vector<vec4> mesh_col;

    std::vector<float> mesh_weights;
    std::vector<int> mesh_jointIndices;

    std::vector<std::vector<int>> mesh_vertBufferIdx; // used to modify the actual data in buffer
    std::vector<std::vector<int>> mesh_faceBufferIdx;

    std::vector<Vertex> mesh_vertices;
    std::vector<HalfEdge> mesh_halfEdges;
    std::vector<Face> mesh_faces;
    std::vector<vec4> mesh_objPos;
    std::vector<vec2> mesh_objTex;
    std::vector<vec4> mesh_objNor;
    std::vector<std::vector<int3>> mesh_objFace;
    std::map<std::pair<int, int>, bool> sharpMap;
    std::map<int, bool> VertexSharpMap;
    QListWidget *vListWidget;
    QListWidget *fListWidget;
    QListWidget *eListWidget;
    Mesh(GLWidget277 *context);
    virtual void create();
    void createObjMesh(const char *);
    void meshClear();
    void clearListItem();
    void UpdateBuffer();
    void meshClearExceptObj();
    void AddingVertexUpdate(HalfEdge *crtEdge);
    void AddingFaceUpdate(Face *face);
    void AddListItem();
    void subdivision(QWidget *main);
    void meshSubdivisionClear();
    void extrude(Face *crtFace, QWidget *main);
    void PlanarityTest();
    void DivideFace(Face *crtFace);

    int heVerticesCounter;
    int heEdgeCounter;
    int heFaceCounter;
    float Sharpness;

    int mid[3000000];
    int cen[3000000];
    std::map<std::pair<int, int>, int> m;

    QProgressDialog *progress;

    void CaculateWeight();

    void skinning(Skeleton &skeleton);
};

#endif // MESH_H
