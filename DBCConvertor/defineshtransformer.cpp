#include "defineshtransformer.h"


DefinesHTransformer::DefinesHTransformer()
{

}

DefinesHTransformer::DefinesHTransformer(BaseMediator *mediator)
{
    project_data_ = nullptr;
    data_ = nullptr;
    set_mediator(mediator);
}

void DefinesHTransformer::setData(DBCdata *data)
{
    project_data_ = data;
}

void DefinesHTransformer::setData(DBCParsedData *data)
{
    data_ = data;
}

void DefinesHTransformer::exec()
{
    if (project_data_ != nullptr && data_ != nullptr)
    {
        QString fileName = project_data_->get_project_path() + "/" + project_data_->get_project_name() + "_Defines" + ".h";
        QFile file(fileName);

        if ((file.open(QIODevice::WriteOnly)))
        {
            QString prj_name = project_data_->get_project_name();

            QString text =  "#ifndef __INCLUDE_" + prj_name.toUpper() +"_DEFINES_H__\n" +
                    "#define __INCLUDE_" + prj_name.toUpper() + "_DEFINES_H__\n" +
                    "#ifdef __cplusplus\n" +
                    "extern \"C\" { \n" +
                    "#endif \n\n\n" +
                    "#include \"gio.h\"\n" +
                    "#include \"Queue.h\"\n" +
                    "\n\n";
            file.write(text.toUtf8());

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                text = "#define " + data_->msgboxes->at(i)->msgBoxName.toUpper() + "_MSG_ID";
                int spaces_number = LENGTH_SECOND_COL - text.size();
                QString spaces(spaces_number, ' ');
                text += spaces;

                QString ID = data_->msgboxes->at(i)->ID;
                auto LongID = ID.toULongLong();
                QString HexID = QString().number(LongID, 16).toUpper().prepend("0x");
                text += HexID;

                spaces_number = LENGTH_THIRD_COL - text.size();
                spaces = QString(spaces_number, ' ');
                text += spaces;

                text += "// " + data_->msgboxes->at(i)->msgBoxName + "\n";

                file.write(text.toUtf8());
            }

            file.write("\n");

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                text = "#define MB_" +  data_->msgboxes->at(i)->msgBoxName.toUpper() + "_MSG";
                int spaces_number = LENGTH_SECOND_COL - text.size();
                QString spaces(spaces_number, ' ');
                text += spaces;

                QString MBnumber = "canMESSAGE_BOX" + QString().number(project_data_->get_MSGBox_start_value() + i, 10) + "\n";
                text += MBnumber;

                file.write(text.toUtf8());
            }

            text = "\ntypedef struct\n{\n";
            file.write(text.toUtf8());

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                text = "    uint32_t";
                int spaces_number = LENGTH_SECOND_COL - text.size();
                QString spaces(spaces_number, ' ');
                text += spaces + "mb_" + data_->msgboxes->at(i)->msgBoxName + ";\n";
                file.write(text.toUtf8());
            }

            text = "}" + prj_name + "MsgBoxes_t";
            file.write(text.toUtf8());

            // Вывод enum структур -----------------------------------------------------------
            for (int i=0; i<data_->enums->size(); i++)
            {
                auto tmp_enum = data_->enums->at(i);
                text = "\ntypedef enum " + tmp_enum->name + "\n{\n";
                file.write(text.toUtf8());

                QMap<int, QString>::iterator it =tmp_enum->values.begin();
                auto counter = tmp_enum->values.size();
                while (it != tmp_enum->values.end()){
                    text = "    " + it.value().toUpper();
                    int spaces_number = LENGTH_THIRD_COL - text.size();
                    QString spaces(spaces_number, ' ');
                    text += spaces + "= " + QString::number(it.key());
                    if (counter-1 != 0){
                        text += ",\n";
                    }else{
                        text += '\n';
                    }
                    file.write(text.toUtf8());

                    counter--;
                    it++;
                }
                text = "}" + tmp_enum->name + "_t;\n\n";
                file.write(text.toUtf8());
            }

            //
            text = "\ntypedef struct\n{\n    VectorMsgBoxes";
            int spaces_number = LENGTH_SECOND_COL - text.size();
            QString spaces(spaces_number, ' ');
            text += spaces + "msgBoxes;";
            file.write(text.toUtf8());

            text = "    TX_queue_struct";
            spaces_number = LENGTH_SECOND_COL - text.size();
            spaces = QString(spaces_number, ' ');
            text += spaces + "*tx_queue;\n\n";
            file.write(text.toUtf8());

            for (int i=0; i<data_->msgboxes->size(); i++){
                auto msbox = data_->msgboxes->at(i);
                for (int j=0; j<msbox->msgSignals->size(); j++){
                    auto signal = msbox->msgSignals->at(j);
                    text = "    " + signal->type_values.name;
                    int spaces_number = LENGTH_SECOND_COL - text.size();
                    QString spaces(spaces_number, ' ');
                    text += spaces + signal->name + ";\n";
                    file.write(text.toUtf8());
                }
                text = "\n";
                file.write(text.toUtf8());
            }

            text = "} Vector_Struct;\n\n\n";
            text += "#ifdef __cplusplus\n}\n#endif\n\n#endif\n\n\n";
            file.write(text.toUtf8());

            file.close();
        }
        else
            qDebug() << "Check Transform failed";

        mediator_->Notify(2);
    }
}
