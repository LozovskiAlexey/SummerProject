#ifndef RECIEVEHTRANSFORMER_H
#define RECIEVEHTRANSFORMER_H

#include <QFile>
#include <QDebug>
#include "BaseComponent.h"

class RecieveHTransformer : public BaseComponent
{
private:
    DBCdata *project_data_;
    DBCParsedData *data_;

public:
    RecieveHTransformer();
    RecieveHTransformer(BaseMediator *mediator);

    void setData(DBCdata *data) override;
    void setData(DBCParsedData  *data) override;

    void exec() override;
};

#endif // RECIEVEHTRANSFORMER_H
