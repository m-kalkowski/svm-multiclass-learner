#include "mainwindow.h"

#include <QApplication>
#include <QTextStream>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m;
    m.show();

    QFile f("layout/qdarkstyle/style.qss");
    if (!f.exists())
    {
        printf("Unable to set stylesheet, file not found\n");
    }
    else
    {
        f.open(QFile::ReadOnly | QFile::Text);
        QTextStream ts(&f);
        a.setStyleSheet(ts.readAll());
    }

    return a.exec();
}
