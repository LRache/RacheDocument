//
// Created by Rache on 2021/10/24.
//

#include "ResourceDialog.h"
#include "globals.h"
#include "ToolFunction.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QFileDialog>
#include <QSplitter>
#include <QFormLayout>
#include <QApplication>
#include <QClipboard>
#include <QBuffer>

ComboDialog::ComboDialog() {
    comboBox        = new QComboBox;
    importTypeComboBox = new QComboBox;
    nameEdit        = new QLineEdit;
    cancelButton    = new QPushButton(tr("取消"));
    okButton        = new QPushButton(tr("确定"));

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    auto formLayout = new QFormLayout;
    formLayout->addRow(tr("名称"), nameEdit);
    formLayout->addRow(tr("类型"), comboBox);
    formLayout->addRow(tr("导入方式"), importTypeComboBox);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addLayout(formLayout);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    okButton->setFocus();

    connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));

    importTypeComboBox->addItem(tr("从文件导入"));
    importTypeComboBox->addItem(tr("从剪切板导入"));
}

void ComboDialog::okButton_clicked() {
    ok = true;
    close();
}

ResourceDialog::ResourceDialog(SubFile *s) {
    treeWidget = new QTreeWidget;
    imageTreeItem   = new QTreeWidgetItem;
    textTreeItem    = new QTreeWidgetItem;
    audioTreeItem   = new QTreeWidgetItem;
    videoTreeItem   = new QTreeWidgetItem;
    otherTreeItem   = new QTreeWidgetItem;

    imageTreeItem->setText(0, ResourceTypeTexts[0]);
    textTreeItem->setText(0, ResourceTypeTexts[1]);
    audioTreeItem->setText(0, ResourceTypeTexts[2]);
    videoTreeItem->setText(0, ResourceTypeTexts[3]);
    otherTreeItem->setText(0, ResourceTypeTexts[4]);

    imageTreeItem->setIcon(0, ImageIcon);
    textTreeItem->setIcon(0, TextIcon);
    audioTreeItem->setIcon(0, AudioIcon);
    videoTreeItem->setIcon(0, VideoIcon);
    otherTreeItem->setIcon(0, OtherIcon);

    treeWidget->addTopLevelItem(imageTreeItem);
    treeWidget->addTopLevelItem(textTreeItem);
    treeWidget->addTopLevelItem(audioTreeItem);
    treeWidget->addTopLevelItem(videoTreeItem);
    treeWidget->addTopLevelItem(otherTreeItem);

    viewer          = new QStackedWidget;
    emptyViewer     = new QLabel(QObject::tr("预览"));
    pixmapViewer    = new ImageViewer;
    audioViewer     = new AudioViewer;
    videoViewer     = new VideoViewer;
    textViewer      = new QPlainTextEdit;

    okButton        = new QPushButton(QObject::tr("完成"));
    exportButton    = new QPushButton(QObject::tr("导出"));
    importButton    = new QPushButton(QObject::tr("导入"));
    removeButton    = new QPushButton(QObject::tr("删除"));

    set_subfile(s);

    init_ui();

    connect(treeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem *, QTreeWidgetItem *)), this, SLOT(current_changed()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));
    connect(exportButton, SIGNAL(clicked()), this, SLOT(export_resource()));
    connect(importButton, SIGNAL(clicked()), this, SLOT(import_resource()));
    connect(removeButton, SIGNAL(clicked()), this, SLOT(remove_resource()));

    okButton->setFocus();
}

void ResourceDialog::init_ui() {
    emptyViewer->setAlignment(Qt::AlignCenter);
    textViewer->setReadOnly(true);

    viewer->addWidget(emptyViewer);
    viewer->addWidget(pixmapViewer);
    viewer->addWidget(textViewer);
    viewer->addWidget(audioViewer);
    viewer->addWidget(videoViewer);
    viewer->setCurrentIndex(0);

    auto splitter = new QSplitter;
    splitter->addWidget(treeWidget);
    splitter->addWidget(viewer);

    auto l2 = new QHBoxLayout;
    l2->addStretch();
    l2->addWidget(okButton);
    l2->addWidget(exportButton);
    l2->addWidget(importButton);
    l2->addWidget(removeButton);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(splitter);
    mainLayout->addLayout(l2);
    setLayout(mainLayout);

    treeWidget->setColumnCount(2);
    treeWidget->setHeaderLabels(QStringList()<<tr("名称")<<tr("大小"));

    setWindowTitle(QObject::tr("资源"));
}

