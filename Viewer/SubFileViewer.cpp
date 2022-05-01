//
// Created by 刘镇睿 on 2020/11/20.
//

#include "Viewer/SubFileViewer.h"
#include "Viewer/ResourceViewer.h"
#include "Dialog/ItemEditor.h"
#include "Dialog/Editor.h"
#include "Dialog/OrderListWidget.h"
#include "Dialog/ResourceViewerDialog.h"
#include "ToolFunction.h"
#include "globals.h"

#include <QToolButton>
#include <QApplication>
#include <QClipboard>
#include <QMessageBox>
#include <QInputDialog>
#include <QDateTimeEdit>
#include <QMenu>
#include <QAction>
#include <QScrollBar>

#define C_STR(s) s.toStdString()

SubFileViewer::SubFileViewer() {
    addSubButton = new QToolButton();
    scrollArea = new QScrollArea();
    nameLabel = new QLabel();
    subFileListButton = new QToolButton();
    subFileListMenu = new QMenu();
    addSubFileAction = new QAction(tr("新建子文件"));
    subFileInfoAction = new QAction(tr("子文件信息"));
    removeSubFileButton = new QToolButton();
    statusBar = nullptr;

    init_ui();

    connect(this, SIGNAL(refreshViewer()), this, SLOT(view()));
    connect(addSubButton, SIGNAL(clicked()), this, SLOT(add_subfile()));
    connect(subFileListMenu, SIGNAL(triggered(QAction*)), this, SLOT(subFileListMenu_triggered(QAction*)));
    connect(removeSubFileButton, SIGNAL(clicked()), this, SLOT(delete_subFile()));
    connect(subFileListButton, SIGNAL(clicked()), this, SLOT(show_subfile_info()));

    nameLabel->setObjectName("SubFileViewerTitleLabel");
    subFileListButton->setObjectName("ViewerSubFileListButton");
    addSubButton->setProperty("ViewerProperty", "SubFile");
    removeSubFileButton->setProperty("ViewerProperty", "SubFile");

    QFile stylesheetFile(RESOURCE_PATH(/stylesheet/subfileviewer.txt));
    if (stylesheetFile.open(QFile::ReadOnly)) {
        setStyleSheet(stylesheetFile.readAll());
    }
}

void SubFileViewer::init_ui() {
    nameLabel->setTextInteractionFlags(Qt::TextInteractionFlag::TextSelectableByMouse);
    addSubButton->setToolTip(tr("新建子文件"));
    addSubButton->setIcon(AddIcon);
    subFileListButton->setIcon(ListIcon);
    subFileListButton->setMenu(subFileListMenu);
    subFileListButton->setPopupMode(QToolButton::MenuButtonPopup);
    removeSubFileButton->setIcon(DeleteIcon);
    removeSubFileButton->setToolTip(tr("删除子文件"));

    auto *nameLabelLayout = new QHBoxLayout();
    nameLabelLayout->addWidget(nameLabel);
    nameLabelLayout->addStretch(1);
    nameLabelLayout->addWidget(subFileListButton);
    nameLabelLayout->addWidget(addSubButton);
    nameLabelLayout->addWidget(removeSubFileButton);

    auto *viewLayout = new QVBoxLayout();
    viewLayout->addLayout(nameLabelLayout);
    viewLayout->addWidget(scrollArea);
    setLayout(viewLayout);

    setWindowTitle(tr("档案"));
}

