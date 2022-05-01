//
// Created by Rache on 2021/10/2.
//

#include "ViewerWidgetSet.h"
#include "globals.h"
#include "qdebug.h"

#include <QApplication>

ViewerItemWidget::ViewerItemWidget(FileItemObject *fileItemObject, ViewerGroupWidget *p) {
    groupWidget = p;
    item = fileItemObject;

    nameLabel = new QLabel();
    textLabel = new ItemTextLabel(this);

    editButton = new ItemToolButton(QObject::tr("编辑"), EditIcon, this);
    copyButton = new ItemToolButton(QObject::tr("复制"), CopyIcon, this);
    pasteButton = new ItemToolButton(QObject::tr("粘贴"), PasteIcon, this);
    removeButton = new ItemToolButton(QObject::tr("删除"), DeleteIcon, this);
    resourceButton = new ItemToolButton(QObject::tr("资源"), ResourceIcon, this);

    nameLabel->setObjectName("ViewerItemNameLabel");
    textLabel->textLabel->setObjectName("ViewerItemTextLabel");

    editButton->setProperty("ViewerProperty", "Item");
    copyButton->setProperty("ViewerProperty", "Item");
    pasteButton->setProperty("ViewerProperty", "Item");
    removeButton->setProperty("ViewerProperty", "Item");
    resourceButton->setProperty("ViewerProperty", "Item");
    setObjectName("ViewerItemWidget");

    auto *boxLayout = new QHBoxLayout;
    boxLayout->addWidget(nameLabel);
    boxLayout->addWidget(textLabel);
    boxLayout->addWidget(editButton);
    boxLayout->addWidget(copyButton);
    boxLayout->addWidget(pasteButton);
    boxLayout->addWidget(removeButton);
    boxLayout->addWidget(resourceButton);
    setLayout(boxLayout);

    if (!ShowToolButton) {
        editButton->hide();
        copyButton->hide();
        pasteButton->hide();
        removeButton->hide();
    }

    fresh();
}

void ViewerItemWidget::fresh() {
    nameLabel->setText(Q_STR(item->name));
    textLabel->setText(Q_STR(item->text));
    textLabel->setToolTip(tr("<b>来源 </b>%1<br><b>日期 </b>%2.%3.%4").arg(Q_STR(item->source),
                                                                       QString::number(item->year),
                                                                       QString::number(item->month),
                                                                       QString::number(item->day)));
    textLabel->setWordWrap(true);
    nameLabel->setFixedWidth(70);
    nameLabel->setWordWrap(true);

    switch (item->type) {
        case FileItemObject::NORMAL:
            setStyleSheet(NormalStyleSheet);
            break;
        case FileItemObject::DOUBT:
            setStyleSheet(DoubtStyleSheet);
            break;
        case FileItemObject::ATTENTION:
            setStyleSheet(AttentionStyleSheet);
            break;
        case FileItemObject::WARNING:
            setStyleSheet(WarningStyleSheet);
            break;
    }

    if (item->resName.empty()) {
        resourceButton->hide();
    } else {
        resourceButton->show();
    }
}

void ViewerItemWidget::moveEvent(QMoveEvent *event) {
    QWidget::moveEvent(event);
}

ItemToolButton::ItemToolButton(const QString &t, const QIcon &i, ViewerItemWidget *p) {
    setToolTip(t);
    setIcon(i);
    itemWidget = p;
}

