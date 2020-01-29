#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QHBoxLayout>
#include <QLabel>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->showList();
    connect(ui->listWidget, SIGNAL(itemClicked(QListWidgetItem*)),
      this, SLOT(onListItemClicked(QListWidgetItem*)));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::showList()
{
    qDebug("showList");
    for (int i = 0; i<10; i++) {

        QListWidgetItem *item = new QListWidgetItem(ui->listWidget, 0);
        item->setSizeHint(QSize(100,100));

        QWidget *w = new QWidget(ui->listWidget);
        QHBoxLayout *layout=new QHBoxLayout(w);
        QPushButton *pushButton=new QPushButton(w);
        pushButton->setText("连接");

        QLabel *label = new QLabel;
        label->setText("生产机" + QString::number(1000+i));

        layout->addWidget(label);
        layout->addWidget(pushButton);
        w->setLayout(layout);
        w->show();
        ui->listWidget->setItemWidget(item, w);
    }
}

void MainWindow::onListItemClicked(QListWidgetItem* item)
{
    qDebug("onListItemClicked");
    if (ui->listWidget->item(0) == item) {
    }
}
