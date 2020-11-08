#ifndef DEFINESHTRANSFORMER_H
#define DEFINESHTRANSFORMER_H

#include <QFile>
#include <QDebug>
#include "BaseComponent.h"


#define LENGTH_SECOND_COL 55
#define LENGTH_THIRD_COL 90

class DefinesHTransformer : public BaseComponent
{
private:
    DBCdata *project_data_;
    DBCParsedData *data_;

public:
    DefinesHTransformer();
    DefinesHTransformer(BaseMediator *mediator);

    void setData(DBCdata *data) override;
    void setData(DBCParsedData  *data) override;

    void exec() override;
};

#endif // DEFINESHTRANSFORMER_H
