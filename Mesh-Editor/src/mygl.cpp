#include "mygl.h"
#include <la.h>

#include <iostream>
#include <QApplication>
#include <QKeyEvent>
#include "utils.h"

MyGL::MyGL(QWidget *parent)
    : GLWidget277(parent), geom_face(this), geom_vertex(this), geom_edge(this), ObjMesh(this), skeleton(this),
      geom_cylinder(this), geom_sphere(this),
      prog_lambert(this), prog_flat(this), prog_skin(this),
      gl_camera()
{
    // Connect the timer to a function so that when the timer ticks the function is executed
    connect(&timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    // Tell the timer to redraw 60 times per second
    timer.start(16);
    currentEdge = NULL;
    currentFace = NULL;
    currentVertex = NULL;

    //isCubeMesh = true;
    timeCount = 0.0f;
    renderMode = 0;
    //setFixedWidth(800);
    setAttribute(Qt::WA_ShowWithoutActivating);

    isDrawSkeleton = false;
    isSkin = false;
}

MyGL::~MyGL()
{
    makeCurrent();
    glDeleteVertexArrays(1, &vao);
    geom_cylinder.destroy();
    geom_sphere.destroy();
    geom_face.destroy();
    geom_vertex.destroy();
    geom_edge.destroy();
    ObjMesh.destroy();
}

void MyGL::initializeGL()
{
    // Create an OpenGL context using Qt's QOpenGLFunctions_3_2_Core class
    // If you were programming in a non-Qt context you might use GLEW (GL Extension Wrangler)instead
    initializeOpenGLFunctions();
    // Print out some information about the current OpenGL context
    debugContextVersion();

    // Set a few settings/modes in OpenGL rendering
    //glDisable(GL_BACKFACECULLING);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    // Set the size with which points should be rendered
    glPointSize(5);
    // Set the color with which the screen is filled at the start of each render call.
    glClearColor(0.5, 0.5, 0.5, 1);

    printGLErrorLog();

    // Create a Vertex Attribute Object
    glGenVertexArrays(1, &vao);

    //Create the instances of Cylinder and Sphere.
    geom_cylinder.create();

    geom_sphere.create();
//    CubeMesh.createCubeMesh();
//    CubeMesh.create();
    ObjMesh.createObjMesh("../assignment_package/cube.obj");
    geom_vertex.SetDraw(false);
    geom_face.SetDraw(false);
    geom_edge.SetDraw(false);
//    Vertex a(-0.5f, -0.5f, 0.5f, 1.0f);
//    Vertex b(0.5f, -0.5f, 0.5f, 1.0f);
//    Vertex c(-0.5f, 0.5f, 0.5f, 1.0f);
//    Vertex d(0.5f, 0.5f, 0.5f, 1.0f);

//    geom_face.a = a;
//    geom_face.b = b;
//    geom_face.c = c;
//    geom_face.d = d;
//    geom_face.create();

    // Create and set up the diffuse shader
    prog_lambert.create(":/glsl/lambert.vert.glsl", ":/glsl/lambert.frag.glsl");
    // Create and set up the flat lighting shader
    prog_flat.create(":/glsl/flat.vert.glsl", ":/glsl/flat.frag.glsl");

    prog_skin.create(":/glsl/skin.vert.glsl", ":/glsl/skin.frag.glsl");
    //testJoint.create();
    // Set a color with which to draw geometry since you won't have one
    // defined until you implement the Node classes.
    // This makes your geometry render green.
    prog_lambert.setGeometryColor(glm::vec4(0,1,0,1));

    // We have to have a VAO bound in OpenGL 3.2 Core. But if we're not
    // using multiple VAOs, we can just bind one once.
//    vao.bind();
    glBindVertexArray(vao);
}

void MyGL::resizeGL(int w, int h)
{
    //This code sets the concatenated view and perspective projection matrices used for
    //our scene's camera view.
    gl_camera = Camera(w, h);
    glm::mat4 viewproj = gl_camera.getViewProj();

    // Upload the view-projection matrix to our shaders (i.e. onto the graphics card)

    prog_lambert.setViewProjMatrix(viewproj);
    prog_flat.setViewProjMatrix(viewproj);
    prog_skin.setViewProjMatrix(viewproj);

    printGLErrorLog();
}

//This function is called by Qt any time your GL window is supposed to update
//For example, when the function updateGL is called, paintGL is called implicitly.
//DO NOT CONSTRUCT YOUR SCENE GRAPH IN THIS FUNCTION!
void MyGL::paintGL()
{
    // Clear the screen so that we only see newly drawn images
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    prog_flat.setViewProjMatrix(gl_camera.getViewProj());
    prog_lambert.setViewProjMatrix(gl_camera.getViewProj());
    prog_skin.setViewProjMatrix(gl_camera.getViewProj());

#define NOPE
#ifdef NOPE
    //Create a model matrix. This one scales the sphere uniformly by 3, then translates it by <-2,0,0>.
    //Note that we have to transpose the model matrix before passing it to the shader
    //This is because OpenGL expects column-major matrices, but you've
    //implemented row-major matrices.
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(0,0,0));
    //Send the geometry's transformation matrix to the shader
//    prog_lambert.setModelMatrix(model);
    prog_flat.setModelMatrix(model);
//    prog_skin.setModelMatrix(model);
    //Draw the example sphere using our lambert shader
    //prog_lambert.draw(geom_sphere);

    prog_lambert.setModelMatrix(model);
    prog_lambert.setTime(timeCount);
    prog_lambert.setRenderMode(renderMode);
    if (isSkin)
    {
        prog_skin.setModelMatrix(model);
        prog_skin.setTime(timeCount);
        prog_skin.setBindMatrices(skeleton.vBindMatrices);
//        int size = skeleton.vBindMatrices.size();
//        for (int i = 0; i < size; i++)
//        {
//            print(skeleton.vBindMatrices[i]);
//            printf("\n");
//        }
        skeleton.updateTransform();
        prog_skin.setJointTransforms(skeleton.vJointTransforms);
        prog_skin.setRenderMode(renderMode);
        prog_skin.draw(ObjMesh);
    }
    else
        prog_lambert.draw(ObjMesh);
    glDisable(GL_DEPTH_TEST);
    prog_flat.draw(geom_face);
    prog_flat.draw(geom_vertex);
    prog_flat.draw(geom_edge);

    if (isDrawSkeleton)
    {
        int size;
        size = skeleton.vDrawJoint.size();
        //printf("size:%d\n", size);
        for (int i = 0; i < size; i++)
        {
            model = skeleton.vDrawJoint[i]->joint->GetOverallTransformation();
            //model = translation;
            //print(skeleton.vDrawJoint[i]->centerPos);
            prog_flat.setModelMatrix(model);
            prog_flat.draw(*skeleton.vDrawJoint[i]);
        }

        prog_flat.setModelMatrix(glm::mat4(1.0f));
        size = skeleton.vDrawConnection.size();
        //printf("size:%d\n", size);
        for (int i = 0; i < size; i++)
        {
            prog_flat.draw(*skeleton.vDrawConnection[i]);
        }
    }
    //prog_flat.draw(testJoint);
    glEnable(GL_DEPTH_TEST);
    //Now do the same to render the cylinder
    //We've rotated it -45 degrees on the Z axis, then translated it to the point <2,2,0>
    model = glm::translate(glm::mat4(1.0f), glm::vec3(2,2,0)) * glm::rotate(glm::mat4(1.0f), glm::radians(-45.0f), glm::vec3(0,0,1));
//    prog_lambert.setModelMatrix(model);
    //prog_lambert.draw(geom_cylinder);
    //printf("GLGLGL\n");
#endif
}


