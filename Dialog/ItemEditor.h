//
// Created by 刘镇睿 on 2020/11/21.
//

#ifndef RACHEFILE_ITEMEDITOR_H
#define RACHEFILE_ITEMEDITOR_H

#include <QDialog>
#include <QString>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QLabel>
#include "RacheFile/FileItemObject.h"
#include "RacheFile/FileDocument.h"

class ItemEditor : public QDialog{
    Q_OBJECT;
public:
    ItemEditor(FileItemObject* itemObject, SubFile* s, FileDocument *d);
    QLineEdit *nameEdit;
    QPlainTextEdit *textEdit;
    QPushButton *cancelButton;
    QPushButton *okButton;
    QRadioButton *normalButton;
    QRadioButton *attentionButton;
    QRadioButton *doubtButton;
    QRadioButton *warningButton;
    QButtonGroup *buttonGroup;
    QDateTimeEdit *dateTimeEdit;
    QLineEdit *sourceEdit;
    QPushButton *resButton;

    QString resName;

    bool ok;
    FileDocument *document;
    SubFile *subFile;

    void initUI();

public slots:
    void okButtonPressed();
    void set_res();
};


#endif //RACHEFILE_ITEMEDITOR_H
