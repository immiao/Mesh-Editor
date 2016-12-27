#include "la.h"

QMatrix4x4 la::to_qmat(const glm::mat4 &m)
{
    return QMatrix4x4(glm::value_ptr(glm::transpose(m)));
}


QVector4D la::to_qvec(const glm::vec4 &v){
    return QVector4D(v[0], v[1], v[2], v[3]);
}
