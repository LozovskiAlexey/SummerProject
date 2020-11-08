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

    msgbox->ID =  lst->at(1);
    msgbox->msgBoxName = msgBoxName[0];
    msgbox->size = lst->at(3).toInt();
    msgbox->sender = msgBoxSender[0];

    data->push_back(msgbox);
}

void DBCParser::readBA_(QStringList *lst, DBCParsedData *data)
{
    // пока что оно просто читает по BA_-метке cycle_time для каждого
    // msgbox
    lst->replaceInStrings("\"", "");
    lst->replaceInStrings(";", "");

    if (lst->at(1) == "GenMsgCycleTime" and lst->at(2) == "BO_"){
        auto id = lst->at(3);
        auto msgbox = data->get_by_id(id);
        msgbox->cycle_time = lst->last().toInt();
    }
}


void DBCParser::readSG_(QStringList *lst, DBCParsedData *data)
{
    sgnl_t *sgnl = new sgnl_t;
    QStringList sgnl_params = lst->at(4).split('|');
    QString size = sgnl_params.at(1).split('@').at(0);

    sgnl->name = lst->at(2);
    sgnl->size = size.toInt();
    sgnl->startbit = sgnl_params.at(0).toInt();
//    sgnl->issigned = sgnl_params.at(1).split('@').at(1) == "1+" ? false : true;
    sgnl->issigned = lst->at(5).contains('-') ? true : false;

    QStringList minMaxVal = lst->at(6).split('|');
    QString minVal = minMaxVal.at(0);
    QString maxVal = minMaxVal.at(1);
    sgnl->values.first = minVal.right(minVal.size()-1);
    sgnl->values.second = maxVal.left(maxVal.size()-1);

    QString sender = lst->last();
    sgnl->sender = sender.left(sender.size()-2);

    if (lst->at(5).contains('.'))
        sgnl->set_type("float");
    else
        sgnl->set_type();

    data->msgboxes->last()->msgSignals->append(sgnl);
}


void DBCParser::readVAL_(QStringList *lst, DBCParsedData *data)
{
    QString msBox_id = lst->at(1);
    QString sgnl_name = lst->at(2);
//    qDebug() << sgnl_name ;

    enum_t *tmp_enum = new enum_t;
    QMap<int, QString> type_values;

    auto msg_box = data->get_by_id(msBox_id);
    auto sgnl = msg_box->get_sgnl_by_name(sgnl_name);

    // трансформировать строку
    auto line = lst->join(' ');
    line.replace("'", "_");
    line.replace('.', '_');
    line.replace(',', '_');
    line.replace(';', "");

    bool ismember = false;
    for (int i=0; i<line.size(); i++){
        if (line[i] == '"'){
            ismember = !ismember;
        }
        if (line[i] == " " and ismember == true){
            line.replace(i, 1, "_");
        }
    }

    auto tmp_lst = line.split(" ");
    tmp_lst.replaceInStrings("\"", "");

    if (not (tmp_lst.contains("0") and tmp_lst.contains("1")) and
            (tmp_lst.contains(QString::number(pow(2, 16)-1)) or
             tmp_lst.contains(QString::number(pow(2, 8)-1)) or
             tmp_lst.contains("SNA") or
             tmp_lst.contains("error"))){
        sgnl->set_type();
    }
    else if (tmp_lst.size() == 8){
        sgnl->set_type("bool");
    }
    else{
        // заполняем enum
        for (int i=3; i<tmp_lst.size()-1; i+=2){
            type_values[tmp_lst.at(i).toInt()] = tmp_lst.at(i+1);
        }

        tmp_enum->set_name(sgnl_name);
        tmp_enum->values = type_values;

        // добавляем в массив enum'ов
        data->push_back(tmp_enum);

        // обновляем значение
        sgnl->set_type(*tmp_enum);

//        qDebug() << "_________________CHECK__________________";
//        qDebug() << sgnl->name;
//        qDebug() << sgnl->type_values.name;
//        qDebug() << "________________________________________";
    }

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
        qDebug() << "CICLE_TIME " << data->msgboxes->at(i)->cycle_time;

        qDebug() << "SG NUMBER " << data->msgboxes->at(i)->msgSignals->size();
        for (int j=0; j<data->msgboxes->at(i)->msgSignals->size(); j++)
        {
            qDebug() << "\t  =================== SIGNALS ================== ";
            qDebug() << "\t NAME " << data->msgboxes->at(i)->msgSignals->at(j)->name;
            qDebug() << "\t SIZE " << data->msgboxes->at(i)->msgSignals->at(j)->size;
            qDebug() << "\t SENDER " << data->msgboxes->at(i)->msgSignals->at(j)->sender;
            qDebug() << "\t FLAG " << data->msgboxes->at(i)->msgSignals->at(j)->issigned;
            qDebug() << "\t STARTBIT " << data->msgboxes->at(i)->msgSignals->at(j)->startbit;
            qDebug() << "\t TYPE " << data->msgboxes->at(i)->msgSignals->at(j)->type_values.name;
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

                else if (lst.size() > 2 && lst.at(0) == "" && lst.at(1) == "SG_"){
                    readSG_(&lst, &data);
                }


//                else if(lst.at(0) == "CM_")
//                    break;

                else if (lst.at(0) == "BA_"){
                    readBA_(&lst, &data);
                }

                else if(lst.at(0) == "VAL_"){
                    readVAL_(&lst, &data);
                }


                str = file.readLine();
                lst = str.split(' ');
            }

            file.close();

//            showData(&data);
            mediator_->set(&data);
            mediator_->Notify(1);
        }
    }
}
