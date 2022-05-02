//
// Created by 刘镇睿 on 2020/12/4.
//

#include "StartUI.h"
#include <QApplication>
#include <QStyle>
#include <QLabel>
#include <QFileDialog>
#include <QInputDialog>
#include <QMenu>
#include "qdebug.h"

StartUI::StartUI() {
    newFileButton = new QToolButton();
    openFileButton = new QToolButton();

    initUI();

    connect(newFileButton, SIGNAL(clicked()), this, SLOT(new_file()));
    connect(openFileButton, SIGNAL(clicked()), this, SLOT(open_file()));
}

void StartUI::initUI() {
    newFileButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    newFileButton->setText(tr("新建"));
    newFileButton->setIcon(qApp->style()->standardIcon(QStyle::SP_FileIcon));
    newFileButton->setIconSize(QSize(16, 16));
    newFileButton->setShortcut(QKeySequence::New);

    openFileButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    openFileButton->setText(tr("打开"));
    openFileButton->setIcon(qApp->style()->standardIcon(QStyle::SP_DialogOpenButton));
    openFileButton->setIconSize(QSize(16, 16));
    openFileButton->setShortcut(QKeySequence::Open);

    auto *layout1 = new QVBoxLayout;
    layout1->addWidget(new QLabel(tr("选择一个操作")));
    layout1->addWidget(newFileButton);
    layout1->addWidget(openFileButton);
    setLayout(layout1);

    std::ifstream recordFile("record.txt");
    if (recordFile.is_open()) {
        char line[256];
        auto *fileMenu = new QMenu;
        while (recordFile.getline(line, 256)) {
            char fn[256];
            char dn[64];
            sscanf(line, "%s %s\n", &fn, &dn);
            fileMenu->addAction(new RecentFileAction(QString::fromUtf8(fn), QString::fromUtf8(dn)));
        }
        openFileButton->setMenu(fileMenu);
        openFileButton->setPopupMode(QToolButton::MenuButtonPopup);
        connect(fileMenu, SIGNAL(triggered(QAction*)), this, SLOT(open_file(QAction*)));
        recordFile.close();
    } else {
        qDebug()<<"读取失败";
    }

    setWindowTitle(tr("档案"));
    setWindowIcon(QIcon(RESOURCE_PATH(/icon/File.png)));
    setFont(QFont("Microsoft YaHei", 6));
}

void StartUI::open_file() {
    QString path = QFileDialog::getOpenFileName(this, tr("打开"), ".", "档案文件(*.docu *.document)");
    if (path.isEmpty()) return;
    auto *window = new MainWindow();
    if (!window->open_file(path)) return;
    window->show();
    close();
}

void StartUI::new_file() {
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
            close();
        }
    }
}

void StartUI::open_file(QAction *action) {
    auto *window = new MainWindow();
    window->open_file(((RecentFileAction*)action)->filename);
    window->show();
    close();
}
