//
// Created by Rache on 2021/10/24.
//

#ifndef RACHEFILE_RESOURCEDIALOG_H
#define RACHEFILE_RESOURCEDIALOG_H

#include <QDialog>
#include <QTreeWidget>
#include <QComboBox>
#include <QPushButton>
#include <QLineEdit>
#include <QStackedWidget>
#include <QPainter>
#include <QTextEdit>
#include <QLabel>
#include <QPlainTextEdit>

#include "ToolFunction.h"
#include "RacheFile/SubFile.h"
#include "Viewer/ResourceViewer.h"
#include "globals.h"

class ResTreeWidgetItem : public QTreeWidgetItem {
public:
    explicit ResTreeWidgetItem(const ResItem& i) {
        resItem = i;
        setText(0, QString::fromStdString(i.name));
        if (i.byteRef->size != 0) {
            setText(1, size_to_string(i.byteRef->size));
        } else {
            setText(1, size_to_string(i.byteRef->bytes.size()));
        }
    };

    ResItem resItem;
};

class ComboDialog : public QDialog{
    Q_OBJECT
public:
    ComboDialog();

    QComboBox *comboBox;
    QComboBox *importTypeComboBox;
    QLineEdit *nameEdit;
    QPushButton *cancelButton;
    QPushButton *okButton;

    bool ok = false;

private slots:
    void okButton_clicked();
};

class ResourceDialog : public QDialog{
    Q_OBJECT

public:
    explicit ResourceDialog(SubFile *s);

    void set_subfile(SubFile *s);

    QTreeWidget *treeWidget;
    QTreeWidgetItem *imageTreeItem;
    QTreeWidgetItem *textTreeItem;
    QTreeWidgetItem *audioTreeItem;
    QTreeWidgetItem *videoTreeItem;
    QTreeWidgetItem *otherTreeItem;

    QStackedWidget *viewer;
    ImageViewer *pixmapViewer;
    AudioViewer *audioViewer;
    VideoViewer *videoViewer;
    QPlainTextEdit *textViewer;
    QLabel *emptyViewer;

    QPushButton *okButton;
    QPushButton *exportButton;
    QPushButton *importButton;
    QPushButton *removeButton;

    SubFile *subFile = nullptr;

    bool ok = false;
    bool unused = false;

private:
    void init_ui();
    void view_text(const ResItem& item) const;
    void view_image(const ResItem& item) const;
    void view_audio(const ResItem& item) const;
    void view_video(const ResItem& item) const;

    bool isSelectedDialog = false;

public slots:
    void import_resource();
    void remove_resource();
    void export_resource();
    void current_changed();
    void okButton_clicked();

    bool check(QTreeWidgetItem *i);
};

class SelectResourceDialog : public QDialog {
    Q_OBJECT
public:
    explicit SelectResourceDialog(const SubFile *subFile);
    ~SelectResourceDialog() override;

    QTreeWidget *treeWidget;
    QTreeWidgetItem *imageTreeItem;
    QTreeWidgetItem *textTreeItem;
    QTreeWidgetItem *audioTreeItem;
    QTreeWidgetItem *videoTreeItem;
    QTreeWidgetItem *otherTreeItem;

    QPushButton *okButton;
    QPushButton *cancelButton;
    QPushButton *unusedButton;

    bool ok = false;
    bool unused = false;

    QString get_selected() const;

public slots:
    void okButton_clicked();
    void unusedButton_clicked();

private:
    void init_ui();
};


#endif //RACHEFILE_RESOURCEDIALOG_H
