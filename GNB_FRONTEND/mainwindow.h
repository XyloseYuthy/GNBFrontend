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

    void on_startButton_clicked();
    void on_stopButton_clicked();
    void on_installdrvButton_clicked();
    void on_uninstalldrvButton_clicked();

    void onListItemClicked(QListWidgetItem* item);
    void onListItemDblClicked(QListWidgetItem* item);

};

#endif // MAINWINDOW_H