void SubFileViewer::view() {
    QProgressDialog *progressDialog = nullptr;
    int count = subFile->count().itemCounter.total;
    if (count > 99) {
        progressDialog = new QProgressDialog;
        progressDialog->setMaximum(0);
        progressDialog->show();
        progressDialog->setWindowIcon(windowIcon());
        progressDialog->setLabelText(tr("正在加载。"));
    }


    viewerMainWidget = new ViewerMainWidget(subFile);
    viewerMainWidget->setObjectName("MainScrollArea");
    delete scrollArea->widget();
    scrollArea->setWidget(viewerMainWidget);
    scrollArea->setWidgetResizable(true);
    viewerMainWidget->setSubFile(subFile);

    connect(viewerMainWidget->addButton, SIGNAL(clicked()), this, SLOT(add_group()));
    connect(viewerMainWidget->orderButton, SIGNAL(clicked()), this, SLOT(order_group()));
    for (auto groupWidget : viewerMainWidget->groups) {
        if (groupWidget == nullptr) continue;
        connect_group_widget_slot(groupWidget);

        for (auto itemWidget : groupWidget->items) {
            if (itemWidget == nullptr) continue;
            connect_item_widget_slot(itemWidget);
            QApplication::processEvents();
        }
    }

    nameLabel->setText(QString::fromStdString(subFile->name));
    if (progressDialog != nullptr) {
        progressDialog->close();
        delete progressDialog;
    }
}

void SubFileViewer::add_item() {
    auto button = (GroupToolButton*)sender();
    auto groupWidget = button->groupWidget;

    ItemEditor editor(nullptr, subFile, fileDocument);
    if (AutoFromClipBoardItemText) {
        editor.textEdit->setPlainText(get_clip_board_text());
    }

    editor.setWindowTitle(tr("新建项"));
    editor.setWindowIcon(windowIcon());
    editor.exec();

    if (editor.ok) {
        if (editor.nameEdit->text().isEmpty()) {
            QMessageBox::warning(this, tr("问题"), tr("名称填写为空。"), QMessageBox::Yes);
            return;
        }
        if (editor.textEdit->toPlainText().isEmpty()) {
            QMessageBox::warning(this, tr("问题"), tr("内容填写为空。"), QMessageBox::Yes);
            return;
        }

        QString name = editor.nameEdit->text();
        QString text = editor.textEdit->toPlainText();
        QDate date = editor.dateTimeEdit->date();

        auto *item = new FileItemObject(C_STR(name),
                C_STR(text), C_STR(editor.sourceEdit->text()));
        date.getDate(&item->year, &item->month, &item->day);
        item->type = (FileItemObject::TYPE)editor.buttonGroup->checkedId();
        QString().toStdString();

        connect_item_widget_slot(groupWidget->add_item(item));
    }
}

