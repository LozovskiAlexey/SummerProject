#include "mainctransformer.h"


MaincTransformer::MaincTransformer()
{

}

MaincTransformer::MaincTransformer(BaseMediator *mediator)
{
    project_data_ = nullptr;
    data_ = nullptr;
    set_mediator(mediator);
}

void MaincTransformer::setData(DBCdata *data)
{
    project_data_ = data;
}

void MaincTransformer::setData(DBCParsedData *data)
{
    data_ = data;
}

void MaincTransformer::exec()
{
    if (project_data_ != nullptr && data_ != nullptr)
    {
        QString fileName = project_data_->get_project_path() + "/" + project_data_->get_project_name() + ".c";
        QFile file(fileName);

        if ((file.open(QIODevice::WriteOnly)))
        {
            QString prj_name = project_data_->get_project_name();

            QString text =  "#include \"Vector.h\"\n";
            file.write(text.toUtf8());

            // ----------------------- INIT - function ---------------------------------
            text = "void " + prj_name + "_init(canBASE_t *node," + prj_name + "_Struct *vector, TX_queue_struct *tx_queue)\n{\n";
            file.write(text.toUtf8());

            text = "    vector->tx_queue";
            int spaces_number = LENGTH_SECOND_COL - text.size();
            QString spaces(spaces_number, ' ');
            text += spaces + "= tx_queue;\n";
            file.write(text.toUtf8());

            for (int i=0; i< data_->msgboxes->size(); i++)
            {
                auto msgSignals = data_->msgboxes->at(i)->msgSignals;

                for (int j=0; j < msgSignals->size(); j++){
                    text = "    vector->" +  msgSignals->at(j)->name;
                    spaces_number = LENGTH_SECOND_COL - text.size();
                    spaces = QString(spaces_number, ' ');
                    text += spaces + "= " + msgSignals->at(j)->start_value + ";\n";
                    file.write(text.toUtf8());
                }
                text = "\n";
                file.write(text.toUtf8());
            }
            text = "}\n\n";
            file.write(text.toUtf8());

            // ----------------------- QUEUE - function ---------------------------------
            text = "void " + prj_name + "queue_call(" + prj_name + "_Struct *vector, uint32_t rti_count)\n{\n";
            file.write(text.toUtf8());

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                QString sender = data_->msgboxes->at(i)->sender;
                QString trans = data_->Transmitter;
                if (data_->msgboxes->at(i)->sender == data_->Transmitter){
                    text = "    void " + prj_name + "_queue_" + data_->msgboxes->at(i)->msgBoxName + "(" +
                            "vector, rti_count, " +
                            QString::number(data_->msgboxes->at(i)->cycle_time) + ");\n";
                    file.write(text.toUtf8());
                }
            }
            text = "}\n\n";
            file.write(text.toUtf8());

            // ----------------------- RECIEVE - function ---------------------------------
            text = "void " + prj_name + "_recieve_call(" + prj_name + "_Struct *vector, uint32_t messageBox, uint8_t *RX_can_data)\n{\n";
            file.write(text.toUtf8());

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                if (data_->msgboxes->at(i)->sender == data_->Reciever)
                {
                    text = "    if (messageBox == vector->msgBoxes.mb_" + data_->msgboxes->at(i)->msgBoxName + ")\n" +
                           "        " + prj_name + "_recieve_" + data_->msgboxes->at(i)->msgBoxName + "(" +
                           "vector, RX_can_data);\n";
                    file.write(text.toUtf8());
                }
            }
            text = "}\n\n";
            file.write(text.toUtf8());

            file.close();
        }
        else
            qDebug() << "Check Transform failed";

        mediator_->Notify(2);
    }
}
