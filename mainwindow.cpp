#include "mainwindow.h"
#include "globals.h"

#include "Dialog/Editor.h"
#include "Dialog/FindDialog.h"
#include "Dialog/ResourceDialog.h"

#include <QMenuBar>
#include <QApplication>
#include <QTranslator>
#include <QFileDialog>
#include <QInputDialog>
#include <QJsonArray>
#include <QJsonDocument>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    viewer = new SubFileViewer;
    document = new FileDocument();

    langZhCn = new QAction("简体中文");
    langZhHk = new QAction("繁體中文");
    langEnUs = new QAction("English");
    newFileAction = new QAction(tr("新建"));
    openFileAction = new QAction(tr("打开"));
    saveFileAction = new QAction(tr("保存"));
    saveAsFileAction = new QAction(tr("另存为"));
    editFileInfoAction = new QAction(tr("文件信息"));
    topWindowAction = new QAction(tr("窗口置顶"));
    showToolButtonAction = new QAction(tr("显示工具按钮"));
    autoFromClipBoardItemAction = new QAction(tr("新建项"));
    autoFromClipBoardSubfileAction = new QAction(tr("新建子文件"));
    aboutQtAction = new QAction(tr("关于Qt"));
    searchAction = new QAction(tr("查找"));
    showTreeAction = new QAction(tr("以树的形式显示"));
    jumpToAction = new QAction(tr("跳转"));
    backToAction = new QAction(tr("返回"));
    editResourceAction = new QAction(tr("编辑资源"));
    openRecentMenu = new QMenu(tr("打开最近文件"));

    connect(langZhCn, SIGNAL(triggered()), this, SLOT(set_language_zh_cn()));
    connect(langZhHk, SIGNAL(triggered()), this, SLOT(set_language_zh_hk()));
    connect(langEnUs, SIGNAL(triggered()), this, SLOT(set_language_en_us()));
    connect(openFileAction, SIGNAL(triggered()), this, SLOT(openFileAction_triggered()));
    connect(openRecentMenu, SIGNAL(triggered(QAction *)), this, SLOT(recentMenu_triggered(QAction *)));
    connect(saveFileAction, SIGNAL(triggered()), this, SLOT(save_file()));
    connect(saveAsFileAction, SIGNAL(triggered()), this, SLOT(save_as()));
    connect(newFileAction, SIGNAL(triggered()), this, SLOT(newFile()));
    connect(topWindowAction, SIGNAL(triggered()), this, SLOT(set_top_window()));
    connect(showToolButtonAction, SIGNAL(triggered(bool)), this, SLOT(set_show_toolbutton(bool)));
    connect(autoFromClipBoardItemAction, SIGNAL(triggered(bool)), this, SLOT(set_auto_from_clip_board_item(bool)));
    connect(autoFromClipBoardSubfileAction, SIGNAL(triggered(bool)), this, SLOT(set_auto_from_clip_board_subfile(bool)));
    connect(editFileInfoAction, SIGNAL(triggered()), this, SLOT(edit_file_info()));
    connect(aboutQtAction, SIGNAL(triggered()), this, SLOT(about_qt()));
    connect(showTreeAction, SIGNAL(triggered()), this, SLOT(show_tree()));
    connect(searchAction, SIGNAL(triggered()), this, SLOT(search_document()));
    connect(editResourceAction, SIGNAL(triggered()), this, SLOT(edit_resource()));

    connect(viewer, SIGNAL(subFileChanged(SubFile*)), this, SLOT(subfile_changed(SubFile*)));

    std::ifstream recordFile("record.txt");
    if (recordFile.is_open()) {
        char line[256];
        while (recordFile.getline(line, 256)) {
            char fn[256];
            char dn[64];
            sscanf(line, "%s %s\n", &fn, &dn);
            fileRecordList.append(RecentFileInfo(QString::fromUtf8(fn), dn));
            openRecentMenu->addAction(new RecentFileAction(QString::fromUtf8(fn), QString::fromUtf8(dn)));
        }
        recordFile.close();
    } else {
        qDebug()<<"读取失败";
    }

    initUI();

    showToolButtonAction->setChecked(ShowToolButton);
    viewer->set_show_toolbutton(ShowToolButton);
    autoFromClipBoardItemAction->setChecked(AutoFromClipBoardItemText);
    autoFromClipBoardSubfileAction->setChecked(AutoFromClipBoardSubFileName);
}

MainWindow::~MainWindow()
{

}

