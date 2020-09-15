#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "BaseMediator.h"
#include "dbcparser.h"
#include "transmithtransformer.h"
#include "recievehtransformer.h"
#include "mainhtransformer.h"
#include "dbcdata.h"


class Mediator : public BaseMediator
{
    DBCParser *parser;
    TransmitHTransformer *transmitHTransformer;
    RecieveHTransformer *recieveHTransformer;
    MainHTransformer *mainHTransformer;

public:
    Mediator();

    void set(DBCdata *data) override;
    void set(DBCParsedData *data) override;
    void Notify(int command) const override;
    void exec(BaseComponent *executer) const;
};


#endif // MEDIATOR_H
