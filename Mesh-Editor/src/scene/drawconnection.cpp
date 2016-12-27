#include "drawconnection.h"

DrawConnection::DrawConnection(GLWidget277 *context, Joint* j0, Joint* j1):Drawable(context)
{
    mode = GL_LINES;
    joint[0] = j0;
    joint[1] = j1;
    create();
}

void DrawConnection::update()
{
    pos[0] = glm::vec4(joint[0]->GetWorldPos(), 1.0f);
    pos[1] = glm::vec4(joint[1]->GetWorldPos(), 1.0f);
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), idx, GL_STATIC_DRAW);
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec4), pos, GL_STATIC_DRAW);
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec4), col, GL_STATIC_DRAW);
}

void DrawConnection::create()
{
    count = 2;

    pos[0] = glm::vec4(joint[0]->GetWorldPos(), 1.0f);
    pos[1] = glm::vec4(joint[1]->GetWorldPos(), 1.0f);
//print(pos[0]);
//print(pos[1]);
//printf("%f %f %f\n", v[0].p[0], v[0].p[1], v[0].p[2]);
//printf("%f %f %f\n", v[1].p[0], v[1].p[1], v[1].p[2]);
    idx[0] = 0;
    idx[1] = 1;

    col[0] = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);
    col[1] = glm::vec4(1.0f, 1.0f, 0.0f, 1.0f);

    // Create a VBO on our GPU and store its handle in bufIdx
    generateIdx();
    // Tell OpenGL that we want to perform subsequent operations on the VBO referred to by bufIdx
    // and that it will be treated as an element array buffer (since it will contain triangle indices)
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    // Pass the data stored in cyl_idx into the bound buffer, reading a number of bytes equal to
    // SPH_IDX_COUNT multiplied by the size of a GLuint. This data is sent to the GPU to be read by shader programs.
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeof(GLuint), idx, GL_STATIC_DRAW);

    // The next few sets of function calls are basically the same as above, except bufPos and bufNor are
    // array buffers rather than element array buffers, as they store vertex attributes like position.
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec4), pos, GL_STATIC_DRAW);

    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(glm::vec4), col, GL_STATIC_DRAW);
}
