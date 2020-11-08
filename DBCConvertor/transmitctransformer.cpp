#include "transmitctransformer.h"

TransmitCTransformer::TransmitCTransformer()
{

}

TransmitCTransformer::TransmitCTransformer(BaseMediator *mediator)
{
    project_data_ = nullptr;
    data_ = nullptr;
    set_mediator(mediator);
}

void TransmitCTransformer::setData(DBCdata *data)
{
    project_data_ = data;
}

void TransmitCTransformer::setData(DBCParsedData *data)
{
    data_ = data;
}

void TransmitCTransformer::exec()
{
    if (project_data_ != nullptr && data_ != nullptr)
    {
        QString fileName = project_data_->get_project_path() + "/" + project_data_->get_project_name() + "_Transmit" + ".c";
        QFile file(fileName);

        if ((file.open(QIODevice::WriteOnly)))
        {
            QString prj_name = project_data_->get_project_name();

            QString text =  "#include " + prj_name +"_Transmit.h\"\n\n";
            file.write(text.toUtf8());

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                if (data_->msgboxes->at(i)->sender == data_->Transmitter){
                    text = "void vector_queue_" + data_->msgboxes->at(i)->msgBoxName + "(" + prj_name + "_Struct" +
                            " *vector, uint32_t rti_count, uint32_t period" + ")\n{\n";
                    text += "    if((rti_count % period) == 0)\n    {\n";
                    file.write(text.toUtf8());

                    // код маскирования
                    auto msgbox = data_->msgboxes->at(i);
                    int msg_size = msgbox->size;

                    text = "    uint8_t TX_can_data[" + QString::number(msg_size) + "] = {";
                    for (int k=0; k < msg_size-1; k++){
                        text += "255, ";
                    }
                    text += "255 };\n\n";
                    file.write(text.toUtf8());


                    for (int j=0; j<msgbox->msgSignals->size(); j++)
                    {
                        auto sgnl = msgbox->msgSignals->at(j);
                        auto sgnl_size = sgnl->size;
                        auto sgnl_startbit = sgnl->startbit;


                        // 1 случай: размер и позиция кратны 8
                        if (sgnl_size % 8 == 0 and sgnl_startbit % 8 == 0)
                        {
                            int array_pos = sgnl_startbit / 8;
                            int array_elems_amount = sgnl_size / 8;
                            QString type = sgnl->type_values.name != "uint8_t" ? "(uint8_t)" : "";
                            QString bitmask = sgnl->type_values.name != "uint8_t" ? " & 0bFF" : " ";

                            text = "        TX_can_data[" + QString::number(array_pos++) + "] = ";
                            text += type + "(vector->" + sgnl->name + bitmask + ");\n";
                            file.write(text.toUtf8());

                            for (int k=1; k<array_elems_amount; k++)
                            {
                                text = "        TX_can_data[" + QString::number(array_pos++) + "] = ";
                                text += type + "((vector->" + sgnl->name + " >> " + QString::number(8 * k) + ")";
                                text += bitmask + ");\n";
                                file.write(text.toUtf8());
                            }
                        }
                    }

                    text = "        add_to_TX_queue(vector->tx_queue, vector->msgBoxes.mb_" +
                           data_->msgboxes->at(i)->msgBoxName + ", TX_can_data);\n";
                    file.write(text.toUtf8());

                    text = "    }\n}\n\n";
                    file.write(text.toUtf8());
                }
            }


            text = "\n" + prj_name +"MsgBoxes create_" + prj_name + "_tx_message_boxes(canBASE_t *node, " + prj_name + "MsgBoxes *msgBoxes)\n{\n";
            file.write(text.toUtf8());
            int spaces_number;

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                if (data_->msgboxes->at(i)->sender == data_->Transmitter){
                    text = "    msgBoxes->mb_" + data_->msgboxes->at(i)->msgBoxName;
                    spaces_number = LENGTH_SECOND_COL - text.size();
                    QString spaces(spaces_number, ' ');
                    text += spaces + "= MB_" + data_->msgboxes->at(i)->msgBoxName.toUpper() + "_MSG;\n";
                    file.write(text.toUtf8());
                }
            }

            text = "\n    can_mbox_t box;\n"                                                                          // Базовые настройки сообщений
                   "    box.can_reg = node;\n"
                   "    box.mask = 0x1FFFFFFF;\n"
                   "    box.id_size = CM_EXTEND_ID;\n"
                   "    box.direction = CM_DIR_RX;\n"
                   "    box.data_size = 8;\n"
                   "    box.can_int = 0;\n\n";
            file.write(text.toUtf8());


            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                if (data_->msgboxes->at(i)->sender == data_->Transmitter){
                    text  = "    box.message_box = msgBoxes->mb_" + data_->msgboxes->at(i)->msgBoxName + ";\n";
                    text += "    box.id = " + data_->msgboxes->at(i)->msgBoxName.toUpper() + "_MSG_ID;\n";
                    text += "    change_message_box(box);\n\n";
                    file.write(text.toUtf8());
                }
            }

            text = "    return *msgBoxes;\n}";
            file.write(text.toUtf8());

            file.close();
        }
        else
            qDebug() << "Check Transform failed";

        mediator_->Notify(2);
    }
}
