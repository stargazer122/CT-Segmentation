#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVTKWidget.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void on_open_clicked();

    void on_refresh_clicked();

private:
    Ui::MainWindow *ui;
    static QString fileName;
    static vtkRenderWindow *renWin;
    void lily(const QString &name,const int &level);

};

#endif // MAINWINDOW_H
