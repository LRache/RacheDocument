//
// Created by 刘镇睿 on 2020/12/5.
//

#ifndef RACHEFILE_ORDERLISTWIDGET_H
#define RACHEFILE_ORDERLISTWIDGET_H

#include <QListWidget>
#include <QList>
#include <QDialog>
#include <QScrollArea>
#include <QPushButton>
#include <QVBoxLayout>
#include <QLabel>

class OrderListWidget : public QDialog{
    Q_OBJECT
public:
    OrderListWidget();
    void set_list(const QList<QString>& l);

    QPushButton *okButton;
    QPushButton *cancelButton;

    QList<QString> list;
    QVector<int> resultList;
    QScrollArea *scrollArea;

    int length=0;
    bool ok = false;
    void up(int i);
    void down(int i);

    QVBoxLayout *scrollWidgetLayout;

public slots:
    void okButton_pressed();
};


#endif //RACHEFILE_ORDERLISTWIDGET_H
