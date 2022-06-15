#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QByteArray>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonValue>
#include <QProcess>
#include <QMainWindow>

#include "database.h"
#include "serialreader_qt.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

private slots:
    void simulateKeyPress(std::string &val);
    void readProcessOutput(int exitCode, QProcess::ExitStatus exitStatus);

    void on_pushButton_clicked();
    void reload_page();

    void on_listWidgetProgram_doubleClicked(const QModelIndex &index);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    SerialReader_QT *serial_reader;
    Database *db;
    QProcess *myProcess = new QProcess(this);
    QSet<QString> set;
    QString currentBtn = "";
    int btnPressedCount = 0;
    int countCommand = 1;
    QString get_command_from_btn_and_index();

};
#endif // MAINWINDOW_H
