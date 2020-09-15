#ifndef MAINHTRANSFORMER_H
#define MAINHTRANSFORMER_H

#include <QFile>
#include <QDebug>
#include "BaseComponent.h"

class MainHTransformer : public BaseComponent
{
private:
    DBCdata *project_data_;
    DBCParsedData *data_;


public:
    MainHTransformer();
    MainHTransformer(BaseMediator *mediator);

    void setData(DBCdata *data) override;
    void setData(DBCParsedData  *data) override;

    void exec() override;
};

#endif // MAINHTRANSFORMER_H
