//
// Created by Rache on 2021/10/4.
//

#include "FileDocumentTree.h"
#include "globals.h"
#include <QDebug>

FileDocumentTree::FileDocumentTree(FileDocument *document) {
    subfileIcon = QIcon(RESOURCE_PATH(icon/document.png));
    groupIcon   = QIcon(RESOURCE_PATH(icon/group.png));

    setObjectName("FileDocumentTree");
    setColumnCount(3);
    QStringList labels;
    labels << tr("名称") << tr("类型") << tr("内容");
    setHeaderLabels(labels);

    int subfileId = 0;
    int groupId;
    int itemId;
    for (auto subfile : document->get_subs()) {
        if (subfile == nullptr) continue;

        auto *subfileTreeItem = new FileDocumentTreeItem(subfileId++, -1, -1);
        subfileTreeItem->setIcon(0, subfileIcon);
        subfileTreeItem->setText(0, QString::fromStdString(subfile->name));
        subfileTreeItem->setText(1, tr("子文件"));
        addTopLevelItem(subfileTreeItem);

        groupId = 0;
        for (auto group : subfile->get_groups()) {
            if (group == nullptr) continue;

            auto *groupTreeItem = new FileDocumentTreeItem(subfileId, groupId++, -1);
            groupTreeItem->setIcon(0, groupIcon);
            groupTreeItem->setText(0, QString::fromStdString(group->name));
            groupTreeItem->setText(1, tr("组"));
            subfileTreeItem->addChild(groupTreeItem);

            itemId = 0;
            for (auto item : group->get_items()) {
                if (item == nullptr) continue;

                auto *itemTreeItem = new FileDocumentTreeItem(subfileId, groupId, itemId++);
                // itemTreeItem->setIcon(0, itemIcon);
                itemTreeItem->setText(0, QString::fromStdString(item->name));
                itemTreeItem->setText(1, tr("项"));
                itemTreeItem->setText(2, QString::fromStdString(item->text));
                groupTreeItem->addChild(itemTreeItem);
            }
        }
    }
}

ItemId FileDocumentTree::get_selected() {
    auto *i = (FileDocumentTreeItem *)currentItem();
    if (i == nullptr) {
        return {};
    }
    return i->id;
}

void FileDocumentTree::set_selected(ItemId id) {
    int end1 = topLevelItemCount();
    for (int i = 0; i < end1; ++i) {
        auto subfileItem = (FileDocumentTreeItem*) topLevelItem(i);
        if (id.subfile == subfileItem->id.subfile) {
            subfileItem->setExpanded(true);
            if (id.group != -1) {
                int end2 = subfileItem->childCount();
                for (int j = 0; j < end2; ++j) {
                    auto groupItem = (FileDocumentTreeItem*) subfileItem->child(j);
                    if (id.group == groupItem->id.group) {
                        groupItem->setExpanded(true);
                        if (id.group != -1)  {
                            int end3 = groupItem->childCount();
                            for (int k = 0; k < end3; ++k) {
                                auto itemItem = (FileDocumentTreeItem*) groupItem->child(k);
                                if (id.item == itemItem->id.item) {
                                    itemItem->setSelected(true);
                                    return;
                                }
                            }
                            return;
                        } else {
                            groupItem->setSelected(true);
                            return;
                        }
                    }
                }
            } else {
                subfileItem->setSelected(true);
                return;
            }
            return;
        }
    }
}
