#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QListWidget *listWidget;

private:
    void showList();

private slots:
    void onListItemClicked(QListWidgetItem* item);
    void on_btn_create_clicked();
    void on_btn_clean_clicked();
    void on_btn_export_clicked();
};

#endif // MAINWINDOW_H
