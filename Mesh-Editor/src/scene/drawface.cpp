#include "drawface.h"
#include "mesh.h"

DrawFace::DrawFace(GLWidget277 *context):Drawable(context)
{
    mode = GL_LINES;
    count = 0;
}

void DrawFace::create()
{

    std::vector<vec4> mesh_pos;
    std::vector<GLuint> mesh_idx;
    std::vector<vec4> mesh_col;
    //printf("%d\n", v.size());
    for (int i = 0; i < v.size(); i++)
    {
        mesh_pos.push_back(vec4(v[i].pos[0], v[i].pos[1], v[i].pos[2], v[i].pos[3]));
        mesh_col.push_back(color);
    }

    for (int i = 0; i < v.size(); i++)
    {
        mesh_idx.push_back(i);
        mesh_idx.push_back((i + 1) % v.size());
    }
    count = mesh_idx.size();
    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh_idx.size() * sizeof(GLuint), &mesh_idx[0], GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_pos.size() * sizeof(vec4), &mesh_pos[0], GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, mesh_col.size() * sizeof(vec4), &mesh_col[0], GL_STATIC_DRAW);
}
