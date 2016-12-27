#include "drawhalfedge.h"
#include "mesh.h"

DrawHalfEdge::DrawHalfEdge(GLWidget277 *context):Drawable(context)
{
    mode = GL_LINES;
    count = 0;
}

void DrawHalfEdge::SetEdge(HalfEdge &e)
{
    edge = e;
}

void DrawHalfEdge::create()
{

    count = 2;

    GLuint vertex_idx[2];

    vec4 v[2];
    v[0].SetVec(edge.sym->vert->pos[0], edge.sym->vert->pos[1], edge.sym->vert->pos[2], edge.sym->vert->pos[3]);
    v[1].SetVec(edge.vert->pos[0], edge.vert->pos[1], edge.vert->pos[2], edge.vert->pos[3]);

//printf("%f %f %f\n", v[0].p[0], v[0].p[1], v[0].p[2]);
//printf("%f %f %f\n", v[1].p[0], v[1].p[1], v[1].p[2]);
    vertex_idx[0] = 0;
    vertex_idx[1] = 1;

    vec4 cc[2];

    cc[0].SetVec(1.0f, 0.0f, 0.0f, 0.0f);
    cc[1].SetVec(1.0f, 1.0f, 0.0f, 0.0f);

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), vertex_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec4), v, GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(vec4), cc, GL_STATIC_DRAW);
}
