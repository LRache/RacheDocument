//
// Created by Rache on 2021/10/23.
//

#include "FindDialog.h"
#include "globals.h"

#include <QVBoxLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <iostream>

FindOptionBox::FindOptionBox(const QString& name) {
    lineEdit = new QLineEdit();
    optionBox = new QComboBox;
    nameLabel = new QLabel(name);

    auto *l = new QVBoxLayout;
    l->addWidget(nameLabel);
    l->addWidget(lineEdit);
    l->addWidget(optionBox);
    setLayout(l);

    for (const auto & FindTypeName : FindTypeNames) {
        optionBox->addItem(FindTypeName);
    }
}

SearchType FindOptionBox::get_search_type() const {
    return (SearchType)optionBox->currentIndex();
}

FindDialog::FindDialog(FileDocument *d) {
    document = d;

    subFileSearchOptionBox  = new FindOptionBox(tr("子文件"));
    groupSearchOptionBox    = new FindOptionBox(tr("组"));
    itemSearchOptionBox     = new FindOptionBox(tr("项"));
    searchButton            = new QPushButton(tr("搜索"));

    searchTable = new QTableWidget;
    resultLabel = new QLabel;

    searchTable->setColumnCount(8);
    searchTable->setHorizontalHeaderLabels(FindTableHeader);
    searchTable->setEditTriggers(QTableWidget::NoEditTriggers);
    searchTable->setSelectionBehavior(QTableWidget::SelectRows);

    auto *l = new QHBoxLayout;
    l->addWidget(subFileSearchOptionBox);
    l->addWidget(groupSearchOptionBox);
    l->addWidget(itemSearchOptionBox);

    auto * l2 = new QHBoxLayout;
    l2->addStretch();
    l2->addWidget(searchButton);

    auto *l3 = new QVBoxLayout;
    l3->addLayout(l);
    l3->addLayout(l2);

    auto *l4 = new QVBoxLayout;
    l4->addWidget(searchTable);
    l4->addWidget(resultLabel);

    auto *optionGroupBox = new QGroupBox(tr("搜索选项"));
    optionGroupBox->setLayout(l3);
    auto *resultGroupBox = new QGroupBox(tr("搜索结果"));
    resultGroupBox->setLayout(l4);

    auto *mainLayout = new QVBoxLayout;
    mainLayout->addWidget(optionGroupBox);
    mainLayout->addWidget(resultGroupBox);
    setLayout(mainLayout);

    connect(searchButton, SIGNAL(clicked()), this, SLOT(search_button_clicked()));
}

void FindDialog::search_button_clicked() {
    QString s1 = subFileSearchOptionBox->lineEdit->text();
    QString s2 = groupSearchOptionBox->lineEdit->text();
    QString s3 = itemSearchOptionBox->lineEdit->text();

    if (s1.isEmpty() and subFileSearchOptionBox->get_search_type() != ALL) {
        QMessageBox::warning(this, tr("问题"), tr("子文件的关键字为空。"));
        return;
    }if (s2.isEmpty() and groupSearchOptionBox->get_search_type() != ALL) {
        QMessageBox::warning(this, tr("问题"), tr("组的关键字为空。"));
        return;
    }if (s3.isEmpty() and itemSearchOptionBox->get_search_type() != ALL) {
        QMessageBox::warning(this, tr("问题"), tr("项的关键字为空。"));
        return;
    }

    DocumentSearcher searcher(document);
    searcher.set_subfile_search(subFileSearchOptionBox->get_search_type(), s1.toStdString());
    searcher.set_group_search(groupSearchOptionBox->get_search_type(), s2.toStdString());
    searcher.set_item_search(itemSearchOptionBox->get_search_type(), s3.toStdString());

    double t;
    std::vector<SearchResult> results = searcher.search(&t);
    searchTable->setRowCount(0);
    searchTable->setRowCount((int)results.size());
    int row = 0;
    for (auto result : results) {
        searchTable->setItem(row, 0, new QTableWidgetItem(QString::fromStdString(result.subFile->name)));
        searchTable->setItem(row, 1, new QTableWidgetItem(QString::fromStdString(result.group->name)));
        searchTable->setItem(row, 2, new QTableWidgetItem(QString::fromStdString(result.item->name)));
        searchTable->setItem(row, 3, new QTableWidgetItem(QString::fromStdString(result.item->text)));
        searchTable->setItem(row, 4, new QTableWidgetItem(QString::fromStdString(result.item->source)));
        searchTable->setItem(row, 5, new QTableWidgetItem(ItemTypeNames[result.item->type]));
        searchTable->setItem(row, 6, new QTableWidgetItem(QString("%1-%2-%3").arg(result.item->year).arg(result.item->month).arg(result.item->day)));
        searchTable->setItem(row, 7, new QTableWidgetItem(QString::fromStdString(result.item->resName)));
        row++;
    }

    QString text = tr("共%1个结果。在%2秒内完成搜索。");
    text = text.arg(row);
    text = text.arg(t/ 1000);
    resultLabel->setText(text);
}
