#include "nodewindow.h"
#include "ui_nodewindow.h"
#include <QDebug>

NodeWindow::NodeWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::NodeWindow)
{
    ui->setupUi(this);
    this->initWindow();
}

NodeWindow::~NodeWindow()
{
    delete ui;
}

void NodeWindow::initWindow()
{
    ui->lab_node->setText("节点 1001");
    ui->lab_desc->setText(qApp->applicationFilePath());
}

void NodeWindow::on_btn_start_clicked()
{
    qDebug("on_btn_start_clicked");
}

void NodeWindow::on_btn_install_driver_clicked()
{
    qDebug("on_btn_install_driver_clicked");
}
