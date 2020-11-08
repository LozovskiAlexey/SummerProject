#ifndef BASEMEDIATOR_H
#define BASEMEDIATOR_H

#include "dbcdata.h"
#include "ParsedDataStructure.h"
#include <QObject>

class BaseMediator : public QObject
{
    Q_OBJECT

signals:
    void work_finished();

public:
    virtual void Notify(int command) = 0;
    virtual void set(DBCdata *) {}
    virtual void set(DBCParsedData *){}
};

#endif // BASEMEDIATOR_H