void SubFileViewer::remove_group() {
    auto button = (GroupToolButton*)sender();
    QMessageBox::Button b = QMessageBox::question(this, tr("删除组"), tr("你确定要删除这个组吗？"),
                                                  QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (b==QMessageBox::No) return;

    viewerMainWidget->remove_group(button->groupWidget);
}

void SubFileViewer::edit_item() {
    auto button = (ItemToolButton*)sender();
    auto itemWidget = button->itemWidget;
    auto itemObject = itemWidget->item;

    ItemEditor editor(itemObject, subFile, fileDocument);
    editor.setWindowTitle(tr("编辑项"));
    editor.setWindowIcon(this->windowIcon());
    editor.exec();
    if (editor.ok) {
        if (editor.textEdit->toPlainText().isEmpty()) {
            QMessageBox::warning(this, tr("问题"), tr("内容填写为空。"), QMessageBox::Yes);
            return;
        }

        itemObject->name = C_STR(editor.nameEdit->text());
        itemObject->text = C_STR(editor.textEdit->toPlainText());
        editor.dateTimeEdit->date().getDate(&itemObject->year, &itemObject->month, &itemObject->day);
        itemObject->source = C_STR(editor.sourceEdit->text());
        itemObject->resName = C_STR(editor.resName);
        itemObject->type = static_cast<FileItemObject::TYPE>(editor.buttonGroup->checkedId());
        itemWidget->fresh();
    }
}

void SubFileViewer::copy_item() {
    auto *button = (ItemToolButton*)sender();
    QClipboard* clipboard = QApplication::clipboard();
    clipboard->setText(Q_STR(button->itemWidget->item->text));
    show_message(tr("已复制到剪切板。"));
}

void SubFileViewer::paste_item() {
    auto *button = (ItemToolButton*) sender();
    QClipboard *clipboard = QApplication::clipboard();
    QString text = clipboard->text();
    button->itemWidget->item->text = C_STR(text);
    button->itemWidget->textLabel->setText(text);
    show_message(tr("粘贴成功"));
}

void SubFileViewer::remove_item() {
    auto *button = (ItemToolButton*)sender();
    QMessageBox::Button b = QMessageBox::question(this, tr("删除项"), tr("你确定要删除这个项吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    if (b==QMessageBox::No) return;

    button->itemWidget->groupWidget->remove_item(button->itemWidget);
}

void SubFileViewer::show_item_resource() {
    auto *button = (ItemToolButton*)sender();
    std::string rn = button->itemWidget->item->resName;

    bool e;
    ResItem ri = subFile->resourceSet.get(rn, e);
    if (e) {
        QMessageBox::critical(this, tr("错误"), tr("在子文件中找不到资源：")+ Q_STR(rn));
        return;
    }
    show_resource_item_switch(ri);
}

void SubFileViewer::item_text_changed() {
    auto *label = (ItemTextLabel*)sender();
    label->itemWidget->item->text = label->text().toStdString();
}

void SubFileViewer::set_subfile(int index) {
    set_subfile(fileDocument->get_subfile(index));
}

void SubFileViewer::set_subfile(SubFile *s) {
    subFile = s;
    pageCount = fileDocument->itemCount / SinglePageItemCount;
    if (fileDocument->itemCount % SinglePageItemCount) pageCount++;
    currentPage = 1;
    view();
    emit subFileChanged(subFile);
}


void SubFileViewer::set_fileDocument(FileDocument *f) {
    fileDocument = f;
    set_subfile(0);
    load_subFileMenu();
}

void SubFileViewer::add_subfile() {
    bool ok;
    QString newName = QInputDialog::getText(this, tr("新建子文件"), tr("输入新的子文件的名称。"),
            QLineEdit::Normal, AutoFromClipBoardSubFileName ? get_clip_board_text() : QString(), &ok);
    if (ok) {
        if (newName.isEmpty()) {
            QMessageBox::warning(this, tr("问题"), tr("名称不能为空。"), QMessageBox::Yes);
            return;
        }
        auto *sub = new SubFile(newName.toStdString());
        fileDocument->add_subfile(sub);
        set_subfile(sub);

        auto *subFileAction = new SubFileAction(sub);
        subFileListMenu->addAction(subFileAction);
    }
}

void SubFileViewer::add_group() {
    bool ok;
    QString groupName = QInputDialog::getText(this, tr("新建组"), tr("请输入组名"), QLineEdit::Normal, "", &ok);
    if (!ok) return;

    if (groupName.isEmpty()) {
        QMessageBox::warning(this, tr("问题"), tr("名称不能为空。"), QMessageBox::Yes);
        return;
    }

    auto *newGroup = new FileItemGroup(C_STR(groupName));
    connect_group_widget_slot(viewerMainWidget->add_group(newGroup));
}

void SubFileViewer::subFileListMenu_triggered(QAction *action) {
    if (action == addSubFileAction) {
        add_subfile();
        return;
    }
    if (action==subFileInfoAction) {
        show_subfile_info();
    }
    auto* subFileAction = (SubFileAction*)action;
    set_subfile(subFileAction->subfile);
}

void SubFileViewer::hide_group() {
    auto button = (GroupToolButton*)sender();
    auto groupWidget = button->groupWidget;

    groupWidget->hide_group();
}

void SubFileViewer::order_group() {
    QList<QString> texts;
    for (FileItemGroup* group : subFile->get_groups()) {
        texts.append(Q_STR(group->name));
    }
    auto *orderListWidget = new OrderListWidget;
    orderListWidget->setWindowIcon(windowIcon());
    orderListWidget->setWindowTitle(tr("排序"));
    orderListWidget->set_list(texts);
    orderListWidget->show();
    orderListWidget->exec();
    if (!orderListWidget->ok) return;
    std::vector<FileItemGroup*> groups = subFile->get_groups();
    subFile->clear_group();
    for (int i : orderListWidget->resultList) {
        subFile->add_group(groups[i]);
    }
    view();
}

void SubFileViewer::edit_group() {
    auto button = (GroupToolButton*)sender();
    auto groupWidget = button->groupWidget;
    auto group = groupWidget->group;

    bool ok;
    QString text = QInputDialog::getText(this, tr("编辑组名"), tr("在此键入组名"), QLineEdit::Normal, Q_STR(group->name), &ok);

    if (!ok) return;
    group->name = C_STR(text);
    groupWidget->nameLabel->setText(text);
}

void SubFileViewer::set_show_toolbutton(bool show) const {
    if (viewerMainWidget == nullptr) return;
    if (show) {
        for (auto groupWidget : viewerMainWidget->groups) {
            if (groupWidget == nullptr) continue;
            for (auto itemWidget : groupWidget->items) {
                if (itemWidget == nullptr) continue;
                itemWidget->editButton->show();
                itemWidget->copyButton->show();
                itemWidget->pasteButton->show();
                itemWidget->removeButton->show();
            }
        }
    } else {
        for (auto groupWidget : viewerMainWidget->groups) {
            if (groupWidget == nullptr) continue;
            for (auto itemWidget : groupWidget->items) {
                if (itemWidget == nullptr) continue;
                itemWidget->editButton->hide();
                itemWidget->copyButton->hide();
                itemWidget->pasteButton->hide();
                itemWidget->removeButton->hide();
            }
        }
    }
}

void SubFileViewer::delete_subFile() {
    if (fileDocument->count_sub_file() == 1) {
        QMessageBox::critical(this, tr("错误"), tr("每个档案中必须至少有一个子文件。请新建一个子文件后再试一次。"), QMessageBox::Yes);
        return;
    } else {
        QMessageBox::Button button = QMessageBox::question(this, tr("删除子文件"), tr("你确定要删除这个子文件吗？"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
        if (button==QMessageBox::No) return;
        SubFile *s = subFile;
        set_subfile(0);
        fileDocument->remove_subfile(s);
        delete s;
    }

    load_subFileMenu();
}

void SubFileViewer::load_subFileMenu() {
    qDeleteAll(subFileActionList);
    subFileActionList.clear();
    subFileListMenu->clear();
    subFileListMenu->addAction(addSubFileAction);
    subFileListMenu->addAction(subFileInfoAction);
    subFileListMenu->addSeparator();
    for (auto i : fileDocument->get_subs()) {
        if (i != nullptr) {
            auto *subFileAction = new SubFileAction(i);
            subFileListMenu->addAction(subFileAction);
            subFileActionList.append(subFileAction);
        }
    }
}

void SubFileViewer::set_statusbar(QStatusBar *bar) {
    statusBar = bar;
}

void SubFileViewer::show_message(const QString& message) {
    if (statusBar!= nullptr){
        statusBar->showMessage(message, 10000);
    }
}

void SubFileViewer::show_subfile_info() {
    auto *editor = new SubFileEditor(subFile);
    editor->setWindowIcon(AppIcon);
    editor->exec();
    if (editor->ok) {
        QString text = editor->subFileNameEditor->text();
        if (text.isEmpty()) {
            QMessageBox::warning(this, tr("问题"), tr("名称填写为空。"), QMessageBox::Yes);
            return;
        }
        subFile->name = text.toStdString();
        nameLabel->setText(text);
        emit subFileChanged(subFile);
        load_subFileMenu();
    }
}

void SubFileViewer::connect_group_widget_slot(ViewerGroupWidget *groupWidget) {
    connect(groupWidget->addButton,     SIGNAL(clicked()), this, SLOT(add_item()));
    connect(groupWidget->hideButton,   SIGNAL(clicked()), this, SLOT(hide_group()));
    connect(groupWidget->editButton,    SIGNAL(clicked()), this, SLOT(edit_group()));
    connect(groupWidget->removeButton,  SIGNAL(clicked()), this, SLOT(remove_group()));
}

void SubFileViewer::connect_item_widget_slot(ViewerItemWidget *itemWidget) {
    connect(itemWidget->editButton,     SIGNAL(clicked()), this, SLOT(edit_item()));
    connect(itemWidget->copyButton,     SIGNAL(clicked()), this, SLOT(copy_item()));
    connect(itemWidget->pasteButton,    SIGNAL(clicked()), this, SLOT(paste_item()));
    connect(itemWidget->removeButton,   SIGNAL(clicked()), this, SLOT(remove_item()));
    connect(itemWidget->resourceButton, SIGNAL(clicked()), this, SLOT(show_item_resource()));
    connect(itemWidget->textLabel, SIGNAL(textChanged()), this, SLOT(item_text_changed()));
}

QString SubFileViewer::get_clip_board_text() {
    QClipboard *clipboard = QApplication::clipboard();
    return clipboard->text();
}

void SubFileViewer::show_resource_item_switch(const ResItem &resItem) {
    switch (resItem.type) {
        case ResItem::EMPTY:
            break;
        case ResItem::IMAGE:
            show_image_resource(resItem);
            break;
        case ResItem::TEXT:
            show_text_resource(resItem);
            break;
        case ResItem::OTHER:
            break;
        case ResItem::AUDIO:
            show_audio_resource(resItem);
            break;
        case ResItem::VIDEO:
            show_video_resource(resItem);
            break;
    }
}

void SubFileViewer::show_image_resource(const ResItem &resItem) {
    QByteArray rb = to_qByteArray(resItem.byteRef->read());

    auto *viewer = new ImageViewer;
    viewer->set_image(QImage::fromData(rb));

    auto dialog = new BaseResourceViewerDialog(viewer, rb, Q_STR(resItem.name));
    dialog->setWindowIcon(ImageIcon);
    dialog->setWindowTitle(tr("图片资源: ") + QString::fromStdString(resItem.name));
    dialog->exec();

    delete viewer;
}

void SubFileViewer::show_text_resource(const ResItem &resItem) {
    QByteArray rb = to_qByteArray(resItem.byteRef->read());

    auto *viewer = new QPlainTextEdit;
    viewer->setPlainText(rb);

    auto dialog = new BaseResourceViewerDialog(viewer, rb, Q_STR(resItem.name));
    dialog->setWindowIcon(TextIcon);
    dialog->setWindowTitle(tr("文本资源: ") + QString::fromStdString(resItem.name));
    dialog->exec();

    delete viewer;
}

void SubFileViewer::show_audio_resource(const ResItem &resItem) {
    QByteArray rb = to_qByteArray(resItem.byteRef->read());

    auto *viewer = new AudioViewer;
    viewer->set_audio(rb);

    auto dialog = new BaseResourceViewerDialog(viewer, rb, Q_STR(resItem.name));
    dialog->setWindowIcon(AudioIcon);
    dialog->setWindowTitle(tr("音频资源: ") + QString::fromStdString(resItem.name));
    dialog->exec();

    delete viewer;
}

void SubFileViewer::show_video_resource(const ResItem &resItem) {
    QByteArray rb = to_qByteArray(resItem.byteRef->read());

    auto viewer = new VideoViewer;
    viewer->set_video(rb);

    BaseResourceViewerDialog dialog(viewer, rb, Q_STR(resItem.name));
    dialog.setWindowIcon(AudioIcon);
    dialog.setWindowTitle(tr("视频资源: ") + QString::fromStdString(resItem.name));
    dialog.exec();
    delete viewer;
}
