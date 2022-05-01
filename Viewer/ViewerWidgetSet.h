//
// Created by Rache on 2021/10/2.
//

#ifndef RACHEFILE_VIEWERWIDGETSET_H
#define RACHEFILE_VIEWERWIDGETSET_H

#include <QLabel>
#include <QToolButton>
#include <QVBoxLayout>

#include "RacheFile/FileItemObject.h"
#include "RacheFile/FileItemGroup.h"
#include "RacheFile/SubFile.h"
#include "Widget/EditableLabel.h"

#define Q_STR(s) QString::fromStdString(s)

class ItemToolButton;
class GroupToolButton;
class ViewerItemWidget;
class ViewerGroupWidget;

class ViewerItemWidget : public QWidget{
public:
    ViewerItemWidget(FileItemObject* fileItemObject, ViewerGroupWidget *p);
    void moveEvent(QMoveEvent *event) override;

    QLabel *nameLabel;
    EditableLabel *textLabel;

    ItemToolButton *editButton;
    ItemToolButton *copyButton;
    ItemToolButton *pasteButton;
    ItemToolButton *removeButton;
    ItemToolButton *resourceButton;

    FileItemObject *item;
    ViewerGroupWidget *groupWidget;

    void fresh();
    int id = -1;
};

class ViewerGroupWidget : public QWidget{
    Q_OBJECT
public:
    ViewerGroupWidget(FileItemGroup *fileItemGroup);
    ~ViewerGroupWidget() override;

    QLabel *nameLabel;

    QToolButton *addButton;
    QToolButton *removeButton;
    QToolButton *editButton;
    QToolButton *hideButton;

    QVBoxLayout *mainLayout;

    std::vector<ViewerItemWidget*> items;

    FileItemGroup *group;

    ViewerItemWidget *add_item(FileItemObject *i);
    void remove_item(ViewerItemWidget *i);
    void hide_group();

private:
    void show_items();
    void hide_items();
};

class ViewerMainWidget : public QWidget{
public:
    explicit ViewerMainWidget(SubFile *subFile);
    ~ViewerMainWidget() override;

    QToolButton *addButton;
    QToolButton *orderButton;
    QVBoxLayout *groupLayout{};

    std::vector<ViewerGroupWidget*> groups;

    void remove_group(ViewerGroupWidget *groupWidget) const;
    void setSubFile(SubFile *s);
    ViewerGroupWidget *add_group(FileItemGroup* g);

    SubFile *subFile{};
    int from = 0;
    int to = -1;
};

class ItemToolButton : public QToolButton {
public:
    ItemToolButton(const QString& t, const QIcon& i, ViewerItemWidget* p);

    ViewerItemWidget *itemWidget;
};

class ItemTextLabel : public EditableLabel {
public:
    explicit ItemTextLabel(ViewerItemWidget *p);

    ViewerItemWidget *itemWidget;
};

class GroupToolButton : public QToolButton {
public:
    GroupToolButton(const QString& t, const QIcon& i, ViewerGroupWidget *p);

    ViewerGroupWidget *groupWidget;
};


#endif //RACHEFILE_VIEWERWIDGETSET_H
