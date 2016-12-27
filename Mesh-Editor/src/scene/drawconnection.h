#ifndef DRAWCONNECTION_H
#define DRAWCONNECTION_H

#include "drawable.h"
#include "joint.h"
class DrawConnection : public Drawable
{
public:
    Joint* joint[2];
    glm::vec4 pos[2];
    glm::vec4 col[2];
    GLuint idx[2];
    DrawConnection(GLWidget277 *context, Joint* j0, Joint* j1);
    virtual void create();
    void update();
};

#endif // DRAWCONNECTION_H
