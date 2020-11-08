#ifndef MAINCTRANSFORMER_H
#define MAINCTRANSFORMER_H

#include <QFile>
#include <QDebug>
#include "BaseComponent.h"

#define LENGTH_SECOND_COL 55
#define LENGTH_THIRD_COL 90

class MaincTransformer : public BaseComponent
{
private:
    DBCdata *project_data_;
    DBCParsedData *data_;

public:
    MaincTransformer();
    MaincTransformer(BaseMediator *mediator);

    void setData(DBCdata *data) override;
    void setData(DBCParsedData  *data) override;

    void exec() override;
};
#endif // MAINCTRANSFORMER_H
