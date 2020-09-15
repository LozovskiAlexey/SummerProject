#include "dbcdata.h"

DBCdata::DBCdata()
{
}

void DBCdata::set_project_name(QString prj_name)
{
    this->project_name = prj_name;
}

void DBCdata::set_project_path(QString prj_path)
{
    this->project_path = prj_path;
}

void DBCdata::set_DBC_flie_name(QString file_name)
{
    this->DBC_file_name = file_name;
}

void DBCdata::set_MSGBox_start_value(int start_value)
{
    this->MSGBox_start_value = start_value;
}

void DBCdata::set_project_structure(project_structure_t structure)
{
    this->structure = structure;
}

QString &DBCdata::get_project_name()
{
    return project_name;
}

QString &DBCdata::get_project_path()
{
    return project_path;
}

QString &DBCdata::get_DBC_flie_name()
{
    return DBC_file_name;
}

int &DBCdata::get_MSGBox_start_value()
{
    return MSGBox_start_value;
}

project_structure_t &DBCdata::get_project_structure()
{
    return structure;
}