void MainWindow::initUI() {
    setFixedSize(600, 800);
    setCentralWidget(viewer);
    viewer->set_statusbar(statusBar());
    setWindowIcon(AppIcon);

    topWindowAction->setCheckable(true);
    showToolButtonAction->setCheckable(true);
    autoFromClipBoardItemAction->setCheckable(true);
    autoFromClipBoardSubfileAction->setCheckable(true);

    aboutQtAction->setIcon(qApp->style()->standardIcon((QStyle::StandardPixmap)0));

    auto* aboutMenu = new QMenu();
    aboutMenu->setTitle(tr("关于"));
    aboutMenu->addAction(aboutQtAction);

    auto* settingLanguageMenu = new QMenu();
    settingLanguageMenu->setTitle(tr("语言"));
    settingLanguageMenu->addAction(langZhCn);
    settingLanguageMenu->addAction(langZhHk);
    settingLanguageMenu->addAction(langEnUs);

    auto *settingAutoClipBoardMenu = new QMenu();
    settingAutoClipBoardMenu->setTitle(tr("自动从剪贴板导入"));
    settingAutoClipBoardMenu->addAction(autoFromClipBoardItemAction);
    settingAutoClipBoardMenu->addAction(autoFromClipBoardSubfileAction);

    auto* windowMenu = new QMenu();
    windowMenu->setTitle(tr("窗口"));
    windowMenu->addAction(topWindowAction);

    auto* settingsMenu = new QMenu();
    settingsMenu->setTitle(tr("设置"));
    settingsMenu->addMenu(settingLanguageMenu);
    settingsMenu->addSeparator();
    settingsMenu->addAction(showToolButtonAction);
    settingsMenu->addMenu(settingAutoClipBoardMenu);

    openFileAction->setShortcut(QKeySequence::Open);
    newFileAction->setShortcut(QKeySequence::New);
    saveFileAction->setShortcut(QKeySequence::Save);
    saveAsFileAction->setShortcut(QKeySequence::SaveAs);

    auto* fileMenu = new QMenu();
    fileMenu->setTitle(tr("文件"));
    fileMenu->addAction(newFileAction);
    fileMenu->addAction(openFileAction);
    fileMenu->addMenu(openRecentMenu);
    fileMenu->addAction(saveFileAction);
    fileMenu->addAction(saveAsFileAction);
    fileMenu->addSeparator();
    fileMenu->addAction(editFileInfoAction);

    auto* toolMenu = new QMenu();
    toolMenu->setTitle(tr("工具"));
    toolMenu->addAction(searchAction);
    toolMenu->addAction(showTreeAction);
    toolMenu->addAction(editResourceAction);

    searchAction->setShortcut(QKeySequence::Find);

    auto* jumpToMenu = new QMenu();
    jumpToMenu->setTitle(tr("跳转"));
    jumpToMenu->addAction(jumpToAction);
    jumpToMenu->addAction(backToAction);

    this->menuBar()->addMenu(fileMenu);
    this->menuBar()->addMenu(jumpToMenu);
    this->menuBar()->addMenu(toolMenu);
    this->menuBar()->addMenu(windowMenu);
    this->menuBar()->addMenu(settingsMenu);
    this->menuBar()->addMenu(aboutMenu);

    searchAction->setIcon(FindIcon);
    showTreeAction->setIcon(TreeIcon);
    editResourceAction->setIcon(ResourceIcon);
}

void MainWindow::set_language_zh_cn() {
    LanguageName = "zh_CN";
}

void MainWindow::set_language_zh_hk() {
    LanguageName = "zh_HK";
}

void MainWindow::set_language_en_us() {
    LanguageName = "en_US";
}

bool MainWindow::openFileAction_triggered() {
    QString path = QFileDialog::getOpenFileName(this, tr("打开"), ".", "档案文件(*.docu *.document)");
    if (path.isEmpty()) return false;
    open_file(path);
    return true;
}

void MainWindow::save_file() {
    if (filepath.isEmpty()) {
        QString path = QFileDialog::getSaveFileName(this, tr("保存"), "./"+QString::fromStdString(document->name)+".docu","档案文件(*.docu);;档案文件(*.document)");
        if (path.isEmpty()) return;
        filepath = path;
        add_record_file(path);
    }
    bool s = document->save(std::string(filepath.toLocal8Bit()));
    if (s) {
        statusBar()->showMessage(tr("已保存文件到")+filepath, 5000);
    } else {
        QMessageBox::critical(this, tr("错误"), tr("保存失败"));
    }
}

bool MainWindow::newFile() {
    bool ok;
    QString newName = QInputDialog::getText(this, tr("新建档案"), tr("输入档案名称"), QLineEdit::Normal, "", &ok);
    if (ok) {
        auto *newDocument = new FileDocument();
        newDocument->name = newName.toStdString();
        bool b;
        QString newSubName = QInputDialog::getText(this, tr("新建子文件"), tr("输入子文件名称"),
                QLineEdit::Normal, "", &b);
        if (b) {
            newDocument->add_subfile(new SubFile(newSubName.toStdString()));
            auto *mainWindow = new MainWindow();
            mainWindow->document = newDocument;
            mainWindow->viewer->set_fileDocument(newDocument);
            mainWindow->viewer->set_subfile(0);
            mainWindow->show();
        } else return false;
        return true;
    }
    return false;
}

