#include "mainwindow.h"
#include <QApplication>


#ifdef __cplusplus
extern "C" {
#endif

#include "gnb_dir.h"

#ifdef __cplusplus
}
#endif


char *app_dirname;

int main(int argc, char *argv[])
{
    app_dirname = gnb_real_dirname_dup(argv[0]);

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
