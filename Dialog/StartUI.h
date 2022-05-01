//
// Created by 刘镇睿 on 2020/12/4.
//

#ifndef RACHEFILE_STARTUI_H
#define RACHEFILE_STARTUI_H

#include <QDialog>
#include <QToolButton>

#include "mainwindow.h"

class StartUI : public QDialog {
    Q_OBJECT
public:
    StartUI();
    QToolButton *newFileButton;
    QToolButton *openFileButton;

    void initUI();

public slots:
    void open_file();
    void open_file(QAction*);
    void new_file();
};


#endif //RACHEFILE_STARTUI_H
