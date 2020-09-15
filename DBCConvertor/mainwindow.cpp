#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    facade_ = new Facade;

    connect(facade_, SIGNAL(workFinishedSignal()), this, SLOT(workFinished_slot()));
    connect(facade_, SIGNAL(failedToFinishWorkSignal()), this, SLOT(failedToFinishWork_slot()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete facade_;
}

// Обновление имен файлов (в радиокнопках) в соответствии с назваинем проекта
void MainWindow::updateProjectName(const QString &new_name)
{
    ui->MainRBC->setText(new_name + ".c");
    ui->MainRBH->setText(new_name + ".h");

    ui->TransmitRBC->setText(new_name + "_Transmit.c");
    ui->TransmitRBH->setText(new_name + "_Transmit.h");

    ui->RecieveRBC->setText(new_name + "_Recieve.c");
    ui->RecieveRBH->setText(new_name + "_Recieve.h");

    ui->DefinesRBH->setText(new_name + "_Defines.h");
}

// динамически обновляет названия файлов проекта, в соответствии с
// введенным пользователем названием проекта
void MainWindow::on_ProjectNameLineEdit_textChanged(const QString &new_project_name)
{
    updateProjectName(new_project_name);
}

// Записывает путь к dbc файлу
void MainWindow::on_pushButton_pressed()
{
    ui->DBClineEdit->clear();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Открыть файл"), "",
                                                    tr("Files (*.dbc)"));

    ui->DBClineEdit->setText(fileName);
}

// Записывает путь к папке, куда будет сохраняться готовый проект
void MainWindow::on_ChooseDirectoryPushButton_pressed()
{
    ui->UpLoadLineEdit->clear();
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "/home",
                                                    QFileDialog::ShowDirsOnly |
                                                    QFileDialog::DontResolveSymlinks);

    ui->UpLoadLineEdit->setText(dir);
}

void MainWindow::on_ExecPushButton_clicked()
{
    auto structure = new project_structure_t;

    structure->main_c = ui->MainRBC->isChecked();
    structure->main_h = ui->MainRBH->isChecked();
    structure->transmit_c = ui->TransmitRBC->isChecked();
    structure->transmit_h = ui->TransmitRBH->isChecked();
    structure->recieve_c = ui->RecieveRBC->isChecked();
    structure->recieve_h = ui->RecieveRBH->isChecked();
    structure->defines_h = ui->DefinesRBH->isChecked();

    auto project = new DBCdata;

    project->set_project_name(ui->ProjectNameLineEdit->text());
    project->set_project_path(ui->UpLoadLineEdit->text());
    project->set_DBC_flie_name(ui->DBClineEdit->text());
    project->set_MSGBox_start_value(ui->MSBoxSpinBox->value());
    project->set_project_structure(*structure);

    facade_->convertDBC(project);
}

void MainWindow::updateCHBState()
{
    bool choose_all_CHB_state = 1;

    choose_all_CHB_state &= ui->MainRBC->isChecked();
    choose_all_CHB_state &= ui->MainRBH->isChecked();
    choose_all_CHB_state &= ui->TransmitRBC->isChecked();
    choose_all_CHB_state &= ui->TransmitRBH->isChecked();
    choose_all_CHB_state &= ui->RecieveRBC->isChecked();
    choose_all_CHB_state &= ui->RecieveRBH->isChecked();
    choose_all_CHB_state &= ui->DefinesRBH->isChecked();

    auto result = Qt::CheckState(choose_all_CHB_state);

    result = choose_all_CHB_state == 0 ? Qt::CheckState(0) : Qt::CheckState(2);
    ui->ChooseAll->setCheckState(result);
}

void MainWindow::on_MainRBC_stateChanged()
{
    updateCHBState();
}

void MainWindow::on_MainRBH_stateChanged()
{
    updateCHBState();
}

void MainWindow::on_TransmitRBC_stateChanged()
{
    updateCHBState();
}

void MainWindow::on_TransmitRBH_stateChanged()
{
    updateCHBState();
}

void MainWindow::on_RecieveRBC_stateChanged()
{
    updateCHBState();
}

void MainWindow::on_RecieveRBH_stateChanged()
{
    updateCHBState();
}

void MainWindow::on_DefinesRBH_stateChanged()
{
    updateCHBState();
}

void MainWindow::on_ChooseAll_clicked()
{
    auto new_state = ui->ChooseAll->checkState();
    new_state = new_state == 1 ?  new_state : Qt::CheckState(2);

    ui->MainRBC->setCheckState(new_state);
    ui->MainRBH->setCheckState(new_state);
    ui->TransmitRBC->setCheckState(new_state);
    ui->TransmitRBH->setCheckState(new_state);
    ui->RecieveRBC->setCheckState(new_state);
    ui->RecieveRBH->setCheckState(new_state);
    ui->DefinesRBH->setCheckState(new_state);
}

void MainWindow::workFinished_slot()
{
    QMessageBox msgBox;
    msgBox.setText("Work finished");
    msgBox.exec();
}

void MainWindow::failedToFinishWork_slot()
{
    QMessageBox msgBox;
    msgBox.setText("Error caught");
    msgBox.exec();
}
