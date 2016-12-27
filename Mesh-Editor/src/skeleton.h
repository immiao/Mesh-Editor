#ifndef SKELETON_H
#define SKELETON_H

#include "joint.h"
#include "scene/drawjoint.h"
#include "scene/drawconnection.h"

class Skeleton
{
public:
    Skeleton(GLWidget277 *c);
    ~Skeleton();
    Joint *root;

    std::vector<DrawJoint*> vDrawJoint;
    std::vector<DrawConnection*> vDrawConnection;
    std::vector<glm::mat4> vBindMatrices;
    std::vector<glm::mat4> vJointTransforms;
    void clear();
    void test(Joint* root);
    void DfsClear(Joint *root);
    void construct(QString filename);
    void update();
    void DfsUpdate(Joint* root);
    void updateTransform();

    Joint* DfsConstruct(QJsonObject root, Joint* parent);
    GLWidget277 *context;
};

#endif // SKELETON_H
