#ifndef MYGL_H
#define MYGL_H

#include <glwidget277.h>
#include <utils.h>
#include <shaderprogram.h>
#include <scene/cylinder.h>
#include <scene/sphere.h>
#include "camera.h"
#include "scene/mesh.h"
#include "scene/drawface.h"
#include "scene/drawhalfedge.h"
#include "scene/drawvertex.h"
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QListWidget>

#include <skeleton.h>

class MyGL
    : public GLWidget277
{
private:
    Cylinder geom_cylinder;// The instance of a unit cylinder we can use to render any cylinder
    Sphere geom_sphere;// The instance of a unit sphere we can use to render any sphere

    ShaderProgram prog_lambert;// A shader program that uses lambertian reflection
    ShaderProgram prog_flat;// A shader program that uses "flat" reflection (no shadowing at all)

    ShaderProgram prog_skin;

    GLuint vao; // A handle for our vertex array object. This will store the VBOs created in our geometry classes.
                // Don't worry too much about this. Just know it is necessary in order to render geometry.

    Camera gl_camera;
    //DrawJoint testJoint;
    /// Timer linked to timerUpdate(). Fires approx. 60 times per second
    QTimer timer;


public:
    HalfEdge *currentEdge;
    Vertex *currentVertex;
    Face *currentFace;
    Mesh ObjMesh;
    //bool isCubeMesh;
    Skeleton skeleton;
    DrawFace geom_face;
    DrawVertex geom_vertex;
    DrawHalfEdge geom_edge;
    explicit MyGL(QWidget *parent = 0);
    ~MyGL();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
    void HighLightVertex(Vertex &v);
    void HighLightFace(Face &f);
    void HighLightEdge(HalfEdge &e);

    float timeCount;
    int renderMode;
    float t;
    float interval = 0.016f;
    bool isDrawSkeleton;
    bool isSkin;
protected:
    void keyPressEvent(QKeyEvent *e);

private slots:
    /// Slot that gets called ~60 times per second
    void timerUpdate();
};


#endif // MYGL_H