void ResourceDialog::set_subfile(SubFile *s) {
    subFile = s;

    for (const auto& i : s->resourceSet.res) {
        auto *treeItem = new ResTreeWidgetItem(i.second);

        switch (i.second.type) {

            case ResItem::EMPTY:
                break;
            case ResItem::IMAGE:
                imageTreeItem->addChild(treeItem);
                break;
            case ResItem::OTHER:
                otherTreeItem->addChild(treeItem);
                break;
            case ResItem::TEXT:
                textTreeItem->addChild(treeItem);
                break;
            case ResItem::AUDIO:
                audioTreeItem->addChild(treeItem);
                break;
            case ResItem::VIDEO:
                videoTreeItem->addChild(treeItem);
                break;
        }
    }
}

void ResourceDialog::import_resource() {
    ComboDialog comboDialog;
    comboDialog.setWindowIcon(windowIcon());
    comboDialog.comboBox->addItems(ResourceTypeTexts);
    comboDialog.comboBox->setItemIcon(0, ImageIcon);
    comboDialog.comboBox->setItemIcon(1, TextIcon);
    comboDialog.comboBox->setItemIcon(2, AudioIcon);
    comboDialog.comboBox->setItemIcon(3, VideoIcon);
    comboDialog.comboBox->setItemIcon(4, OtherIcon);

    auto current = treeWidget->currentItem();
    if (current != nullptr) {
        if (current == imageTreeItem) {
            comboDialog.comboBox->setCurrentIndex(0);
        } else if (current == textTreeItem) {
            comboDialog.comboBox->setCurrentIndex(1);
        } else if (current == audioTreeItem) {
            comboDialog.comboBox->setCurrentIndex(2);
        } else if (current == videoTreeItem) {
            comboDialog.comboBox->setCurrentIndex(3);
        } else if (current == otherTreeItem) {
            comboDialog.comboBox->setCurrentIndex(4);
        }
    }

    comboDialog.exec();

    if (!comboDialog.ok) return;
    QString name = comboDialog.nameEdit->text();
    std::string stdName = name.toStdString();
    if (subFile->resourceSet.res.find(stdName) != subFile->resourceSet.res.end()) {
        QMessageBox::warning(this, tr("问题"), tr("重复的名称"));
        return;
    }

    RLib::RBytes rb;
    auto t = (ResItem::ResType)(comboDialog.comboBox->currentIndex() + 1);
    if (comboDialog.importTypeComboBox->currentIndex() == 0) {
        QString filename = QFileDialog::getOpenFileName(this, tr("打开资源文件"), ".");
        if (filename.isEmpty()) return;

        QFile file(filename);
        if (!file.open(QFile::ReadOnly)) {
            QMessageBox::critical(this, tr("错误"), tr("打开文件失败。"));
            return;
        }

        QByteArray byteArray = file.readAll();
        rb.write(byteArray.data(), byteArray.size());
    } else {
        QClipboard *clipboard = QApplication::clipboard();
        if (t == ResItem::TEXT) {
            std::string str = clipboard->text().toStdString();
            rb.write(str);
            rb.write('\0');
        } else if (t == ResItem::IMAGE) {
            QPixmap p = clipboard->pixmap();
            if (p.isNull()) {
                QMessageBox::warning(this, tr("问题"), tr("剪切板中图片为空。"));
                return;
            }

            QByteArray byteArray;
            QBuffer buffer(&byteArray);
            buffer.open(QIODevice::WriteOnly);
            p.save(&buffer, "PNG");
            buffer.close();

            rb.write(byteArray.data(), byteArray.size());
        } else {
            return;
        }
    }

    auto *treeItem = new ResTreeWidgetItem(subFile->resourceSet.add(stdName, rb, t));
    switch (t) {

        case ResItem::EMPTY:
            break;
        case ResItem::IMAGE:
            imageTreeItem->addChild(treeItem);
            break;
        case ResItem::OTHER:
            otherTreeItem->addChild(treeItem);
            break;
        case ResItem::TEXT:
            textTreeItem->addChild(treeItem);
            break;
        case ResItem::AUDIO:
            audioTreeItem->addChild(treeItem);
            break;
        case ResItem::VIDEO:
            videoTreeItem->addChild(treeItem);
            break;
    }
}

