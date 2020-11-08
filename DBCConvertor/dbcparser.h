#ifndef DBCPARSER_H
#define DBCPARSER_H

#include <QFile>
#include "BaseComponent.h"
#include "dbcdata.h"
#include "ParsedDataStructure.h"
#include <QDebug>

class DBCParser : public BaseComponent
{
private: 
    DBCdata *data_;
public:
    DBCParser();
    DBCParser(BaseMediator *mediator);

    void readBU_(QStringList *lst, DBCParsedData *data);
    void readBO_(QStringList *lst, DBCParsedData *data);
    void readBA_(QStringList *lst, DBCParsedData *data);
    void readSG_(QStringList *lst, DBCParsedData *data);
    void readVAL_(QStringList *lst, DBCParsedData *data);
    void showData(DBCParsedData *data);
    void setData(DBCdata *data) override;
    void exec() override;
};

#endif // DBCPARSER_H
