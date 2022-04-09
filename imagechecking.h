#ifndef IMAGECHECKING_H
#define IMAGECHECKING_H

#include <QString>
#include <QStringList>

class ImageChecking
{
public:
    ImageChecking(const QString &name);
    int Checking_empty_type();
    int Checking_size();
private:
    int count;
    QString path;
    QStringList full_name_list;
    int Get_list();

};

#endif // IMAGECHECKING_H
