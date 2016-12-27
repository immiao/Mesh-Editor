#pragma once

#include "drawable.h"
#include <la.h>

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>

class Cylinder : public Drawable
{
public:
    Cylinder(GLWidget277* context);
    virtual void create();
};
