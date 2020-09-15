#ifndef FACADE_H
#define FACADE_H

#include "mediator.h"
#include "dbcdata.h"
#include "dbcparser.h"
#include <QObject>

class Facade : public QObject
{
    Q_OBJECT

public:
    Facade();
    ~Facade();

    void convertDBC(DBCdata *data);

signals:
    void workFinishedSignal();
    void failedToFinishWorkSignal();

private:
    Mediator *DBC_mediator;
};

#endif // FACADE_H
