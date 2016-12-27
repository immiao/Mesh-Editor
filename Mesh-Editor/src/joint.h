#ifndef JOINT_H
#define JOINT_H
#include <vector>
#include <QString>
#include <glm/glm.hpp>
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/ext.hpp"
#include "utils.h"

class Joint
{
public:
    Joint();
    Joint(QString n, glm::vec3 p, glm::quat r, Joint* baba);
    QString name;
    Joint *parent;
    std::vector<Joint*> children;
    glm::vec3 pos;
    glm::quat rot;
    glm::mat4 bindMatrix;

    glm::mat4 GetLocalTransformation();
    glm::mat4 GetOverallTransformation();
    glm::vec3 GetWorldPos();

    int drawJointId;
    std::vector<int> drawConnectionId;
};

#endif // JOINT_H
