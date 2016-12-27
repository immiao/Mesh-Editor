#include "skeleton.h"
#include <QDebug>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QString>
#include <QJsonDocument>

Skeleton::Skeleton(GLWidget277 *c):context(c)
{
    root = NULL;
}

void Skeleton::DfsClear(Joint *root)
{
    if (!root) return;
    int size = root->children.size();
    for (int i = 0; i < size; i++)
        DfsClear(root->children[i]);
    delete root;
}

void Skeleton::clear()
{
    int size = vDrawConnection.size();
    for (int i = 0; i < size; i++)
        delete vDrawConnection[i];
    vDrawConnection.clear();

    size = vDrawJoint.size();
    for (int i = 0; i < size; i++)
        delete vDrawJoint[i];
    vDrawJoint.clear();

    DfsClear(root);
    root = NULL;
}

void Skeleton::test(Joint* root)
{
    qDebug() << root->name;
    printf("pos:%f %f %f\n", root->pos.x, root->pos.y, root->pos.z);
    glm::vec3 wPos = root->GetWorldPos();
    printf("worldPos:%f %f %f\n", wPos.x, wPos.y, wPos.z);
    printf("rot:%f %f %f %f\n", root->rot.w, root->rot.x, root->rot.y, root->rot.z);
    printf("childrenSize:%d\n", root->children.size());
    int size = root->children.size();
    for (int i = 0; i < size; i++)
        test(root->children[i]);
}

void Skeleton::construct(QString filename)
{
    QFile file;
    file.setFileName(filename);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();
    QJsonDocument document = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject jsonObject = document.object();
    QJsonObject rootObject = jsonObject.value(QString("root")).toObject();

    root = DfsConstruct(rootObject, NULL);
}

Joint* Skeleton::DfsConstruct(QJsonObject root, Joint* parent)
{
    QString name = root["name"].toString();
    QJsonArray posArray = root["pos"].toArray();
    QJsonArray rotArray = root["rot"].toArray();
    QJsonArray childrenArray = root["children"].toArray();

    glm::vec3 pos(posArray[0].toDouble(), posArray[1].toDouble(), posArray[2].toDouble());
    float sin = glm::sin(rotArray[0].toDouble() * 0.5f);
    glm::quat rot(glm::cos(rotArray[0].toDouble() * 0.5f), sin * rotArray[1].toDouble(), sin * rotArray[2].toDouble(), sin * rotArray[3].toDouble());

    Joint* pJoint = new Joint(name, pos, rot, parent);
    pJoint->bindMatrix = glm::inverse(pJoint->GetOverallTransformation());
    DrawJoint *drawJoint = new DrawJoint(context, pJoint);
    pJoint->drawJointId = vDrawJoint.size();
    vDrawJoint.push_back(drawJoint);
    vBindMatrices.push_back(pJoint->bindMatrix);
    vJointTransforms.push_back(pJoint->GetOverallTransformation());

    std::vector<Joint*> children;

    int size = childrenArray.size();
    for (int i = 0; i < size; i++)
    {
        Joint* childJoint = DfsConstruct(childrenArray[i].toObject(), pJoint);
        DrawConnection *drawConnection = new DrawConnection(context, pJoint, childJoint);
        vDrawConnection.push_back(drawConnection);
        children.push_back(childJoint);
    }
    pJoint->children = children;
    return pJoint;
}

Skeleton::~Skeleton()
{
    clear();
}

void Skeleton::update()
{
    int size = vDrawJoint.size();
    for (int i = 0; i < size; i++)
        vDrawJoint[i]->update();
    size = vDrawConnection.size();
    for (int i = 0; i < size; i++)
        vDrawConnection[i]->update();
}

void Skeleton::DfsUpdate(Joint* root)
{
    vJointTransforms.push_back(root->GetOverallTransformation());
    int size = root->children.size();
    for (int i = 0; i < size; i++)
    {
        DfsUpdate(root->children[i]);
    }
}

void Skeleton::updateTransform()
{
    vJointTransforms.clear();
    DfsUpdate(root);
//    printf("START\n");
//    print(vJointTransforms[10]);
//    printf("\n");
//    print(vBindMatrices[10]);
//    printf("\n");
}
