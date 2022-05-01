//
// Created by Rache on 2021/11/21.
//

#include "ProgressBarDialog.h"

ProgressBarDialog::ProgressBarDialog(const int &max, const QString &text) {
    setMaximum(max);
    setValue(0);
    setLabelText(text);
}

void ProgressBarDialog::operator++() {
    p++;
    setValue(p);
}
