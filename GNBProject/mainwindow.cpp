#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QProcessEnvironment>
#include <QHBoxLayout>
#include <QLabel>
#include <QProgressBar>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>

extern  char *app_dirname;


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    // 固定窗口大小
    this->setFixedSize(561, 407);
    this->showList();
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
      this, SLOT(onListItemClicked(QListWidgetItem*)));
    connect(ui->listWidget, SIGNAL(itemDoubleClicked(QListWidgetItem*)),
      this, SLOT(onListItemDblClicked(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showList()
{

    qDebug("showList");
    for (int i = 0; i<10; i++) {

        QWidget *widget = new QWidget(ui->listWidget);
        widget->setStyleSheet("background:transparent;");

        QWidget *frontArea = new QWidget(widget);
        frontArea->setFixedHeight(32);
        frontArea->setFixedWidth(130);

        QLabel *lab_node = new QLabel(frontArea);
        lab_node->setText("节点" + QString::number(1000+i));

        QLabel *lab_ip = new QLabel(frontArea);
        lab_ip->setText("虚拟ip： 192.168.1.1");

        QWidget *backArea = new QWidget(widget);
        backArea->setFixedSize(158, 32);

        QProgressBar *pg_data = new QProgressBar(backArea);
        pg_data->setTextVisible(false);
        pg_data->setFixedHeight(12);
        pg_data->setStyleSheet("QProgressBar{ border:none; background:rgb(230, 230, 230); border-radius:0px; text-align:center; color:gray }\
                                QProgressBar::chunk{ background:rgb(71, 137, 250); border-radius:0px; }");

        QLabel *lab_speed = new QLabel(backArea);
        lab_speed->setText("0 B/s");

        QLabel *lab_status = new QLabel(widget);
        lab_status->setText(QStringLiteral("等待中"));
        lab_status->setStyleSheet("QLabel{padding-left:0px;padding-right:0px;}");
        lab_status->setFixedWidth(55);

        QVBoxLayout *verLayout = new QVBoxLayout;
        verLayout->setContentsMargins(0, 0, 0, 0);
        verLayout->setMargin(0);
        verLayout->setSpacing(0);
        verLayout->addWidget(lab_node);
        verLayout->addWidget(lab_ip);
        frontArea->setLayout(verLayout);

        QVBoxLayout *verLayout2 = new QVBoxLayout;
        verLayout2->setContentsMargins(0, 0, 0, 0);
        verLayout2->setMargin(0);
        verLayout2->setSpacing(0);
        verLayout2->addWidget(pg_data);
        verLayout2->addWidget(lab_speed);
        backArea->setLayout(verLayout2);

        QHBoxLayout *layout = new QHBoxLayout(widget);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setMargin(0);
        layout->setSpacing(0);

        layout->addWidget(frontArea);
        layout->addWidget(backArea);
        layout->addWidget(lab_status);
        widget->setLayout(layout);

        QListWidgetItem *item = new QListWidgetItem();
        QSize size = item->sizeHint();
        item->setSizeHint(QSize(size.width(), 56));
        ui->listWidget->addItem(item);
        widget->setSizeIncrement(size.width(), 56);
        ui->listWidget->setItemWidget(item, widget);
    }
}

void MainWindow::onListItemClicked(QListWidgetItem* item)
{
    qDebug("onListItemClicked");
    if (ui->listWidget->item(0) == item) {
    }
}

void MainWindow::onListItemDblClicked(QListWidgetItem* item)
{
    qDebug("onListItemClicked");
}

void MainWindow::on_btn_create_clicked()
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

    if (text.toInt() > 3) {
        QMessageBox::warning(nullptr, QStringLiteral("提示"), QStringLiteral("最多生成3个节点"), QMessageBox::Yes, QMessageBox::Yes);
        return;
    }


    QProcess process(0);

    QStringList args;

    QString gnb_command;

    QString gnb_command_arg_string1;

    gnb_command_arg_string1.sprintf("%d",pos);

    gnb_command.sprintf("%s/gnb_setup.cmd",  app_dirname);

    args.append( gnb_command_arg_string1.toStdString().c_str() );

    qDebug("start execute %s %s", gnb_command.toStdString().c_str(),gnb_command_arg_string1.toStdString().c_str());

    process.execute(gnb_command,args);

    process.waitForFinished();

    qDebug("end execute %s %s", gnb_command.toStdString().c_str(),gnb_command_arg_string1.toStdString().c_str());


}

void MainWindow::on_btn_clean_clicked()
{
    ui->listWidget->clear();
    qDebug("TODO：清空列表");
}

void MainWindow::on_btn_export_clicked()
{
    QFileDialog dlg(this);
    //dlg.setDirectory("G:/D.T.Qt/Notes");//设置默认目录
    dlg.setAcceptMode(QFileDialog::AcceptOpen);
    // 限制只打开目录
    dlg.setFileMode(QFileDialog::Directory);

    if(dlg.exec() != QFileDialog::Accepted ||
            dlg.selectedFiles().length() > 1)
        return;

    for (auto fileName: dlg.selectedFiles()) {
        qDebug("%s%s", "导出目录至：", fileName.toStdString().c_str());
    }
}
