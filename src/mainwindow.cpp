#include "configurator.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    serial_reader = new SerialReader_QT("/dev/ttyACM0");
    db = new Database();

    connect(myProcess,
            &QProcess::finished,
            this,
            &MainWindow::readProcessOutput);


    if (serial_reader->isOpen())
    {
        connect(serial_reader,
                &SerialReader_QT::serialValueReceived,
                this,
                &MainWindow::simulateKeyPress);

    }

    for (auto &val : db->get_all_programs())
    {
        ui->listWidgetProgram->addItem(val);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void
MainWindow::simulateKeyPress(std::string &val)
{
    QString tmp = QString::fromStdString(val)[0];

    if (tmp != currentBtn)
    {
        currentBtn = tmp;
        btnPressedCount = 0;
    }

    QString cmd_by_index = get_command_from_btn_and_index();

    if (countCommand > 1)
    {
        ++btnPressedCount;
    }

    //auto key = QKeySequence(Qt::ControlModifier | Qt::Key_B);

    QString program = "xdotool";
    QStringList args;
    // args << "key" << "ctrl+alt+l" << "ctrl+alt+o";
    args << "key" << cmd_by_index;

    // xdotool search --name "LibreOffice Calc"

    myProcess->start(program, args);
}

void
MainWindow::readProcessOutput(int exitCode, QProcess::ExitStatus exitStatus)
{

    qDebug() << exitCode << " " << exitStatus;
    qDebug() << myProcess->readLine();

}


void
MainWindow::on_pushButton_clicked()
{
    if (ui->lineEditProgramName->text().isEmpty())
    {
        ui->lineEditProgramName->setFocus();
        QMessageBox::warning(this, "Missing Information", "Please set a Program.");
        return;
    }

    Configurator *config = new Configurator(ui->lineEditProgramName->text(), db, this);

    connect(config,
            &Configurator::close_config,
            this,
            &MainWindow::reload_page);

    config->display();

    ui->lineEditProgramName->hide();
    ui->listWidgetProgram->hide();
    ui->pushButton->hide();
}

void
MainWindow::reload_page()
{
    ui->lineEditProgramName->show();
    ui->listWidgetProgram->show();
    ui->pushButton->show();
    ui->listWidgetProgram->clear();
    for (auto &val : db->get_all_programs())
    {
        ui->listWidgetProgram->addItem(val);
    }
    this->repaint();
}

void
MainWindow::on_listWidgetProgram_doubleClicked(const QModelIndex &index)
{
    auto *item = ui->listWidgetProgram->itemFromIndex(index);
    ui->lineEditProgramName->setText(item->text());
}


QString
MainWindow::get_command_from_btn_and_index()
{
    QJsonParseError *err = nullptr;
    QString data = db->get_cmd_by_program_button(ui->lineEditProgramName->text(), currentBtn);

    QByteArray dataByteArray = QByteArray(data.toStdString().c_str(), data.count());
    QJsonDocument jsonDoc = QJsonDocument::fromJson(dataByteArray, err);

    if (err == nullptr)
    {
        QJsonObject jsonObj = jsonDoc.object();
        QString currIndex = QString::number(btnPressedCount);
        QJsonValue val = jsonObj.take(currIndex);

        countCommand = jsonObj.count();

        if (val != QJsonValue::Undefined)
        {
            auto result = val.toString();
            return result.replace(" ", "+");
        }
    }
    else
    {
        qDebug() << err->errorString();
    }

    return "";
}