ViewerGroupWidget::ViewerGroupWidget(FileItemGroup *fileItemGroup) {
    nameLabel = new QLabel(Q_STR(fileItemGroup->name));
    nameLabel->setObjectName("GroupNameLabel");

    addButton = new GroupToolButton(tr("添加一项"), AddIcon, this);
    removeButton = new GroupToolButton(tr("删除"), DeleteIcon, this);
    editButton = new GroupToolButton(tr("编辑"), EditIcon, this);
    hideButton = new GroupToolButton(tr("收起"), UpIcon, this);

    addButton->setProperty("ViewerProperty", "Group");
    removeButton->setProperty("ViewerProperty", "Group");
    editButton->setProperty("ViewerProperty", "Group");
    hideButton->setProperty("ViewerProperty", "Group");

    auto *groupNameLayout = new QHBoxLayout;
    groupNameLayout->addWidget(nameLabel);
    groupNameLayout->addWidget(addButton);
    groupNameLayout->addWidget(removeButton);
    groupNameLayout->addWidget(editButton);
    groupNameLayout->addWidget(hideButton);
    groupNameLayout->addStretch(1);

    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(groupNameLayout);
    for (auto item : fileItemGroup->get_items()) {
        auto itemWidget = new ViewerItemWidget(item, this);
        mainLayout->addWidget(itemWidget);
        items.emplace_back(itemWidget);
        QApplication::processEvents();
    }

    setLayout(mainLayout);

    group = fileItemGroup;
    if (group->hide) {
        hide_items();
        hideButton->setToolTip(tr("展开"));
        hideButton->setIcon(DownIcon);
    }
}

ViewerGroupWidget::~ViewerGroupWidget() {
    for (auto i : items) {
        delete i;
    }
}

ViewerItemWidget *ViewerGroupWidget::add_item(FileItemObject *i) {
    group->add_item(i);
    auto itemWidget = new ViewerItemWidget(i, this);
    mainLayout->addWidget(itemWidget);
    items.emplace_back(itemWidget);
    return itemWidget;
}

void ViewerGroupWidget::remove_item(ViewerItemWidget *i) {
    items[i->id] = nullptr;
    mainLayout->removeWidget(i);
    group->remove_item(i->item);
    delete i->item;
    delete i;
}

void ViewerGroupWidget::hide_group() {
    if (group->hide) {
        group->hide = false;
        show_items();
        hideButton->setIcon(UpIcon);
        hideButton->setToolTip(tr("收起"));
    } else {
        group->hide = true;
        hide_items();
        hideButton->setIcon(DownIcon);
        hideButton->setToolTip(tr("展开"));
    }
}

void ViewerGroupWidget::show_items() {
    for (auto i : items) {
        i->show();
    }
}

void ViewerGroupWidget::hide_items() {
    for (auto i : items) {
        i->hide();
    }
}

GroupToolButton::GroupToolButton(const QString &t, const QIcon &i, ViewerGroupWidget *p) {
    setToolTip(t);
    setIcon(i);
    groupWidget = p;
}

ViewerMainWidget::ViewerMainWidget(SubFile *subFile) {
    addButton = new QToolButton;
    orderButton = new QToolButton;

    addButton->setIcon(AddIcon);
    orderButton->setIcon(ListIcon);

    addButton->setToolTip(tr("添加"));
    orderButton->setToolTip(tr("排序"));

    addButton->setProperty("ViewerProperty", "SubFile");
    orderButton->setProperty("ViewerProperty", "SubFile");
}

void ViewerMainWidget::setSubFile(SubFile *sub) {
    subFile = sub;
    groupLayout = new QVBoxLayout;

    for (auto group : subFile->get_groups()) {
        if (group == nullptr) continue;
        auto groupWidget = new ViewerGroupWidget(group);
        groupLayout->addWidget(groupWidget);
        groups.emplace_back(groupWidget);
    }

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(orderButton);
    buttonLayout->setAlignment(Qt::AlignHCenter);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(groupLayout);
    mainLayout->addLayout(buttonLayout);
    mainLayout->addStretch(1);
    setLayout(mainLayout);
}

ViewerMainWidget::~ViewerMainWidget() {
    for (auto i : groups) {
        delete i;
    }
}

void ViewerMainWidget::remove_group(ViewerGroupWidget *groupWidget) const {
    groupLayout->removeWidget(groupWidget);
    subFile->remove_group(groupWidget->group);
    delete groupWidget->group;
    delete groupWidget;
}

ViewerGroupWidget *ViewerMainWidget::add_group(FileItemGroup *g) {
    subFile->add_group(g);
    auto groupWidget = new ViewerGroupWidget(g);

    groupLayout->addWidget(groupWidget);
    groups.emplace_back(groupWidget);
    return groupWidget;
}

ItemTextLabel::ItemTextLabel(ViewerItemWidget *p) {
    itemWidget = p;
}
