#pragma once

#include "drawable.h"

#include <QOpenGLContext>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <la.h>

class Sphere : public Drawable
{
public:
    Sphere(GLWidget277 *context);
    virtual void create();
};
