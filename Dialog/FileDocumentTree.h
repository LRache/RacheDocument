//
// Created by Rache on 2021/10/4.
//

#ifndef RACHEFILE_FILEDOCUMENTTREE_H
#define RACHEFILE_FILEDOCUMENTTREE_H

#include <QTreeWidget>
#include "RacheFile/FileDocument.h"

class FileDocumentTreeItem : public QTreeWidgetItem {
public:
    explicit FileDocumentTreeItem(int subfile, int group, int item) {
        id.subfile = subfile;
        id.group = group;
        id.item = item;
    };
    ItemId id;
};

class FileDocumentTree : public QTreeWidget{
public:
    explicit FileDocumentTree(FileDocument *document);

    QIcon subfileIcon;
    QIcon groupIcon;

    ItemId get_selected();
    void set_selected(ItemId id);
};



#endif //RACHEFILE_FILEDOCUMENTTREE_H
