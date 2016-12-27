#include "halfedge.h"
#include <iostream>

HalfEdge::HalfEdge()
{
    face = NULL;
    vert = NULL;
    next = NULL;
    sym = NULL;
}

HalfEdge::HalfEdge(Face* f, Vertex* v, HalfEdge *n, HalfEdge *s, int i)
{
    face = f;
    vert = v;
    next = n;
    sym = s;
    id = i;
}
