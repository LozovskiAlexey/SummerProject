#include "recievehtransformer.h"

RecieveHTransformer::RecieveHTransformer()
{

}

RecieveHTransformer::RecieveHTransformer(BaseMediator *mediator)
{
    project_data_ = nullptr;
    data_ = nullptr;
    set_mediator(mediator);
}

void RecieveHTransformer::setData(DBCdata *data)
{
    project_data_ = data;
}

void RecieveHTransformer::setData(DBCParsedData *data)
{
    data_ = data;
}

void RecieveHTransformer::exec()
{
    if (project_data_ != nullptr && data_ != nullptr)
    {
        QString fileName = project_data_->get_project_path() + "/" + project_data_->get_project_name() + "_Recieve" + ".h";
        QFile file(fileName);

        if ((file.open(QIODevice::WriteOnly)))
        {
            QString prj_name = project_data_->get_project_name();

            QString text =  "#ifndef __INCLUDE_" + prj_name.toUpper() +"_RECIEVE_H__\n" +
                            "#define __INCLUDE_" + prj_name.toUpper() + "_RECIEVE_H__\n" +
                            "#ifdef __cplusplus\n" +
                            "extern \"C\" { \n" +
                            "#endif \n\n\n" +
                            "#include \"" + prj_name + "_Defines.h\"\n" +
                            "#include \"gio.h\"\n" +
                            "#include \"can_msg.h\"\n" +
                            "#include \"Queue.h\"\n" +
                            "\n\n" +
                            "// RX Messages\n";
            file.write(text.toUtf8());

            for (int i=0; i<data_->msgboxes->size(); i++)
            {
                if (data_->msgboxes->at(i)->sender == data_->Reciever){
                    text = "void " + prj_name + "_recieve_" + data_->msgboxes->at(i)->msgBoxName + "(" +
                            prj_name + "_Struct" + " *vector, uint8_t *RX_can_data);\n";
                    file.write(text.toUtf8());
                }
            }
            text = "\n" + prj_name +"MsgBoxes create_" + prj_name + "_rx_message_boxes(canBASE_t *node, " + prj_name + "MsgBoxes *msgBoxes);\n\n\n" +
                   "#ifdef __cplusplus\n}\n#endif\n\n#endif\n";
            file.write(text.toUtf8());
        }
        else
            qDebug() << "Check Transform failed";

        mediator_->Notify(2);
    }
}
