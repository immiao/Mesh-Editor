#include "joint.h"
#include <iostream>

Joint::Joint()//:rot(0.0f, 0.0f, 0.0f, 0.0f)
{

}

Joint::Joint(QString n, glm::vec3 p, glm::quat r, Joint* baba)
{
    name = n;
    pos = p;
    rot = r;
    parent = baba;
}

glm::mat4 Joint::GetLocalTransformation()
{
    glm::mat4 trans = glm::translate(glm::mat4(), pos);
    glm::mat4 rotation = glm::toMat4(rot);
//    std::cout << glm::to_string(trans) << std::endl;
//    print(trans);
//    print(rotation);
//    print(trans * rotation);
//    std::cout << glm::to_string(rotation) << std::endl;
//    std::cout << glm::to_string(trans * rotation) << std::endl;
    return trans * rotation;
}

glm::mat4 Joint::GetOverallTransformation()
{
    if (parent)
        return parent->GetOverallTransformation() * GetLocalTransformation();
    return GetLocalTransformation();
}

glm::vec3 Joint::GetWorldPos()
{
    glm::mat4 globalMatrix = GetOverallTransformation();

    return glm::vec3(globalMatrix[3][0], globalMatrix[3][1], globalMatrix[3][2]);
}
