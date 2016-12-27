#ifndef DRAWVERTEX_H
#define DRAWVERTEX_H

#include "drawable.h"
#include "vertex.h"
class DrawVertex : public Drawable
{
public:
    Vertex a;
    DrawVertex(GLWidget277 *context);
    virtual void create();
    void SetVertex(Vertex &v);
    void UpdateBuffer();
};

#endif // DRAWVERTEX_H
