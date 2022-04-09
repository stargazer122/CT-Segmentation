#include "imagechecking.h"
#include <QDir>
#include <QString>
#include <QFileInfo>
#include <QMessageBox>
#include <QDebug>
#include <iostream>

ImageChecking::ImageChecking(const QString &name)
{
     path=name;
     count=Get_list();
     printf("%d files are readed!\n",count);
}
int ImageChecking::Checking_empty_type()
{
    if(count==0)
    {
        QMessageBox *meg=new QMessageBox;
        meg->setText("File are not allowed to blank");
        meg->exec();
        return 0;
    }
    /*-----逐个测试文件格式---------------------*/
    for(int i=0;i<count;i++)
    {
        QFileInfo info(full_name_list[i]);
        QString suffix= info.suffix();
        if(suffix!="dcm")
        {
            QMessageBox *meg=new QMessageBox;
            meg->setText("请检查文件格式（.dcm）！");
            meg->exec();
            return 0;
        }
    }
    qDebug()<<"Reading the file..."; //注意是qDebug不是QDebug!
    return 1;
}
int ImageChecking::Checking_size()
{
    qint64 size=0;
    for(int i=0;i<count;i++)
    {
        QFileInfo info(full_name_list[i]);
        size += info.size();
    }
    qint64 KMG=size/1024/1024;
    if(KMG>145)
    {
        QMessageBox *meg=new QMessageBox;
        meg->setText("文件过大，请调整（140M）!");
        meg->exec();
        return 0;
    }
    return 1;

}
int ImageChecking::Get_list()
{
    QDir dir(path);
    dir.setFilter(QDir::Files | QDir::NoSymLinks); //十分重要，否则不能成功读出文件名；
    QStringList list=dir.entryList(); //list获取的为文件名字
    int t=list.count();
    for(int i=0;i<t;i++)
    {
         QString fullName=path+"/"+list[i];
         full_name_list.push_back(fullName);
    }
    return t;
}
