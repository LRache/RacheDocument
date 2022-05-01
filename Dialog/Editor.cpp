//
// Created by 刘镇睿 on 2020/12/6.
//

#include "Editor.h"
#include "ToolFunction.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QGroupBox>
#include <QMessageBox>
#include <QDebug>

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#define TEXT_SIZE 12
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define TEXT_SIZE 10
#endif

DocumentEditor::DocumentEditor(FileDocument *document) {
    fileDocument = document;

    SubFileCounter subFileCounter = document->count();
    counterLabel = new QLabel();
    counterLabel->setText(tr("子文件数：%1个\n组数：%2个\n\n总计项：%3项\n普通项：%4项\n存疑项：%5项\n注意项：%6项\n警告项：%7项\n\n资源数：%8个\n资源大小：%9")
    .arg(subFileCounter.subFile)
    .arg(subFileCounter.group)
    .arg(subFileCounter.itemCounter.total)
    .arg(subFileCounter.itemCounter.normal)
    .arg(subFileCounter.itemCounter.doubt)
    .arg(subFileCounter.itemCounter.attention)
    .arg(subFileCounter.itemCounter.warning)
    .arg(subFileCounter.resource)
    .arg(size_to_string(subFileCounter.resourceSize))
    );

    auto *groupBox = new QGroupBox;
    auto *layout1 = new QVBoxLayout;
    layout1->addWidget(counterLabel);
    groupBox->setTitle(tr("统计"));
    groupBox->setLayout(layout1);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(groupBox);
    setLayout(mainLayout);
}

SubFileEditor::SubFileEditor(SubFile *s) {
    subFile = s;
    subFileNameEditor = new QLineEdit;
    counterLabel = new QLabel;
    okButton = new QPushButton(tr("完成"));
    cancelButton = new QPushButton(tr("取消"));

    QFont font = QFont("Microsoft YaHei", 12);
    subFileNameEditor->setText(QString::fromStdString(s->name));
    subFileNameEditor->setPlaceholderText(tr("在此键入子文件名"));
    subFileNameEditor->setFont(font);
    GroupCounter groupCounter = s->count();
    counterLabel->setText(tr("组数：%1个\n\n总计项：%2项\n普通项：%3项\n存疑项：%4项\n注意项：%5项\n警告项：%6项")
                                  .arg(groupCounter.group)
                                  .arg(groupCounter.itemCounter.total)
                                  .arg(groupCounter.itemCounter.normal)
                                  .arg(groupCounter.itemCounter.doubt)
                                  .arg(groupCounter.itemCounter.attention)
                                  .arg(groupCounter.itemCounter.warning)
    );
    counterLabel->setFont(font);
    okButton->setDefault(true);

    auto *groupBox1 = new QGroupBox;
    groupBox1->setFont(font);
    groupBox1->setTitle(tr("名称"));
    auto *layout1 = new QVBoxLayout;
    layout1->addWidget(subFileNameEditor);
    groupBox1->setLayout(layout1);

    auto *groupBox2 = new QGroupBox;
    groupBox2->setFont(font);
    groupBox2->setTitle(tr("统计"));
    auto *layout2 = new QVBoxLayout;
    layout2->addWidget(counterLabel);
    groupBox2->setLayout(layout2);

    auto *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(groupBox1);
    mainLayout->addWidget(groupBox2);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_pressed()));
}

void SubFileEditor::okButton_pressed() {
    ok = true;
    close();
}

LinkEditor::LinkEditor(FileDocument *fileDocument) {
    usedLinkButton = new QCheckBox(tr("启用链接"));
    tree = new FileDocumentTree(fileDocument);
    okButton = new QPushButton(tr("完成"));
    cancelButton = new QPushButton(tr("取消"));

    usedLinkButton->setChecked(true);

    auto textLabel = new QLabel(tr("选择一个要链接到的项。"));

    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(okButton);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(usedLinkButton);
    mainLayout->addWidget(textLabel);
    mainLayout->addWidget(tree);
    mainLayout->addLayout(buttonLayout);
    setLayout(mainLayout);

    connect(usedLinkButton, SIGNAL(clicked(bool)), this, SLOT(use_link(bool)));
    connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_pressed()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void LinkEditor::use_link(bool b) {
    use = b;
    tree->setEnabled(b);
}

void LinkEditor::okButton_pressed() {
    id = tree->get_selected();
    if (use and id.subfile == -1) {
        QMessageBox::warning(this, tr("问题"), tr("未选中项。"));
    } else {
        ok = true;
        id = tree->get_selected();
        close();
    }
}
