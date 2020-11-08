#include "mainhtransformer.h"

MainHTransformer::MainHTransformer()
{

}

MainHTransformer::MainHTransformer(BaseMediator *mediator)
{
    project_data_ = nullptr;
    data_ = nullptr;
    set_mediator(mediator);
}

void MainHTransformer::setData(DBCdata *data)
{
    project_data_ = data;
}

void MainHTransformer::setData(DBCParsedData *data)
{
    data_ = data;
}

void MainHTransformer::exec()
{
    if (project_data_ != nullptr && data_ != nullptr)
    {
        QString fileName = project_data_->get_project_path() + "/" + project_data_->get_project_name() + ".h";
        QFile file(fileName);

        if ((file.open(QIODevice::WriteOnly)))
        {
            QString prj_name = project_data_->get_project_name();

            QString text =  "#ifndef __INCLUDE_" + prj_name.toUpper() +"_H__\n" +
                            "#define __INCLUDE_" + prj_name.toUpper() + "_H__\n" +
                            "#ifdef __cplusplus\n" +
                            "extern \"C\" { \n" +
                            "#endif \n\n\n" +
                            "#include \"can.h\"\n" +
                            "#include \"gio.h\"\n" +
                            "#include \"can_msg.h\"\n" +
                            "#include" + "\"" + prj_name + "_Defines.h\"\n"
                            "#include" + "\"" + prj_name + "_Transmit.h\"\n"
                            "#include" + "\"" + prj_name + "_Recieve.h\"\n"
                            "#include \"Queue.h\"\n\n\n";
            file.write(text.toUtf8());

            text = "void " + prj_name + "_init(canBASE_t *node," + prj_name + "_Struct *vector, TX_queue_struct *tx_queue);\n";
            file.write(text.toUtf8());
            text = "void " + prj_name + "queue_call(" + prj_name + "_Struct *vector, uint32_t rti_count);\n";
            file.write(text.toUtf8());
            text = "void " + prj_name + "_recieve_call(" + prj_name + "_Struct *vector, uint32_t messageBox, uint8_t *RX_can_data);\n";
            file.write(text.toUtf8());

            text = "#ifdef __cplusplus\n}\n#endif\n\n#endif\n\n\n";
            file.write(text.toUtf8());
        }
        else
            qDebug() << "Check Transform failed";

        mediator_->Notify(2);
    }
}
