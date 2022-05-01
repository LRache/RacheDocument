//
// Created by Rache on 2021/11/21.
//

#ifndef RACHEFILE_PROGRESSBARDIALOG_H
#define RACHEFILE_PROGRESSBARDIALOG_H

#include <QProgressDialog>

class ProgressBarDialog : public QProgressDialog{
public:
    explicit ProgressBarDialog(const int &max, const QString &text);

    void operator++();

private:
    int p = 0;
};


#endif //RACHEFILE_PROGRESSBARDIALOG_H