void MyGL::keyPressEvent(QKeyEvent *e)
{

    float amount = 2.0f;
    if(e->modifiers() & Qt::ShiftModifier){
        amount = 10.0f;
    }
    // http://doc.qt.io/qt-5/qt.html#Key-enum
    // This could all be much more efficient if a switch
    // statement were used, but I really dislike their
    // syntax so I chose to be lazy and use a long
    // chain of if statements instead
    if (e->key() == Qt::Key_Escape) {
        QApplication::quit();
    } else if (e->key() == Qt::Key_Right) {
        gl_camera.RotateAboutUp(-amount);
    } else if (e->key() == Qt::Key_Left) {
        gl_camera.RotateAboutUp(amount);
    } else if (e->key() == Qt::Key_Up) {
        gl_camera.RotateAboutRight(-amount);
    } else if (e->key() == Qt::Key_Down) {
        gl_camera.RotateAboutRight(amount);
    } else if (e->key() == Qt::Key_1) {
        gl_camera.fovy += amount;
    } else if (e->key() == Qt::Key_2) {
        gl_camera.fovy -= amount;
    } else if (e->key() == Qt::Key_W) {
        gl_camera.TranslateAlongLook(amount);
    } else if (e->key() == Qt::Key_S) {
        gl_camera.TranslateAlongLook(-amount);
    } else if (e->key() == Qt::Key_D) {
        gl_camera.TranslateAlongRight(amount);
    } else if (e->key() == Qt::Key_A) {
        gl_camera.TranslateAlongRight(-amount);
    } else if (e->key() == Qt::Key_Q) {
        gl_camera.TranslateAlongUp(-amount);
    } else if (e->key() == Qt::Key_E) {
        gl_camera.TranslateAlongUp(amount);
    } else if (e->key() == Qt::Key_N && currentEdge != NULL) {
        currentEdge = currentEdge->next;
        //printf("ID:%d\n", currentEdge->vert->id);
        HighLightEdge(*currentEdge);
    } else if (e->key() == Qt::Key_M && currentEdge != NULL) {
        currentEdge = currentEdge->sym;
        HighLightEdge(*currentEdge);
    } else if (e->key() == Qt::Key_F && currentEdge != NULL) {
        currentFace = currentEdge->face;
        HighLightFace(*currentFace);
    } else if (e->key() == Qt::Key_V && currentEdge != NULL) {
        currentVertex = currentEdge->vert;
        HighLightVertex(*currentVertex);
    } else if ((e->modifiers() & Qt::ShiftModifier) && e->key() == Qt::Key_H && currentFace != NULL) {
        currentEdge = currentFace->start_edge;
        HighLightEdge(*currentEdge);
    } else if (e->key() == Qt::Key_H && currentVertex != NULL) {
        currentEdge = currentVertex->edge;
        HighLightEdge(*currentEdge);
    } else if (e->key() == Qt::Key_R) {
        renderMode ^= 1;
    }
    gl_camera.RecomputeAttributes();
    update();  // Calls paintGL, among other things
}

