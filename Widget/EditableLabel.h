//
// Created by Rache on 2022/2/26.
//

#ifndef RACHEFILE_EDITABLELABEL_H
#define RACHEFILE_EDITABLELABEL_H

#include <QStackedWidget>
#include <QLabel>
#include <QLineEdit>

class EditableLabel : public QStackedWidget{
    Q_OBJECT;

public:
    EditableLabel();

    QLabel *textLabel;
    QLineEdit *textEdit;

    QString text();
    void setText(const QString &text);
    void setWordWrap(bool b);
    void setToolTip(const QString &tooltip);

    bool eventFilter(QObject *obj, QEvent *event) override;

signals:
    void textChanged();
};


#endif //RACHEFILE_EDITABLELABEL_H
