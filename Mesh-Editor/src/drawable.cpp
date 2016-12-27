#include "drawable.h"
#include <la.h>

Drawable::Drawable(GLWidget277* context)
    : bufIdx(), bufPos(), bufNor(), bufCol(), bufWeights(), bufJointIndices(),
      idxBound(false), posBound(false), norBound(false), colBound(false),
      weightsBound(false), jointIndicesBound(false),
      context(context)
{
    mode = GL_TRIANGLES;
    isDraw = true;
}

Drawable::~Drawable()
{
    destroy();
}

void Drawable::destroy()
{
    idxBound = false;
    posBound = false;
    norBound = false;
    colBound = false;
    weightsBound = false;
    jointIndicesBound = false;

    context->glDeleteBuffers(1, &bufIdx);
    context->glDeleteBuffers(1, &bufPos);
    context->glDeleteBuffers(1, &bufNor);
    context->glDeleteBuffers(1, &bufCol);
    context->glDeleteBuffers(1, &bufWeights);
    context->glDeleteBuffers(1, &bufJointIndices);
}

void Drawable::SetDraw(bool b)
{
    isDraw = b;
}

GLenum Drawable::drawMode()
{
    // Since we want every three indices in bufIdx to be
    // read to draw our Drawable, we tell that the draw mode
    // of this Drawable is GL_TRIANGLES

    // If we wanted to draw a wireframe, we would return GL_LINES

    return mode;
}

int Drawable::elemCount()
{
    return count;
}

void Drawable::generateIdx()
{
    idxBound = true;
    // Create a VBO on our GPU and store its handle in bufIdx
    context->glGenBuffers(1, &bufIdx);
}

void Drawable::generatePos()
{
    posBound = true;
    // Create a VBO on our GPU and store its handle in bufPos
    context->glGenBuffers(1, &bufPos);
}

void Drawable::generateNor()
{
    norBound = true;
    // Create a VBO on our GPU and store its handle in bufNor
    context->glGenBuffers(1, &bufNor);
}

void Drawable::generateCol()
{
    colBound = true;
    // Create a VBO on our GPU and store its handle in bufCol
    context->glGenBuffers(1, &bufCol);
}

void Drawable::generateWeights()
{
    weightsBound = true;
    context->glGenBuffers(1, &bufWeights);
}

void Drawable::generateJointIndices()
{
    jointIndicesBound = true;
    context->glGenBuffers(1, &bufJointIndices);
}

bool Drawable::bindIdx()
{
    if(idxBound) {
        context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    }
    return idxBound;
}

bool Drawable::bindPos()
{
    if(posBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    }
    return posBound;
}

bool Drawable::bindNor()
{
    if(norBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufNor);
    }
    return norBound;
}

bool Drawable::bindCol()
{
    if(colBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    }
    return colBound;
}

bool Drawable::bindWeights()
{
    if(weightsBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufWeights);
    }
    return weightsBound;
}

bool Drawable::bindJointIndices()
{
    if(jointIndicesBound){
        context->glBindBuffer(GL_ARRAY_BUFFER, bufJointIndices);
    }
    return jointIndicesBound;
}