void MyGL::timerUpdate()
{
    // This function is called roughly 60 times per second.
    // Use it to perform any repeated actions you want to do,
    // such as
    t += interval;
    if (t > 2.0f)
    {
        setFocus();
        t = 0.0f;
    }
    timeCount += 0.08f;
    //printf("%f\n", timeCount);

    update();
}

void MyGL::HighLightVertex(Vertex &v)
{
    currentVertex = &v;
    geom_vertex.destroy();
    geom_vertex.SetVertex(v);
    geom_vertex.create();
    geom_vertex.SetDraw(true);
    update();
}

void MyGL::HighLightFace(Face &f)
{
    currentFace = &f;
    HalfEdge *startEdge = f.start_edge;
    HalfEdge *currentEdge = startEdge;
    geom_face.v.clear();
    do
    {
        //printf("vert:%d\n", currentEdge->vert->id);
        geom_face.v.push_back(*currentEdge->vert);
        currentEdge = currentEdge->next;
    } while (currentEdge != startEdge);
    geom_face.destroy();
    geom_face.create();
    geom_face.SetDraw(true);
    update();
    //glDisable(GL_DEPTH_TEST);
    //prog_lambert.draw(geom_face);
    //glEnable(GL_DEPTH_TEST);
}

void MyGL::HighLightEdge(HalfEdge &e)
{
    currentEdge = &e;
    printf("currentEdge:%d\n", e.id);
    geom_edge.destroy();
    geom_edge.SetEdge(e);
    geom_edge.create();
    geom_edge.SetDraw(true);
    update();
}
