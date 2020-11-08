#include "transmithtransformer.h"

TransmitHTransformer::TransmitHTransformer()
{

}

TransmitHTransformer::TransmitHTransformer(BaseMediator *mediator)
{
    project_data_ = nullptr;
    data_ = nullptr;

    set_mediator(mediator);
}

void TransmitHTransformer::setData(DBCdata *data)
{
    project_data_ = data;
}

void TransmitHTransformer::setData(DBCParsedData *data)
{
    data_ = data;
}

void TransmitHTransformer::exec()
{
    if (project_data_ != nullptr && data_ != nullptr)
    {
        QString fileName = project_data_->get_project_path() + "/" + project_data_->get_project_name() + "_Transmit" + ".h";
        QFile file(fileName);

        if ((file.open(QIODevice::WriteOnly)))
        {
            QString prj_name = project_data_->get_project_name();

            QString text =  "#ifndef __INCLUDE_" + prj_name.toUpper() +"_TRANSMIT_H__\n" +
                            "#define __INCLUDE_" + prj_name.toUpper() + "_TRANSMIT_H__\n" +
                            "#ifdef __cplusplus\n" +
                            "extern \"C\" { \n" +
                            "#endif \n\n\n" +
                            "#include \"" + prj_name + "_Defines.h\"\n" +
                            "#include \"gio.h\"\n" +
                            "#include \"can_msg.h\"\n" +
                            "#include \"Queue.h\"\n" +
                            "#include <math.h>\n\n\n" +
                            "// TX Messages\n";
            file.write(text.toUtf8());

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                QString sender = data_->msgboxes->at(i)->sender;
                QString trans = data_->Transmitter;
                if (data_->msgboxes->at(i)->sender == data_->Transmitter){
                    text = "void " + prj_name + "_queue_" + data_->msgboxes->at(i)->msgBoxName + "(" +
                            prj_name + "_Struct" + " *vector, uint32_t rti_count, uint32_t period" + ")\n";
                    file.write(text.toUtf8());
                }
            }
            text = "\n" + prj_name +"MsgBoxes create_" + prj_name + "_tx_message_boxes(canBASE_t *node, " + prj_name + "MsgBoxes *msgBoxes);\n\n\n" +
                   "#ifdef __cplusplus\n}\n#endif\n\n#endif\n";
            file.write(text.toUtf8());

            file.close();
        }
        else
            qDebug() << "Check Transform failed";

        mediator_->Notify(2);
    }
}
