#include "drawvertex.h"
#include "mesh.h"

DrawVertex::DrawVertex(GLWidget277 *context):Drawable(context)
{
    mode = GL_POINTS;
    count = 0;
}

void DrawVertex::create()
{
    count = 1;

    GLuint vertex_idx = 0;

    vec4 v;
    v.SetVec(a.pos[0], a.pos[1], a.pos[2], a.pos[3]);

    vec4 cc;
    cc.SetVec(1.0f, 1.0f, 1.0f, 1.0f);

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1 * sizeof(GLuint), &vertex_idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(vec4), &v, GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(vec4), &cc, GL_STATIC_DRAW);
}

void DrawVertex::SetVertex(Vertex &v)
{
    a = v;
}

void DrawVertex::UpdateBuffer()
{
    vec4 v;
    v.SetVec(a.pos[0], a.pos[1], a.pos[2], a.pos[3]);

    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, 1 * sizeof(vec4), &v, GL_STATIC_DRAW);
}
