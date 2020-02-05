#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QProcessEnvironment>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QInputDialog>
#include <QFileDialog>
#include <QDesktopServices>

#define GNB_NODE_ID Qt::UserRole + 1


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

int node_conf_num;

static void setup_gnb_node(int node_num){

    char cmd_buf[1024];
    sprintf(cmd_buf, "%s/gnb_quick_setup.cmd %d",  app_dirname, node_num);
    system(cmd_buf);

}

static void export_gnb_node(const char *export_path){

    QString path;    
    path.sprintf("%s/",  export_path);

    char cmd_buf[1024];
    sprintf(cmd_buf,"%s\\gnb_export_node.cmd %s %d", app_dirname, export_path, node_conf_num);
    system(cmd_buf);
    QDesktopServices::openUrl(QUrl::fromLocalFile(path));

    qDebug("export_gnb_node cmd_buf[%s]",cmd_buf);
}


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    node_conf_num = 0;
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

}


void MainWindow::onListItemDblClicked(QListWidgetItem* item)
{

    QString nodeid= item->data(GNB_NODE_ID).toString();  

}


void MainWindow::showList()
{

    int ret;

    gnb_heap_t *heap = gnb_heap_create(1024*1024);

    gnb_fixed_list_store_t *fixed_list_store = gnb_fixed_list_store_create(heap, 100, 1024);

    fixed_list_store->num = fixed_list_store->size;

    char conf_path[1024];

    snprintf(conf_path,1024,"%s\\conf",app_dirname);

    ret = gnb_get_sub_dirname(conf_path, 1024, (char **)&fixed_list_store->array, (int *)&fixed_list_store->num);

    int i;

    char *dirname;

    for (i=0; i<fixed_list_store->num; i++) {

        dirname = (char *)fixed_list_store->array[i];

        if( '.' == dirname[0] ){
            continue;
        }

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

    node_conf_num = (int)fixed_list_store->num;

    gnb_heap_release(heap);

}


void MainWindow::on_createButton_clicked()
{

    bool res;
    QString text = QInputDialog::getText(this, tr("创建节点数"), tr("请输入节点数"), QLineEdit::Normal, nullptr, &res);
    if (!res) return;

    if (text.isEmpty()) {
        QMessageBox::warning(nullptr, QStringLiteral("提示"), QStringLiteral("请输入节点数"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    QRegExp rx("^[0-9]*[1-9][0-9]*$");
    int pos = rx.indexIn(text);
    if(pos < 0){
        QMessageBox::warning(nullptr, QStringLiteral("提示"), QStringLiteral("请输入数字"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    if (text.toInt() > 6) {
        QMessageBox::warning(nullptr, QStringLiteral("提示"), QStringLiteral("受配置模板限制暂时最多可以创建6个节点，要创建"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }

    setup_gnb_node( text.toInt() );

    this->showList();

}


void MainWindow::on_cleanButton_clicked()
{

    if ( 0 == node_conf_num ){
        QMessageBox::warning(nullptr, "提示", "没有发现已创建节点配置", QMessageBox::Yes);
        return;
    }

    int result = QMessageBox::information(nullptr, "GNB", "后面的操作会清空此前创建的节点配置包括通信密钥，确认是否需要操作",
                              QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

    if(QMessageBox::No==result){
        return;
    }

    char cmd_buf[1024];
    sprintf(cmd_buf,"rd /S /Q %s\\conf %s %d", app_dirname);
    qDebug("clean [%s]", cmd_buf);

    system(cmd_buf);
    ui->listWidget->clear();
    node_conf_num = 0;
    this->showList();

}


void MainWindow::on_exportButton_clicked()
{

    if ( 0 == node_conf_num ){
        QMessageBox::warning(nullptr, "提示", "请确认节点配置已经创建", QMessageBox::Yes);
        return;
    }

    QFileDialog openFilePath(this);
    openFilePath.setAcceptMode(QFileDialog::AcceptOpen);
    openFilePath.setFileMode( QFileDialog::DirectoryOnly );

    QString filepath= openFilePath.getExistingDirectory();
    filepath.replace("/","\\");

    const char *export_path = filepath.toStdString().c_str();

    if (0==strlen(export_path)){
        return;
    }

    export_gnb_node(filepath.toStdString().c_str());
    qDebug("%s%s", "导出目录至：", filepath.toStdString().c_str());

}

