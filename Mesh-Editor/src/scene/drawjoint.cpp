#include "drawjoint.h"
#include "utils.h"
DrawJoint::DrawJoint(GLWidget277 *context, Joint *p):Drawable(context)
{
    mode = GL_LINE_STRIP;
    joint = p;
    Color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
    size = 37;
    create();
}

void DrawJoint::update()
{
    for (int i = 0; i < size; i++)
        col[i] = Color;
//    for (int i = 0; i < size; i++)
//        print(pos[i]);
    //printf("counter:%d\n", counter);
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), idx, GL_STATIC_DRAW);
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), pos, GL_STATIC_DRAW);
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), col, GL_STATIC_DRAW);
}

void DrawJoint::create()
{
    count = size;

    int counter = 0;
    glm::vec4 temp;
    for (int i = 0; i < 12; i++)
        pos[counter++] = glm::rotate(glm::mat4(1.0f), glm::radians(i * 30.0f), glm::vec3(0, 1, 0)) * glm::vec4(0.5f, 0, 0, 1);
    for (int i = 0; i < 10; i++)
        pos[counter++] = glm::rotate(glm::mat4(1.0f), glm::radians(i * 30.0f), glm::vec3(0, 0, 1)) * glm::vec4(0.5f, 0, 0, 1);
    temp = pos[counter - 1];
    for (int i = 1; i < 12; i++)
        pos[counter++] = glm::rotate(glm::mat4(1.0f), glm::radians(i * 30.0f), glm::vec3(1, 0, 0)) * temp;
    for (int i = 0; i < 4; i++)
        pos[counter++] = glm::rotate(glm::mat4(1.0f), glm::radians(i * 30.0f), glm::vec3(0, 0, 1)) * temp;
    for (int i = 0; i < size; i++)
        idx[i] = i;

    for (int i = 0; i < size; i++)
        col[i] = Color;
//    for (int i = 0; i < size; i++)
//        print(pos[i]);
    //printf("counter:%d\n", counter);
    generateIdx();
    context->glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufIdx);
    context->glBufferData(GL_ELEMENT_ARRAY_BUFFER, size * sizeof(GLuint), idx, GL_STATIC_DRAW);
    generatePos();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufPos);
    context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), pos, GL_STATIC_DRAW);
    generateCol();
    context->glBindBuffer(GL_ARRAY_BUFFER, bufCol);
    context->glBufferData(GL_ARRAY_BUFFER, size * sizeof(glm::vec4), col, GL_STATIC_DRAW);

}
