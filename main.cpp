#include <QApplication>
#include <QDir>
#include <iostream>

#include "Dialog/StartUI.h"
#include "init.h"

int main(int argc, char *argv[])
{
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::PassThrough);
    QApplication app(argc, argv);
    QDir::setCurrent(QApplication::applicationDirPath());
    QApplication::setStyle("fusion");
    QApplication::setApplicationDisplayName("档案");
    QApplication::setApplicationName("Files");
    QApplication::setApplicationVersion("2.1");
    QApplication::setOrganizationDomain("Rache");
    QApplication::setOrganizationName("Rache.com");

    QFile file(RESOURCE_PATH(stylesheet/application.txt));
    if (file.open(QFile::ReadOnly)) {
        app.setStyleSheet(QString::fromLocal8Bit(file.readAll()));
    }

    init_globals();

    if (argc > 1) {
        std::string fp;
        for (int i = 0; argv[1][i] != '\0'; i++) {
            char c = argv[1][i];
            if (c == '\\') {
                fp += '/';
            } else {
                fp += c;
            }
        }
        std::cout << "Open file:" << fp << std::endl;

        auto *mainWindow = new MainWindow();
        mainWindow->open_file(QString::fromLocal8Bit(fp.c_str()));
        mainWindow->show();
    } else {
        auto *w = new StartUI();
        w->show();
    }

    int exec = QApplication::exec();
    write_settings();

    return exec;
}
