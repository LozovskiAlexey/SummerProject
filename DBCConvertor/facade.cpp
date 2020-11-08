#include "facade.h"

Facade::Facade()
{
    DBC_mediator = new Mediator;
    connect(DBC_mediator, SIGNAL(work_finished(void)), this, SLOT(on_mediator_work_slot(void)));
}

Facade::~Facade()
{
    delete DBC_mediator;
}

void Facade::convertDBC(DBCdata *data)
{
    DBC_mediator->set(data);
    DBC_mediator->Notify(0);
}


void Facade::on_mediator_work_slot(){
     emit workFinishedSignal();
}
