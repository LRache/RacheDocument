//
// Created by 刘镇睿 on 2020/12/5.
//

#include "OrderListWidget.h"
#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>

#include "globals.h"

#if defined(__APPLE__) && (defined(__GNUC__) || defined(__xlC__) || defined(__xlc__))
#define TEXT_SIZE 12
#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define TEXT_SIZE 10
#endif


OrderListWidget::OrderListWidget() {
    scrollArea = new QScrollArea;
    okButton = new QPushButton(tr("完成"));
    cancelButton = new QPushButton(tr("取消"));
    okButton->setDefault(true);

    auto  *hBoxLayout = new QHBoxLayout;
    hBoxLayout->addStretch(1);
    hBoxLayout->addWidget(cancelButton);
    hBoxLayout->addWidget(okButton);

    auto *vBoxLayout = new QVBoxLayout;
    vBoxLayout->addWidget(new QLabel("调整顺序。"));
    vBoxLayout->addWidget(scrollArea);
    vBoxLayout->addLayout(hBoxLayout);
    setLayout(vBoxLayout);
    setFixedSize(400, 600);

    connect(okButton, SIGNAL(clicked()), this, SLOT(okButton_pressed()));
    connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
}

void OrderListWidget::set_list(const QList<QString>& l) {
    QIcon upIcon(RESOURCE_PATH(/icon/up.png));
    QIcon downIcon(RESOURCE_PATH(/icon/down.png));

    int i = 0;
    list = l;
    const QFont font("Microsoft YaHei", TEXT_SIZE);
    const int width = 250;
    QFontMetrics fontMetrics(font);

    auto *scrollWidget = new QWidget;
    scrollWidgetLayout = new QVBoxLayout;
    for (const QString& text : l) {
        resultList.append(i);

        int fontSize = fontMetrics.width(text);
        QString str = text;
        if(fontSize > width) str = fontMetrics.elidedText(text, Qt::ElideRight, width);

        auto *widget = new QWidget();
        auto *upButton = new QToolButton();
        upButton->setIcon(upIcon);
        upButton->setFixedSize(20, 20);
        auto *downButton = new QToolButton();
        downButton->setIcon(downIcon);
        downButton->setFixedSize(20, 20);
        auto *textLabel = new QLabel();
        textLabel->setToolTip(text);
        textLabel->setFont(font);
        textLabel->setFixedWidth(width);
        textLabel->setText(str);

        auto *layout = new QHBoxLayout;
        layout->addWidget(textLabel);
        layout->addWidget(upButton);
        layout->addWidget(downButton);
        widget->setLayout(layout);
        widget->setStyleSheet("background-color: rgb(221, 224, 230)");
        scrollWidgetLayout->addWidget(widget);

        connect(upButton, &QToolButton::clicked, [=](){up(i);});
        connect(downButton, &QToolButton::clicked, [=](){down(i);});

        i++;
    }
    scrollWidgetLayout->addStretch(1);
    scrollWidget->setLayout(scrollWidgetLayout);
    scrollArea->setWidgetResizable(true);
    scrollArea->setWidget(scrollWidget);
    length = i;
}

void OrderListWidget::up(int i) {
    int index = resultList.indexOf(i);
    if (index - 1 < 0) return;
    resultList.removeOne(i);
    resultList.insert(index - 1, i);

    QWidget* widget = scrollWidgetLayout->itemAt(index)->widget();
    scrollWidgetLayout->removeWidget(widget);
    scrollWidgetLayout->insertWidget(index - 1, widget);
}

void OrderListWidget::down(int i) {
    int index = resultList.indexOf(i);
    if (index + 1 >= length) return;
    resultList.removeOne(i);
    resultList.insert(index + 1, i);

    QWidget* widget = scrollWidgetLayout->itemAt(index)->widget();
    scrollWidgetLayout->removeWidget(widget);
    scrollWidgetLayout->insertWidget(index + 1, widget);
}

void OrderListWidget::okButton_pressed() {
    ok = true;
    close();
}
