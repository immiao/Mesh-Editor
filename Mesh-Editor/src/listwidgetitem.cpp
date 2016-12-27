#include "listwidgetitem.h"

ListWidgetItem::ListWidgetItem(int i, const QString &text, QListWidget *parent, int type):QListWidgetItem(text, parent, type)
{
    id = i;
}
