//
// Created by Rache on 2021/10/30.
//

#include "ResourceViewerDialog.h"

#include <QFileDialog>
#include <QMessageBox>
#include <QVBoxLayout>
#include <utility>

BaseResourceViewerDialog::BaseResourceViewerDialog(QWidget *v, QByteArray b, const QString& n) {
    resourceBytes = std::move(b);
    name = n;
    viewer = v;

    nameLabel       = new QLabel(n);
    closeButton     = new QPushButton(tr("关闭"));
    exportButton    = new QPushButton(tr("导出"));

    nameLabel->setObjectName("ResourceViewerDialogNameLabel");

    init_ui();

    connect(closeButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(exportButton, SIGNAL(clicked()), this, SLOT(export_resource()));
}

void BaseResourceViewerDialog::init_ui() {
    auto buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    buttonLayout->addWidget(closeButton);
    buttonLayout->addWidget(exportButton);

    auto mainLayout = new QVBoxLayout;
    mainLayout->addWidget(nameLabel);
    mainLayout->addWidget(viewer);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);
}

void BaseResourceViewerDialog::export_resource() {
    QString fn = QFileDialog::getSaveFileName(this, QObject::tr("导出"));
    if (fn.isEmpty()) return;

    QFile f(fn);
    if (!f.open(QFile::WriteOnly)) {
        QMessageBox::critical(this, QObject::tr("错误"), QObject::tr("写入文件失败。"));
        return;
    }

    f.write(resourceBytes);
    f.close();
}
