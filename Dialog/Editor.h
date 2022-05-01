//
// Created by 刘镇睿 on 2020/12/6.
//

#ifndef RACHEFILE_EDITOR_H
#define RACHEFILE_EDITOR_H

#include "RacheFile/FileDocument.h"
#include "Dialog/FileDocumentTree.h"
#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>
#include <QLabel>

class DocumentEditor : public QDialog{
    Q_OBJECT
public:
    explicit DocumentEditor(FileDocument *document);
    QLabel *counterLabel;

private:
    FileDocument *fileDocument;
};

class SubFileEditor : public QDialog{
    Q_OBJECT;
public:
    explicit SubFileEditor(SubFile* s);
    QLineEdit *subFileNameEditor;
    QLabel *counterLabel;
    QPushButton *okButton;
    QPushButton *cancelButton;
    bool ok=false;

private:
    SubFile *subFile;

public slots:
    void okButton_pressed();
};

class LinkEditor : public QDialog {
    Q_OBJECT;
public:
    explicit LinkEditor(FileDocument *fileDocument);

    QCheckBox *usedLinkButton;
    FileDocumentTree *tree;
    QPushButton *okButton;
    QPushButton *cancelButton;

    bool ok = false;
    bool use = true;
    ItemId id;

public slots:
    void okButton_pressed();
    void use_link(bool b);
};

#endif //RACHEFILE_EDITOR_H
