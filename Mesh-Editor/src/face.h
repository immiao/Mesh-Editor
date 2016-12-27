#ifndef FACE_H
#define FACE_H

#include "utils.h"
#include "halfedge.h"


class HalfEdge;
class Face
{
public:
    HalfEdge *start_edge;
    vec4 color;
    int id;
    bool isSharp = false;
    Face();
};

#endif // FACE_H
