#ifndef DRAWFACE_H
#define DRAWFACE_H

#include "drawable.h"
#include "vertex.h"
#include "utils.h"

class DrawFace : public Drawable
{
public:
    std::vector<Vertex> v;
    vec4 color;
    DrawFace(GLWidget277 *context);
    virtual void create();
};

#endif // DRAWFACE_H
