#include "vertex.h"

Vertex::Vertex(float x, float y, float z, float w)
{
    pos[0] = x;
    pos[1] = y;
    pos[2] = z;
    pos[3] = w;
}

Vertex::Vertex(vec4 v)
{
    pos[0] = v.p[0];
    pos[1] = v.p[1];
    pos[2] = v.p[2];
    pos[3] = v.p[3];
}

Vertex &Vertex::operator=(vec4 v)
{
    pos[0] = v.p[0];
    pos[1] = v.p[1];
    pos[2] = v.p[2];
    pos[3] = v.p[3];
    return *this;
}