bool MainWindow::open_file(const QString& path) {
    statusBar()->showMessage(tr("正在加载文件"));
    if (!load_file(path)) {
        QMessageBox::critical(this, tr("错误"), tr("无法打开文件"), QMessageBox::Ok);
        return false;
    }
    filepath = path;
    viewer->set_fileDocument(document);
    viewer->set_show_toolbutton(showToolButtonAction->isChecked());
    add_record_file(path);
    statusBar()->showMessage(tr("就绪。"));
    return true;
}

void MainWindow::closeEvent(QCloseEvent *event) {
    std::ofstream recordFile("record.txt");
    if (recordFile.is_open()) {
        for (auto &info : fileRecordList) {
            recordFile << info.filename.toUtf8().data();
            recordFile << ' ';
            recordFile << info.documentName;
            recordFile << '\n';
        }
        recordFile.close();
    }

    delete document;
    document = nullptr;
    if (is_file_exist("temp.tmp")) {
        QFile::remove("temp.tmp");
    }
}

void MainWindow::set_top_window() {
    if (topWindowAction->isChecked()) {
        setWindowFlag(Qt::WindowStaysOnTopHint);
    } else {
        setWindowFlag(Qt::CustomizeWindowHint);
    }
    show();
}

void MainWindow::subfile_changed(SubFile *subFile) {
    setWindowTitle(QString::fromStdString(document->name + "/" + subFile->name));
}

void MainWindow::edit_file_info() {
    auto *editor = new DocumentEditor(document);
    editor->setWindowIcon(AppIcon);
    editor->exec();
}

void MainWindow::set_show_toolbutton(bool show) const {
    ShowToolButton = show;
    viewer->set_show_toolbutton(show);
}

void MainWindow::about_qt() {
    QMessageBox::aboutQt(this, tr("关于Qt"));
}

void MainWindow::save_as() {
    QString path = QFileDialog::getSaveFileName(this, tr("另存为"),
                                                "./"+QString::fromStdString(document->name)+".docu",
                                                "档案文件(*.docu);;档案文件(*.document)");
    if (path.isEmpty()) return;
    add_record_file(path);
    filepath = path;
    bool b = document->save(std::string(filepath.toLocal8Bit()));
    if (b) {
        statusBar()->showMessage(tr("已保存文件到")+path, 5000);
    } else {
        QMessageBox::critical(this, tr("错误"), tr("保存失败"));
    }
}

void MainWindow::set_auto_from_clip_board_item(bool b) {
    AutoFromClipBoardItemText = b;
}

void MainWindow::set_auto_from_clip_board_subfile(bool b) {
    AutoFromClipBoardSubFileName = b;
}

bool MainWindow::load_file(const QString &path) {
    delete document;

    if (is_file_exist("temp.tmp")) {
        if (QFile::remove("temp.tmp")) {
            std::cout << "remove temp file successfully" << std::endl;
        } else {
            std::cout << "remove temp file failed" << std::endl;
        }
    }
    QFile::copy(path, "temp.tmp");

    document = new FileDocument;
    return document->open("temp.tmp");
}

void MainWindow::show_tree() const {
    auto *tree = new FileDocumentTree(viewer->fileDocument);
    tree->setWindowTitle(QString::fromStdString(viewer->fileDocument->name) + tr(" 树"));
    tree->setWindowIcon(TreeIcon);
    tree->show();
}

void MainWindow::search_document() const {
    FindDialog dialog(document);
    dialog.setWindowIcon(FindIcon);
    dialog.setWindowTitle(tr("搜索"));
    dialog.exec();
}

void MainWindow::edit_resource() const {
    ResourceDialog dialog(viewer->subFile);
    dialog.setWindowIcon(ResourceIcon);
    dialog.exec();
}

void MainWindow::add_record_file(const QString& path) {
    auto iter = fileRecordList.begin();
    auto endIter = fileRecordList.end();
    int i = 0;
    while (iter != endIter) {
        if (iter->filename == path) {
            fileRecordList.erase(iter);
            openRecentMenu->removeAction(openRecentMenu->actions()[i]);
            break;
        }
        i++;
        iter++;
    }
    fileRecordList.insert(0, RecentFileInfo(path, document->name));
    openRecentMenu->insertAction(openRecentMenu->actions()[0], new RecentFileAction(path, QString::fromStdString(document->name)));
}

void MainWindow::recentMenu_triggered(QAction *action) {
    auto a =(RecentFileAction*)action;
    open_file(a->filename);
}
