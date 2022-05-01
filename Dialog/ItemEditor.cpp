//
// Created by 刘镇睿 on 2020/11/21.
//

#include <QGridLayout>
#include <QLabel>
#include <QGroupBox>
#include <QDateTimeEdit>
#include "ResourceDialog.h"
#include "QDebug"
#include "ItemEditor.h"
#include "Editor.h"

#define Q_STR(s) QString::fromStdString(s)

ItemEditor::ItemEditor(FileItemObject* itemObject, SubFile* s, FileDocument *d) {
    nameEdit = new QLineEdit();
    textEdit = new QPlainTextEdit();
    dateTimeEdit = new QDateTimeEdit();
    sourceEdit = new QLineEdit();
    cancelButton = new QPushButton(tr("取消"));
    okButton = new QPushButton(tr("完成"));
    normalButton = new QRadioButton(tr("正常"));
    attentionButton = new QRadioButton(tr("注意"));
    doubtButton = new QRadioButton(tr("存疑"));
    warningButton = new QRadioButton(tr("警告"));
    resButton = new QPushButton(tr("设置资源"));
    warningButton->setChecked(true);
    buttonGroup = new QButtonGroup;

    buttonGroup->addButton(normalButton, 0);
    buttonGroup->addButton(doubtButton, 1);
    buttonGroup->addButton(attentionButton, 2);
    buttonGroup->addButton(warningButton, 3);
    buttonGroup->setExclusive(true);

    if (itemObject != nullptr) {
        nameEdit->setText(Q_STR(itemObject->name));
        textEdit->setPlainText(Q_STR(itemObject->text));
        dateTimeEdit->setDate(QDate(itemObject->year, itemObject->month, itemObject->day));
        sourceEdit->setText(Q_STR(itemObject->source));
        buttonGroup->button(itemObject->type)->setChecked(true);

        if (!itemObject->resName.empty()) {
            resButton->setText(Q_STR(itemObject->resName));
        }
        resName = Q_STR(itemObject->resName);
    } else {
        buttonGroup->button(0)->setChecked(true);
        dateTimeEdit->setDate(QDate::currentDate());
    }

    ok = false;
    document = d;
    subFile = s;

    initUI();

    connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonPressed()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(resButton, SIGNAL(clicked()), this, SLOT(set_res()));
}

void ItemEditor::initUI() {
    nameEdit->setPlaceholderText(tr("在此键入名称。"));
    textEdit->setPlaceholderText(tr("在此键入内容。"));
    sourceEdit->setPlaceholderText(tr("在此键入来源"));

    auto *label1 = new QLabel(tr("名称"));
    auto *label2 = new QLabel(tr("内容"));
    auto *label3 = new QLabel(tr("来源"));
    auto *label4 = new QLabel(tr("日期"));
    auto *label5 = new QLabel(tr("资源"));

    auto *layout1 = new QGridLayout;
    layout1->addWidget(label1, 0, 0);
    layout1->addWidget(nameEdit, 0, 1);
    layout1->addWidget(label2, 1, 0, Qt::AlignTop);
    layout1->addWidget(textEdit, 1, 1);
    layout1->addWidget(label3, 2, 0);
    layout1->addWidget(sourceEdit, 2, 1);
    layout1->addWidget(label4, 3, 0);
    layout1->addWidget(dateTimeEdit, 3, 1);
    layout1->addWidget(label5, 4, 0);
    layout1->addWidget(resButton, 4, 1);

    auto *groupBox = new QGroupBox;
    auto *layout2 = new QHBoxLayout;
    layout2->addWidget(normalButton);
    layout2->addWidget(attentionButton);
    layout2->addWidget(doubtButton);
    layout2->addWidget(warningButton);
    groupBox->setTitle(tr("标记"));
    groupBox->setLayout(layout2);

    auto *layout3 = new QHBoxLayout;
    layout3->addStretch(1);
    layout3->addWidget(cancelButton);
    layout3->addWidget(okButton);

    dateTimeEdit->setCalendarPopup(true);
    dateTimeEdit->setDisplayFormat("yyyy-MM-dd");

    auto *layout = new QVBoxLayout;
    layout->addLayout(layout1);
    layout->addWidget(groupBox);
    layout->addLayout(layout3);

    setLayout(layout);
    okButton->setDefault(true);
}

void ItemEditor::okButtonPressed() {
    ok = true;
    close();
}

void ItemEditor::set_res() {
    SelectResourceDialog dialog(subFile);
    dialog.setWindowIcon(windowIcon());
    dialog.setWindowTitle(tr("选择一个资源"));
    dialog.exec();

    if (!dialog.ok) return;
    if (dialog.unused) {
        resName = "";
        resButton->setText(tr("设置资源"));
    } else {
        resName = dialog.get_selected();
        resButton->setText(resName);
    }
}
