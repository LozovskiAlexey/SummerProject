#ifndef BASEMEDIATOR_H
#define BASEMEDIATOR_H

#include "dbcdata.h"
#include "ParsedDataStructure.h"

class BaseMediator
{
public:
    virtual void Notify(int command) const = 0;
    virtual void set(DBCdata *) {}
    virtual void set(DBCParsedData *){}
};

#endif // BASEMEDIATOR_H