void ResourceDialog::remove_resource() {
    QTreeWidgetItem *current = treeWidget->currentItem();
    if (!check(current)) return;

    QMessageBox::Button button = QMessageBox::question(this, tr("删除"), tr("确定要删除这个资源吗？"));
    if (button != QMessageBox::Button::Yes) return;

    auto *resTreeWidgetItem = (ResTreeWidgetItem*)current;
    subFile->resourceSet.remove(resTreeWidgetItem->resItem.name);
    switch (resTreeWidgetItem->resItem.type) {

        case ResItem::EMPTY:
            break;
        case ResItem::IMAGE:
            imageTreeItem->removeChild(current);
            break;
        case ResItem::OTHER:
            otherTreeItem->removeChild(current);
            break;
        case ResItem::TEXT:
            textTreeItem->removeChild(current);
            break;
        case ResItem::AUDIO:
            audioTreeItem->removeChild(current);
            break;
        case ResItem::VIDEO:
            videoTreeItem->removeChild(current);
            break;
    }
}

void ResourceDialog::current_changed() {
    QTreeWidgetItem *current = treeWidget->currentItem();
    if (current == nullptr
        or current == imageTreeItem
        or current == otherTreeItem
        or current == textTreeItem
        or current == audioTreeItem
        or current == videoTreeItem
        ) {
        viewer->setCurrentIndex(0);
        return;
    }
    auto *resTreeWidgetItem = (ResTreeWidgetItem*)current;
    switch (resTreeWidgetItem->resItem.type) {

        case ResItem::EMPTY:
            viewer->setCurrentIndex(0);
            break;
        case ResItem::IMAGE:
            view_image(resTreeWidgetItem->resItem);
            break;
        case ResItem::TEXT:
            view_text(resTreeWidgetItem->resItem);
            break;
        case ResItem::OTHER:
            viewer->setCurrentIndex(0);
            break;
        case ResItem::AUDIO:
            view_audio(resTreeWidgetItem->resItem);
            break;
        case ResItem::VIDEO:
            view_video(resTreeWidgetItem->resItem);
            break;
    }
}

void ResourceDialog::view_text(const ResItem& item) const {
    RLib::RBytes rb = item.byteRef->read();
    char s[rb.size() + 1];
    rb.read_string(s);
    textViewer->setPlainText(s);
    viewer->setCurrentIndex(2);
}

void ResourceDialog::view_image(const ResItem& item) const {
    pixmapViewer->set_image(QImage::fromData(to_qByteArray(item.byteRef->read())));
    viewer->setCurrentIndex(1);
}

void ResourceDialog::view_audio(const ResItem &item) const {
    audioViewer->set_audio(to_qByteArray(item.byteRef->read()));
    viewer->setCurrentIndex(3);
}

void ResourceDialog::view_video(const ResItem &item) const {
    videoViewer->set_video(to_qByteArray(item.byteRef->read()));
    viewer->setCurrentIndex(4);
}

void ResourceDialog::okButton_clicked() {
    if (!isSelectedDialog) {
        close();
        return;
    }

    QTreeWidgetItem *current = treeWidget->currentItem();
    if (current == nullptr
        or current == imageTreeItem
        or current == otherTreeItem
        or current == textTreeItem) {
        QMessageBox::warning(this, tr("问题"), tr("未选择资源项。"));
        return;
    }
    ok = true;
    close();
}

bool ResourceDialog::check(QTreeWidgetItem *i) {
    if (i == nullptr
        or i == imageTreeItem
        or i == otherTreeItem
        or i == textTreeItem
        or i == audioTreeItem
        or i == videoTreeItem) {
        QMessageBox::warning(this, tr("问题"), tr("未选择资源项。"));
        return false;
    }
    return true;
}

