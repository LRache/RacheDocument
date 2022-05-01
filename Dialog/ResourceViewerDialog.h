//
// Created by Rache on 2021/10/30.
//

#ifndef RACHEFILE_RESOURCEVIEWERDIALOG_H
#define RACHEFILE_RESOURCEVIEWERDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>

class BaseResourceViewerDialog : public QDialog {
    Q_OBJECT
public:
    BaseResourceViewerDialog(QWidget *viewer, QByteArray b, const QString& n);

    QLabel *nameLabel;
    QPushButton *closeButton;
    QPushButton *exportButton;

private:
    QByteArray resourceBytes;
    QString name;
    QWidget *viewer;

    void init_ui();

private slots:
    void export_resource();
};

#endif //RACHEFILE_RESOURCEVIEWERDIALOG_H
