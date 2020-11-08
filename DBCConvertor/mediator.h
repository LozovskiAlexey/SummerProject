#ifndef MEDIATOR_H
#define MEDIATOR_H

#include "BaseMediator.h"
#include "dbcparser.h"
#include "transmitctransformer.h"
#include "transmithtransformer.h"
#include "recievehtransformer.h"
#include "recievectransformer.h"
#include "mainhtransformer.h"
#include "defineshtransformer.h"
#include "mainctransformer.h"
#include "dbcdata.h"


class Mediator : public BaseMediator
{
    Q_OBJECT

    int counter;
    DBCParser *parser;
    TransmitHTransformer *transmitHTransformer;
    TransmitCTransformer *transmitCTransformer;
    RecieveHTransformer *recieveHTransformer;
    MainHTransformer *mainHTransformer;
    MaincTransformer *maincTransformer;
    DefinesHTransformer *definesHTransformer;
    RecieveCTransformer *recieveCTransformer;

public:
    Mediator();

    void set(DBCdata *data) override;
    void set(DBCParsedData *data) override;
    void Notify(int command) override;
    void exec(BaseComponent *executer) const;
};


#endif // MEDIATOR_H
