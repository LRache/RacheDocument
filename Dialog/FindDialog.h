//
// Created by Rache on 2021/10/23.
//

#ifndef RACHEFILE_FINDDIALOG_H
#define RACHEFILE_FINDDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>
#include <QComboBox>
#include "RacheFile/DocumentSearcher.h"

class FindOptionBox : public QWidget {
    Q_OBJECT;
public:
    explicit FindOptionBox(const QString& name);

    QLineEdit *lineEdit;
    QComboBox *optionBox;
    QLabel *nameLabel;

    SearchType get_search_type() const;

};

class FindDialog : public QDialog{
    Q_OBJECT
public:
    explicit FindDialog(FileDocument *d);

    FindOptionBox *subFileSearchOptionBox;
    FindOptionBox *groupSearchOptionBox;
    FindOptionBox *itemSearchOptionBox;
    QPushButton *searchButton;

    QTableWidget *searchTable;
    QLabel *resultLabel;

    FileDocument *document;

public slots:
    void search_button_clicked();

private:
    QString typeNames[4];
};


#endif //RACHEFILE_FINDDIALOG_H
