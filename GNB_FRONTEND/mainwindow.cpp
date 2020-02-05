#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QProcessEnvironment>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include "gnb_alloc.h"
#include "gnb_fixed_list_store.h"
#include "gnb_dir.h"

#ifdef __cplusplus
}
#endif


extern char *app_dirname;

const char *cur_node_id;

int isStart=0;

#define GNB_NODE_ID Qt::UserRole + 1

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->showList();

    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)),      this, SLOT(onListItemClicked(QListWidgetItem*)));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),this, SLOT(onListItemDblClicked(QListWidgetItem*)));

}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::onListItemClicked(QListWidgetItem* item)
{

    QString nodeid= item->data(GNB_NODE_ID).toString();
    cur_node_id = strdup(nodeid.toStdString().c_str());
    qDebug("onListItemClicked[%s]", cur_node_id );

}


void MainWindow::onListItemDblClicked(QListWidgetItem* item)
{

    QString nodeid= item->data(GNB_NODE_ID).toString();

    if ( nullptr != cur_node_id){
        free((void *)cur_node_id);
    }

    cur_node_id = strdup(nodeid.toStdString().c_str());
    qDebug("onListItemDblClicked[%s]", cur_node_id );

}


void MainWindow::showList()
{

    int ret;

    cur_node_id = nullptr;

    gnb_heap_t *heap = gnb_heap_create(1024*1024);

    gnb_fixed_list_store_t *fixed_list_store = gnb_fixed_list_store_create(heap, 100, 1024);

    fixed_list_store->num = fixed_list_store->size;

    char conf_path[1024];

    snprintf(conf_path,1024,"%s\\conf",app_dirname);

    ret = gnb_get_sub_dirname(conf_path, 1024, (char **)&fixed_list_store->array, (int *)&fixed_list_store->num);

    int i;

    char *dirname;

    int idx = 0;

    for (i=0; i<fixed_list_store->num; i++) {

        dirname = (char *)fixed_list_store->array[i];

        if( '.' == dirname[0] ){
            continue;
        }

        if ( 0==idx ){
            cur_node_id = strdup(dirname);
        }

        idx++;

        QWidget *widget = new QWidget(ui->listWidget);
        widget->setStyleSheet("background:transparent;");

        QLabel *lab_node = new QLabel(widget);
        lab_node->setText("节点 : " + QString(QLatin1String(dirname)));

        QLabel *lab_vip = new QLabel(widget);
        lab_vip->setText(" ");


        QListWidgetItem *item = new QListWidgetItem();

        item->setData(GNB_NODE_ID, QString(QLatin1String(dirname)));

        QSize size = item->sizeHint();

        item->setSizeHint(QSize(size.width(), 32));

        ui->listWidget->addItem(item);
        widget->setSizeIncrement(size.width(), 32);

        ui->listWidget->setItemWidget(item, widget);


        QHBoxLayout *hLayout = new QHBoxLayout;
        hLayout->setContentsMargins(0, 0, 0, 0);
        hLayout->setMargin(0);
        hLayout->setSpacing(0);

        hLayout->addWidget(lab_node);
        hLayout->addWidget(lab_vip);

        widget->setLayout(hLayout);

    }

    qDebug("cur_node_id[%s]",cur_node_id);

    gnb_heap_release(heap);

}


void op_gnb_node(const char *gnb_path, const char *node_id, int op){

    char cmd_buf[1024];

    if (0==op){
        sprintf(cmd_buf,"start %s\\bin\\Window10_x86_64\\gnb.exe -i WindowsTun -c %s\\conf\\%s", gnb_path, gnb_path, node_id);
        system(cmd_buf);
        sprintf(cmd_buf,"start %s\\bin\\Window10_x86_64\\gnb_es.exe -s -b %s\\conf\\%s\\gnb.map --dump-address --upnp", gnb_path, gnb_path, node_id);
        system(cmd_buf);
    }else{
        sprintf(cmd_buf,"%s\\sh\\gnb_windows_stop.cmd %s",  gnb_path);
        system(cmd_buf);
    }

}


void op_if_drv(const char *gnb_path, int op){

    char cmd_buf[1024];

    if(1==op){
        sprintf(cmd_buf,"%s\\if_drv\\tap-windows\\addtap.bat",     gnb_path);
    }else{
        sprintf(cmd_buf,"%s\\if_drv\\tap-windows\\deltapall.bat",  gnb_path);
    }

    system(cmd_buf);

}


void MainWindow::on_startButton_clicked()
{

    if ( 0 != isStart ){
        QMessageBox::warning(nullptr, "提示", "GNB服务可能已经启动，请确认", QMessageBox::Yes);
        return;
    }
    if ( nullptr == cur_node_id ){
        QMessageBox::warning(nullptr, "提示", "请选择一个GNB节点", QMessageBox::Yes);
        return;
    }

    isStart = 1;

    op_gnb_node(app_dirname, cur_node_id, 0);

    qDebug("on_startButton_clicked");

}


void MainWindow::on_stopButton_clicked()
{

    if ( nullptr == cur_node_id ){
        QMessageBox::warning(nullptr, "提示", "请选择一个节点", QMessageBox::Yes);
        return;
    }

    isStart = 0;

    op_gnb_node(app_dirname, cur_node_id, 1);

    qDebug("on_stopButton_clicked");

}


void MainWindow::on_installdrvButton_clicked()
{
    int result = QMessageBox::information(nullptr, "GNB", "GNB是一个绿色软件，但在Windows上需要依赖一个第三方的虚拟网卡驱动（OpenVPN开发），不打算安装可以选择取消",
                             QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if(QMessageBox::No==result){
        return;
    }

    op_if_drv(app_dirname,1);

    qDebug("on_installdrvButton_clicked");

}


void MainWindow::on_uninstalldrvButton_clicked()
{
    op_if_drv(app_dirname,2);
    qDebug("on_uninstalldrvButton_clicked");
}

