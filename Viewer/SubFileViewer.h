//
// Created by 刘镇睿 on 2020/11/20.
//

#ifndef RACHEFILE_SUBFILEVIEWER_H
#define RACHEFILE_SUBFILEVIEWER_H

#include "RacheFile/FileDocument.h"
#include "Viewer/ViewerWidgetSet.h"
#include <QLabel>
#include <QToolButton>
#include <QScrollArea>
#include <QGridLayout>
#include <QIcon>
#include <QAction>
#include <QStatusBar>
#include <QTimer>
#include <QProgressDialog>

class SubFileAction : public QAction {
public:
    explicit SubFileAction(SubFile *s) {
        subfile = s;
        setText(QString::fromStdString(s->name));
    }
    SubFile *subfile;
};


class SubFileViewer : public QWidget{
    Q_OBJECT
public:
    SubFileViewer();
    FileDocument* fileDocument = nullptr;
    SubFile* subFile = nullptr;

    QLabel* nameLabel;
    QScrollArea *scrollArea;
    QToolButton *addSubButton;
    QToolButton *subFileListButton;
    QToolButton *removeSubFileButton;
    QMenu *subFileListMenu;
    QAction *addSubFileAction;
    QAction *subFileInfoAction;
    QList<SubFileAction*> subFileActionList;
    ViewerMainWidget *viewerMainWidget = nullptr;

    void init_ui();

    void set_fileDocument(FileDocument* f);
    void set_subfile(int index);
    void set_subfile(SubFile *s);

    void load_subFileMenu();

    void set_statusbar(QStatusBar *bar);

    int currentPage = 0;
    int pageCount = 0;

public slots:
    void view();

    // for item
    void edit_item();
    void copy_item();
    void paste_item();
    void remove_item();
    void show_item_resource();
    void item_text_changed();

    // for group
    void add_item();
    void remove_group();
    void edit_group();
    void hide_group();

    // for sub file
    void add_subfile();
    void add_group();
    void order_group();
    void delete_subFile();
    void show_subfile_info();

    void set_show_toolbutton(bool show) const;

    void subFileListMenu_triggered(QAction* action);

signals:
    void subFileChanged(SubFile* subFile);
    void refreshViewer();

private:
    QStatusBar *statusBar;

    static QString get_clip_board_text();
    void show_message(const QString& message);
    void connect_group_widget_slot(ViewerGroupWidget *groupWidget);
    void connect_item_widget_slot(ViewerItemWidget *itemWidget);

    static void show_resource_item_switch(const ResItem& resItem);
    static void show_image_resource(const ResItem& resItem);
    static void show_text_resource(const ResItem& resItem);
    static void show_audio_resource(const ResItem& resItem);
    static void show_video_resource(const ResItem& resItem);
};


#endif //RACHEFILE_SUBFILEVIEWER_H
