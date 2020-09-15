#include "mediator.h"

Mediator::Mediator()
{
    parser = new DBCParser(this);
    transmitHTransformer = new TransmitHTransformer(this);
    recieveHTransformer = new RecieveHTransformer(this);
    mainHTransformer = new MainHTransformer(this);
    // добавить класс-генератор
}


void Mediator::exec(BaseComponent *executer) const
{
    executer->exec();
}


void Mediator::set(DBCdata *data)
{
    parser->setData(data);
    transmitHTransformer->setData(data);
    recieveHTransformer->setData(data);
    mainHTransformer->setData(data);
}

void Mediator::set(DBCParsedData *data)
{
    transmitHTransformer->setData(data);
    recieveHTransformer->setData(data);
    mainHTransformer->setData(data);
}

void Mediator::Notify(int command) const
{
    switch (command){
    case 0:
        exec(parser);
        break;
    case 1:
        exec(transmitHTransformer);
        exec(recieveHTransformer);
        exec(mainHTransformer);
        // создать фабрики и параллельно запустить exec()
        break;
    case 2:
        // завершение работы, отправка сигнала в facade о конце работы
        break;
    }
}
