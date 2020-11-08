#include "mediator.h"

Mediator::Mediator()
{
    parser = new DBCParser(this);
    transmitHTransformer = new TransmitHTransformer(this);
    transmitCTransformer = new TransmitCTransformer(this);

    recieveHTransformer = new RecieveHTransformer(this);
    recieveCTransformer = new RecieveCTransformer(this);

    mainHTransformer = new MainHTransformer(this);

    definesHTransformer = new DefinesHTransformer(this);

    maincTransformer = new MaincTransformer(this);
    // добавить класс-генератор
}


void Mediator::exec(BaseComponent *executer) const
{
    executer->exec();
}


void Mediator::set(DBCdata *data)
{
    parser->setData(data);
    if (data->get_project_structure().main_c)
        maincTransformer->setData(data);
    if (data->get_project_structure().main_h)
        mainHTransformer->setData(data);
    if (data->get_project_structure().defines_h)
        definesHTransformer->setData(data);
    if (data->get_project_structure().recieve_c)
        recieveCTransformer->setData(data);
    if (data->get_project_structure().recieve_h)
        recieveHTransformer->setData(data);
    if (data->get_project_structure().transmit_c)
        transmitCTransformer->setData(data);
    if (data->get_project_structure().transmit_h)
        transmitHTransformer->setData(data);
}

void Mediator::set(DBCParsedData *data)
{
    transmitHTransformer->setData(data);
    transmitCTransformer->setData(data);
    recieveHTransformer->setData(data);
    recieveCTransformer->setData(data);
    mainHTransformer->setData(data);
    definesHTransformer->setData(data);
    maincTransformer->setData(data);
}

void Mediator::Notify(int command) const
{
    switch (command){
    case 0:
        exec(parser);
        break;
    case 1:
        exec(transmitHTransformer);
        exec(transmitCTransformer);
        exec(recieveHTransformer);
        exec(recieveCTransformer);
        exec(mainHTransformer);
        exec(definesHTransformer);
        exec(maincTransformer);
        break;
    case 2:
        // завершение работы, отправка сигнала в facade о конце работы
        break;
    }
}
