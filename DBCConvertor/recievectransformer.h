#ifndef RECIEVECTRANSFORMER_H
#define RECIEVECTRANSFORMER_H

#include <QFile>
#include <QDebug>
#include "BaseComponent.h"

#define LENGTH_SECOND_COL 55
#define LENGTH_THIRD_COL 90

class  RecieveCTransformer : public BaseComponent
{
private:
    DBCdata *project_data_;
    DBCParsedData *data_;

public:
    RecieveCTransformer();
    RecieveCTransformer(BaseMediator *mediator);

    void setData(DBCdata *data) override;
    void setData(DBCParsedData  *data) override;

    void exec() override;
};



#endif // RECIEVECTRANSFORMER_H