void ResourceDialog::export_resource() {
    QTreeWidgetItem *current = treeWidget->currentItem();
    if (!check(current)) return;

    QString fn = QFileDialog::getSaveFileName(this, QObject::tr("导出"));
    if (fn.isEmpty()) return;
    auto *resTreeWidgetItem = (ResTreeWidgetItem*)current;

    std::ofstream ofstream;
    std::string filename(fn.toLocal8Bit());
    ofstream.open(filename, std::ios::binary);
    if (!ofstream.is_open()) {
        QMessageBox::critical(this, QObject::tr("错误"), QObject::tr("写入文件失败。"));
        return;
    }

    resTreeWidgetItem->resItem.byteRef->writeStream(ofstream);
    ofstream.close();
    QMessageBox::information(this, QObject::tr("导出"), QObject::tr("已导出资源到")+fn);
}

SelectResourceDialog::SelectResourceDialog(const SubFile *subFile) {
    treeWidget = new QTreeWidget;
    imageTreeItem   = new QTreeWidgetItem;
    textTreeItem    = new QTreeWidgetItem;
    audioTreeItem   = new QTreeWidgetItem;
    videoTreeItem   = new QTreeWidgetItem;
    otherTreeItem   = new QTreeWidgetItem;

    imageTreeItem->setText(0, ResourceTypeTexts[0]);
    textTreeItem->setText(0, ResourceTypeTexts[1]);
    audioTreeItem->setText(0, ResourceTypeTexts[2]);
    videoTreeItem->setText(0, ResourceTypeTexts[3]);
    otherTreeItem->setText(0, ResourceTypeTexts[4]);

    imageTreeItem->setIcon(0, ImageIcon);
    textTreeItem->setIcon(0, TextIcon);
    audioTreeItem->setIcon(0, AudioIcon);
    videoTreeItem->setIcon(0, VideoIcon);
    otherTreeItem->setIcon(0, OtherIcon);

    treeWidget->addTopLevelItem(imageTreeItem);
    treeWidget->addTopLevelItem(textTreeItem);
    treeWidget->addTopLevelItem(audioTreeItem);
    treeWidget->addTopLevelItem(videoTreeItem);
    treeWidget->addTopLevelItem(otherTreeItem);

    okButton = new QPushButton(QObject::tr("确定"));
    cancelButton = new QPushButton(QObject::tr("取消"));
    unusedButton = new QPushButton(QObject::tr("不使用资源"));

    init_ui();

    for (const auto& i : subFile->resourceSet.res) {
        auto *treeItem = new ResTreeWidgetItem(i.second);

        switch (i.second.type) {

            case ResItem::EMPTY:
                break;
            case ResItem::IMAGE:
                imageTreeItem->addChild(treeItem);
                break;
            case ResItem::OTHER:
                otherTreeItem->addChild(treeItem);
                break;
            case ResItem::TEXT:
                textTreeItem->addChild(treeItem);
                break;
            case ResItem::AUDIO:
                audioTreeItem->addChild(treeItem);
                break;
            case ResItem::VIDEO:
                videoTreeItem->addChild(treeItem);
                break;
        }
    }

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_clicked()));
    connect(unusedButton, SIGNAL(clicked()), this, SLOT(unusedButton_clicked()));
}

void SelectResourceDialog::init_ui() {
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(okButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(unusedButton);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(treeWidget);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    treeWidget->setColumnCount(2);
    treeWidget->setHeaderLabels(QStringList()<<tr("名称")<<tr("大小"));
}

QString SelectResourceDialog::get_selected() const {
    if (unused) return QString();

    QTreeWidgetItem *current = treeWidget->currentItem();
    if (current == nullptr
        or current == imageTreeItem
        or current == otherTreeItem
        or current == textTreeItem
        or current == audioTreeItem
        or current == videoTreeItem) {
        return QString();
    }

    return current->text(0);
}

SelectResourceDialog::~SelectResourceDialog() {
    delete imageTreeItem;
    delete textTreeItem;
    delete audioTreeItem;
    delete videoTreeItem;
    delete otherTreeItem;
    delete treeWidget;
}

void SelectResourceDialog::okButton_clicked() {
    ok = true;
    close();
}

void SelectResourceDialog::unusedButton_clicked() {
    ok = false;
    unused = true;
    close();
}
