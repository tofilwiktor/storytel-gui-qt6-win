#include "mainwindow.h"

#include <QApplication>
#include <QFile>
#include <cstdlib>

int main(int argc, char *argv[])
{
    putenv("QT_MEDIA_BACKEND=windows");
    putenv("QT_MULTIMEDIA_PREFERRED_PLUGINS=directshow");
    QApplication a(argc, argv);
    MainWindow w;
    QFile file(":qdarkstyle/dark/darkstyle.qss");
    file.open(QFile::ReadOnly);
    QString stylesheet = QLatin1String(file.readAll());
    a.setStyleSheet(stylesheet);
    w.showList();
    w.mediaPlayerInit();
    w.initKeyboardShortcuts();
    w.show();
    return a.exec();
}
