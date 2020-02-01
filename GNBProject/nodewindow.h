#ifndef NODEWINDOW_H
#define NODEWINDOW_H

#include <QMainWindow>

namespace Ui {
class NodeWindow;
}

class NodeWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit NodeWindow(QWidget *parent = nullptr);
    ~NodeWindow();

private:
    void initWindow();

private slots:
    void on_btn_start_clicked();
    void on_btn_install_driver_clicked();

    void on_btn_uninstall_driver_clicked();

    void on_btn_stop_clicked();

private:
    Ui::NodeWindow *ui;
};

#endif // NODEWINDOW_H
