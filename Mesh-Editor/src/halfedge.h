#ifndef HALFEDGE_H
#define HALFEDGE_H

#include "face.h"
#include "vertex.h"
class Vertex;
class Face;
class HalfEdge
{
public:
    Face *face;
    Vertex *vert;
    HalfEdge *next;
    HalfEdge *sym;
    int id;
    bool isSharp = false;

    HalfEdge();
    HalfEdge(Face* f, Vertex* v, HalfEdge *n, HalfEdge *s, int i);
};

#endif // HALFEDGE_H
