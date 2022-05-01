#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QSettings>

#include "Viewer/SubFileViewer.h"
#include "RacheFile/FileDocument.h"
#include "globals.h"

struct RecentFileInfo{
    RecentFileInfo(const QString& fn,  const std::string& dn) {
        documentName    = dn;
        filename        = fn;
    }
    std::string documentName;
    QString filename;
};

class RecentFileAction : public QAction {
public:
    RecentFileAction(const QString& fn, const QString& dn) {
        filename = fn;
        documentName = dn;
        setText(QString("%1(%2)").arg(dn, fn));
    }

    QString filename;
    QString documentName;
};

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

    QList<RecentFileInfo> fileRecordList;

    SubFileViewer *viewer;

    QAction *langZhCn;
    QAction *langZhHk;
    QAction *langEnUs;
    QAction *newFileAction;
    QAction *openFileAction;
    QAction *saveFileAction;
    QAction *saveAsFileAction;
    QAction *editFileInfoAction;
    QAction *topWindowAction;
    QAction *showToolButtonAction;
    QAction *autoFromClipBoardItemAction;
    QAction *autoFromClipBoardSubfileAction;
    QAction *aboutQtAction;
    QAction *searchAction;
    QAction *showTreeAction;
    QAction *jumpToAction;
    QAction *backToAction;
    QAction *editResourceAction;
    QMenu *openRecentMenu;

    FileDocument *document;
    QString filepath;

    bool load_file(const QString& path);
    void add_record_file(const QString& path);

public slots:
    static void set_language_zh_cn() ;
    static void set_language_zh_hk() ;
    static void set_language_en_us() ;
    void set_top_window();
    void set_show_toolbutton(bool show) const;
    static void set_auto_from_clip_board_item(bool b) ;
    static void set_auto_from_clip_board_subfile(bool b) ;
    void subfile_changed(SubFile * subFile);
    bool openFileAction_triggered();
    void recentMenu_triggered(QAction *action);
    void save_file();
    void save_as();
    bool newFile();
    bool open_file(const QString& path);
    void edit_file_info();
    void about_qt();
    void show_tree() const;
    void search_document() const;
    void edit_resource() const;

protected:
    void closeEvent(QCloseEvent*event) override;

private:
    void initUI();
};
#endif // MAINWINDOW_H
