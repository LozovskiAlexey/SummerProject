#include "facade.h"

Facade::Facade()
{
    DBC_mediator = new Mediator;
}

Facade::~Facade()
{
    delete DBC_mediator;
}

void Facade::convertDBC(DBCdata *data)
{
    DBC_mediator->set(data);
    DBC_mediator->Notify(0);

    if (data->get_MSGBox_start_value() == 0)
        emit workFinishedSignal();
    else
        emit failedToFinishWorkSignal();
}
