#include "mainwindow.h"
#include <QApplication>
#include "nodewindow.h"



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 切换指定窗口
#if 0
    // 主窗口
    MainWindow w;
#else
    // 节点窗口
    NodeWindow w;
#endif
    w.show();

    return a.exec();
}
