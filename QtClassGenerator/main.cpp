#include "main_window.h"
#include <QApplication>

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);
    a.setApplicationVersion("1.0.0");
    a.setApplicationName(QStringLiteral("Qt Class Generator"));
    MainWindow w;
    w.setWindowTitle(a.applicationName());
    w.show();

    return a.exec();
}
