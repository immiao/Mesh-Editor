#ifndef DRAWHALFEDGE_H
#define DRAWHALFEDGE_H

#include "drawable.h"
#include "halfedge.h"

class DrawHalfEdge : public Drawable
{
public:
    HalfEdge edge;
    DrawHalfEdge(GLWidget277 *context);
    virtual void create();
    void SetEdge(HalfEdge &e);
};

#endif // DRAWHALFEDGE_H
