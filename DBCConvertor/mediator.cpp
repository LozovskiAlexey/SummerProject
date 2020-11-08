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

    counter = 0;
}


void Mediator::exec(BaseComponent *executer) const
{
    executer->exec();
}


void Mediator::set(DBCdata *data)
{
    parser->setData(data);
    if (data->get_project_structure().main_c){
        maincTransformer->setData(data);
        counter += 1;
    }
    if (data->get_project_structure().main_h){
        mainHTransformer->setData(data);
        counter += 1;
    }
    if (data->get_project_structure().defines_h){
        definesHTransformer->setData(data);
        counter += 1;
    }
    if (data->get_project_structure().recieve_c){
        recieveCTransformer->setData(data);
        counter += 1;
    }
    if (data->get_project_structure().recieve_h){
        recieveHTransformer->setData(data);
        counter += 1;
    }
    if (data->get_project_structure().transmit_c){
        transmitCTransformer->setData(data);
        counter += 1;
    }
    if (data->get_project_structure().transmit_h){
        transmitHTransformer->setData(data);
        counter += 1;
    }
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

void Mediator::Notify(int command)
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
        counter--;
        if (counter == 0)
            Notify(3);
        break;
    case 3:
        emit work_finished();
        break;
    }
}
