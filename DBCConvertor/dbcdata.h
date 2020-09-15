#ifndef DBCDATA_H
#define DBCDATA_H

#include "Data.h"
#include <QString>
#include "projectdatastructure.h"

class DBCdata : public AbstractData
{
public:
    DBCdata();

    void set_project_name(QString);
    void set_project_path(QString);
    void set_DBC_flie_name(QString);
    void set_MSGBox_start_value(int);
    void set_project_structure(project_structure_t);

    QString &get_project_name();
    QString &get_project_path();
    QString &get_DBC_flie_name();
    int &get_MSGBox_start_value();
    project_structure_t &get_project_structure();

private:
    QString project_name;              // Название проекта
    QString project_path;              // Путь, где будет сохранен результат
    QString DBC_file_name;             // DBC файл
    int MSGBox_start_value;            // стартовый индекс messagebox
    project_structure_t structure;     // Файлы, которые нужно сгенерировать
};

#endif // DBCDATA_H
