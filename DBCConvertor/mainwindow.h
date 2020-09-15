#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>
#include "dbcdata.h"
#include "facade.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void updateProjectName(const QString &new_name);
    void updateCHBState();

public slots:
    void workFinished_slot();

    void failedToFinishWork_slot();

private slots:
    void on_ProjectNameLineEdit_textChanged(const QString &new_project_name);

    void on_pushButton_pressed();

    void on_ChooseDirectoryPushButton_pressed();

    void on_ExecPushButton_clicked();

    void on_MainRBC_stateChanged();

    void on_MainRBH_stateChanged();

    void on_TransmitRBC_stateChanged();

    void on_TransmitRBH_stateChanged();

    void on_RecieveRBC_stateChanged();

    void on_RecieveRBH_stateChanged();

    void on_DefinesRBH_stateChanged();

    void on_ChooseAll_clicked();

private:
    Ui::MainWindow *ui;
    Facade *facade_;
};
#endif // MAINWINDOW_H
