#ifndef DRAWJOINT_H
#define DRAWJOINT_H

#include "drawable.h"
#include "joint.h"
class DrawJoint : public Drawable
{
public:
    DrawJoint(GLWidget277 *context, Joint *p);
    virtual void create();

    Joint *joint;

    int size;
    int idx[37];
    glm::vec4 pos[37];
    glm::vec4 col[37];
    glm::vec4 Color;

    void update();
};

#endif // DRAWJOINT_H
