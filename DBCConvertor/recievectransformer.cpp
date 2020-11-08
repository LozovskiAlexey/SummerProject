#include "recievectransformer.h"

RecieveCTransformer::RecieveCTransformer()
{

}

RecieveCTransformer::RecieveCTransformer(BaseMediator *mediator)
{
    project_data_ = nullptr;
    data_ = nullptr;
    set_mediator(mediator);
}

void RecieveCTransformer::setData(DBCdata *data)
{
    project_data_ = data;
}

void RecieveCTransformer::setData(DBCParsedData *data)
{
    data_ = data;
}

void RecieveCTransformer::exec()
{
    if (project_data_ != nullptr && data_ != nullptr)
    {
        QString fileName = project_data_->get_project_path() + "/" + project_data_->get_project_name() + "_Recieve" + ".c";
        QFile file(fileName);

        if ((file.open(QIODevice::WriteOnly)))
        {
            QString prj_name = project_data_->get_project_name();

            QString text =  "#include " + prj_name +"_Recieve.h\"\n\n";
            file.write(text.toUtf8());

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                if (data_->msgboxes->at(i)->sender == data_->Reciever){

                    text = "void " + prj_name + "_recieve_" + data_->msgboxes->at(i)->msgBoxName + "(" +
                            prj_name + "_Struct" + " *vector, uint8_t *RX_can_data)\n{\n";
                    file.write(text.toUtf8());


                    // код маскирования -----------------------------------

                    auto msgbox = data_->msgboxes->at(i);
                    for (int j=0; j<msgbox->msgSignals->size(); j++)
                    {
                        auto sgnl = msgbox->msgSignals->at(j);
                        text = "    " + sgnl->name + " = ";

                        // формирование текста маски
                        auto sgnl_startbit = sgnl->startbit;
                        int sgnl_size = sgnl->size;
                        QString mask;

                        // 1 случай: размер, кратный 8, позиция, кратная 8
                        if (sgnl_size % 8 == 0 and sgnl_startbit % 8 == 0){
                            int elem_amount = sgnl_size / 8;     // количество элементов
                            int elem_number = sgnl_startbit / 8 + elem_amount -1; // номер элемента в массиве

                            QString symbol(elem_amount, '(');
                            QString type = sgnl->type_values.name == "uint8_t" ? "" : "(" + sgnl->type_values.name +")";
                            mask = type + symbol;

                            for (int k=0; k<elem_amount-1; k++){

                                mask += "RX_can_data[" + QString::number(elem_number) + "]" +
                                        "<<" + QString::number(8 * (elem_number)) + ") | ";
                                elem_number--;
                            }

                            mask += "RX_can_data[" + QString::number(elem_number) + "]);\n";
                        }
                        // 2 случай: размер и позиция не кратны 8, не учитывает случай, если нужны биты из двух соседних элементов
                        // массива
                        else
                        {
                            int symbols = sgnl_startbit % 8 == 0 ? 1 : 2;
                            int elem_number = sgnl_startbit / 8; // номер элемента в массиве

                            QString symbol(symbols, '(');
                            QString type = sgnl->type_values.name == "uint8_t" ? "" : "(" + sgnl->type_values.name +")";
                            mask = type + symbol + "RX_can_data[" + QString::number(elem_number) + "] ";

                            if (sgnl_startbit % 8 != 0)
                                mask += ">> " + QString::number(sgnl_startbit %8) + ") ";

                            QString bitmask(sgnl_size, '1');
                            bitmask.prepend("0b");

                            mask += "& " + bitmask + ");\n";
                        }

                        text += mask;
                        file.write(text.toUtf8());
                    }

                    // ----------------------------------------------------


                    text = "}\n\n";
                    file.write(text.toUtf8());
                }
            }

            text = "\n" + prj_name +"MsgBoxes create_" + prj_name + "_rx_message_boxes(canBASE_t *node, " + prj_name + "MsgBoxes *msgBoxes)\n{\n";
            file.write(text.toUtf8());
            int spaces_number;

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                if (data_->msgboxes->at(i)->sender == data_->Reciever){
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
                   "    box.can_int = 1;\n\n";
            file.write(text.toUtf8());


            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                if (data_->msgboxes->at(i)->sender == data_->Reciever){
                    text  = "    box.message_box = msgBoxes->mb_" + data_->msgboxes->at(i)->msgBoxName + ";\n";
                    text += "    box.id = " + data_->msgboxes->at(i)->msgBoxName.toUpper() + "_MSG_ID;\n";
                    text += "    change_message_box(box);\n\n";
                    file.write(text.toUtf8());
                }
            }

            text = "    return *msgBoxes;\n}";
            file.write(text.toUtf8());
        }
        else
            qDebug() << "Check Transform failed";

        mediator_->Notify(2);
    }
}
