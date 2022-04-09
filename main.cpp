#include "mainwindow.h"
#include <QApplication>
#include <QTextCodec>
#include <vtkAutoInit.h>

VTK_MODULE_INIT(vtkRenderingOpenGL2);
VTK_MODULE_INIT(vtkInteractionStyle);

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*---下面三行用于防止中文乱码---------------------------*/

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF8"));

    MainWindow w;
    w.show();

    return a.exec();
}
