#include "mainwindow.h"
#include <QApplication>
#include "nodewindow.h"



#include <string.h>
#include <stdlib.h>

char *gnb_real_dirname_dup(char *path){

    char *string = strdup(path);

    char *resolved_path = (char *)malloc(1024);

    size_t string_len = strlen( (const char *)string );

    int i;

    for ( i=string_len-1; i<=0; i-- ) {

        if ( '/'==string[i] || '\\'==string[i] ){
            string[i]='\0';
            break;
        }

    }

#ifdef _WIN32
    if ( NULL == _fullpath(resolved_path, string, 1024) ){
        free(string);
        free(resolved_path);
        return NULL;
    }

    free(string);
    return resolved_path;
#else

    if ( NULL == realpath(string,resolved_path) ){
        free(string);
        free(resolved_path);
        return NULL;
    }

    free(string);
    return resolved_path;

#endif

}


char *app_dirname;

int main(int argc, char *argv[])
{

    app_dirname = gnb_real_dirname_dup(argv[0]);

    QApplication a(argc, argv);

    // 切换指定窗口
#if 1
    // 主窗口
    MainWindow w;
#else
    // 节点窗口
    NodeWindow w;
#endif
    w.show();

    return a.exec();
}
