#ifndef LISTWIDGETITEM_H
#define LISTWIDGETITEM_H

#include <QListWidgetItem>

class ListWidgetItem : public QListWidgetItem
{
public:
    int id;
    ListWidgetItem(int i, const QString & text, QListWidget * parent = 0, int type = Type);
};

#endif // LISTWIDGETITEM_H
