#include "dbcparser.h"

DBCParser::DBCParser()
{

}

DBCParser::DBCParser(BaseMediator *mediator)
{
    set_mediator(mediator);
}

void DBCParser::setData(DBCdata *data)
{
    data_ = data;
}

void DBCParser::readBU_(QStringList *lst, DBCParsedData *data)
{
    data->Reciever = lst->at(1);
    data->Transmitter = lst->at(2).left(lst->at(2).size()-2);
}

void DBCParser::readBO_(QStringList *lst, DBCParsedData *data)
{
    msgbox_t *msgbox = new msgbox_t;
    msgbox->msgSignals = new QVector<sgnl_t *>;

    QStringList msgBoxName = lst->at(2).split(':');
    QStringList msgBoxSender = lst->at(4).split('\r');

    msgbox->ID = lst->at(1);
    msgbox->msgBoxName = msgBoxName[0];
    msgbox->size = lst->at(3).toInt();
    msgbox->sender = msgBoxSender[0];

    data->push_back(msgbox);

}

void DBCParser::readSG_(QStringList *lst, DBCParsedData *data)
{
    sgnl_t *sgnl = new sgnl_t;
    QStringList sgnl_params = lst->at(4).split('|');
    QString size = sgnl_params.at(1).split('@').at(0);

    sgnl->name = lst->at(2);
    sgnl->size = size.toInt();
    sgnl->startbit = sgnl_params.at(0).toInt();
    sgnl->issigned = sgnl_params.at(1).split('@').at(1) == "1+" ? true : false;

    QStringList minMaxVal = lst->at(6).split('|');
    QString minVal = minMaxVal.at(0);
    QString maxVal = minMaxVal.at(1);
    sgnl->minValue = minVal.right(minVal.size()-1);
    sgnl->maxValue = maxVal.left(maxVal.size()-1);

    QString sender = lst->last();
    sgnl->sender = sender.left(sender.size()-2);

    data->msgboxes->last()->msgSignals->append(sgnl);
}

void DBCParser::showData(DBCParsedData *data)
{
    for (int i=0; i<data->msgboxes->size(); i++)
    {
        qDebug() << " =================== INDEX ================== " << i;
        qDebug() << "NAME " << data->msgboxes->at(i)->msgBoxName;
        qDebug() << "ID " << data->msgboxes->at(i)->ID;
        qDebug() << "SIZE " <<  data->msgboxes->at(i)->size;
        qDebug() << "SENDER " << data->msgboxes->at(i)->sender;

        qDebug() << "SG NUMBER " << data->msgboxes->at(i)->msgSignals->size();
        for (int j=0; j<data->msgboxes->at(i)->msgSignals->size(); j++)
        {
            qDebug() << "\t  =================== SIGNALS ================== ";
            qDebug() << "\t NAME " << data->msgboxes->at(i)->msgSignals->at(j)->name;
            qDebug() << "\t SIZE " << data->msgboxes->at(i)->msgSignals->at(j)->size;
            qDebug() << "\t SENDER " << data->msgboxes->at(i)->msgSignals->at(j)->sender;
            qDebug() << "\t FLAG " << data->msgboxes->at(i)->msgSignals->at(j)->issigned;
            qDebug() << "\t STARTBIT " << data->msgboxes->at(i)->msgSignals->at(j)->startbit;
        }
    }
}


void DBCParser::exec()
{
    if (data_ != nullptr)
    {
        QFile file(data_->get_DBC_flie_name());

        if ((file.exists())&&(file.open(QIODevice::ReadOnly)))
        {
            DBCParsedData data;
            QString str = "";
            QStringList lst = str.split(' ');

            data.init();

            while(!file.atEnd())
            {
                if (lst.at(0) == "BU_:"){
                    readBU_(&lst, &data);
                }

                else if (lst.at(0) == "BO_"){
                    readBO_(&lst, &data);
                }


                else if (lst.size() > 2 && lst.at(1) == "SG_"){
                    readSG_(&lst, &data);
                }


                else if(lst.at(0) == "CM_")
                    break;


                str = file.readLine();
                lst = str.split(' ');
            }


//            showData(&data);
            file.close();

            mediator_->set(&data);
            mediator_->Notify(1);
        }
    }
}
