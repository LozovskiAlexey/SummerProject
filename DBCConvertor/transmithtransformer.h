#ifndef TRANSMITHTRANSFORMER_H
#define TRANSMITHTRANSFORMER_H

#include <QFile>
#include <QDebug>
#include "BaseComponent.h"

class TransmitHTransformer : public BaseComponent
{
private:
    DBCdata *project_data_;
    DBCParsedData *data_;

public:
    TransmitHTransformer();
    TransmitHTransformer(BaseMediator *mediator);

    void setData(DBCdata *data) override;
    void setData(DBCParsedData  *data) override;

    void exec() override;
};

#endif // TRANSMITHTRANSFORMER_H
