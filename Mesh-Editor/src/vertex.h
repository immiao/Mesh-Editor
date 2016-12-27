#ifndef VERTEX_H
#define VERTEX_H

#include "halfedge.h"
#include "utils.h"
#include "joint.h"

class HalfEdge;
class Vertex
{
public:
    Joint *influencedJoint[2];
    float pos[4];
    HalfEdge *edge;
    int id;
    bool isSharp = false;
    Vertex(float x, float y, float z, float w);
    Vertex(vec4 v);
    Vertex(){}
    Vertex &operator=(vec4 v);
};

#endif // VERTEX_H
