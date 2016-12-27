#ifndef JOINTTREEWIDGETITEM_H
#define JOINTTREEWIDGETITEM_H

#include "joint.h"
#include "QTreeWidgetItem"

class JointTreeWidgetItem : public QTreeWidgetItem
{
public:
    Joint* pJoint;
    JointTreeWidgetItem(QStringList, Joint*);
};

#endif // JOINTTREEWIDGETITEM_H
