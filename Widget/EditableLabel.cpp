
//
// Created by Rache on 2022/2/26.
//

#include "EditableLabel.h"

#include <QEvent>
#include <QKeyEvent>
#include <QScrollBar>

#include "qdebug.h"

EditableLabel::EditableLabel() {
    textLabel = new QLabel;
    textEdit = new QLineEdit;
    textLabel->installEventFilter(this);
    textEdit->installEventFilter(this);

    addWidget(textLabel);
    addWidget(textEdit);
}

void EditableLabel::setText(const QString &text) {
    textLabel->setText(text);
}

void EditableLabel::setWordWrap(bool on) {
    textLabel->setWordWrap(on);
}

void EditableLabel::setToolTip(const QString &tooltip) {
    textLabel->setToolTip(tooltip);
    QWidget::setToolTip(tooltip);
}

bool EditableLabel::eventFilter(QObject *obj, QEvent *event) {
    if (obj == textLabel) {
        if (event->type() == QEvent::MouseButtonDblClick) {
            setCurrentIndex(1);
            textEdit->setText(textLabel->text());
            textEdit->setFocus();
            return false;
        }
    } else if (obj == textEdit) {
        if (event->type() == QEvent::FocusOut) {
            setCurrentIndex(0);
            textLabel->setText(textEdit->text());
            emit textChanged();
        } else if (event->type() == QEvent::KeyPress) {
            if (((QKeyEvent*)event)->key() == Qt::Key_Enter) {
                setCurrentIndex(0);
                textLabel->setText(textEdit->text());
                emit textChanged();
            }
        }
    }
    return QObject::eventFilter(obj, event);
}

QString EditableLabel::text() {
    return textLabel->text();
}
