#include "configurator.h"
#include "ui_configurator.h"

Configurator::Configurator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Configurator)
{
    ui->setupUi(this);
    init();
}

Configurator::Configurator(const QString &programName, Database *db, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Configurator)
{

    ui->setupUi(this);
    ui->lineEditProgram->setText(programName);

    stream_db = db;
    program_name = programName;

    init_db();
    init();
    load_from_db();
}

Configurator::~Configurator()
{
    lines = {};
    delete ui;
}

void
Configurator::on_buttonBox_accepted()
{
    if (stream_db != nullptr)
    {
        for (auto const& [key, val] : *btnCmds)
        {
            if (val.count() > 0)
            {
                auto btnName = btn_map->at(key);
                QString cmd = QJsonDocument(val).toJson(QJsonDocument::JsonFormat::Compact);

                auto result = stream_db->insert_or_update_command(btnName,
                                                                  ui->lineEditProgram->text(),
                                                                  cmd);
                if (!result)
                {
                    QMessageBox::warning(this, "Saving error", "Couldn't save command");
                    break;
                }
            }
        }
    }

    emit close_config();
    this->close();
}


void
Configurator::on_buttonBox_rejected()
{
    emit close_config();
    this->close();
}


void
Configurator::display()
{
    this->show();
    this->ui->pushButton_1->setStyleSheet("background-color:red");
}

void
Configurator::select_button(const int &val)
{
    pre_save_btn_cmds(selectedButton);
    ui->listWidgetCommand->clear();

    ui->pushButton_0->setStyleSheet("");
    ui->pushButton_1->setStyleSheet("");
    ui->pushButton_2->setStyleSheet("");
    ui->pushButton_3->setStyleSheet("");
    ui->pushButton_4->setStyleSheet("");
    ui->pushButton_5->setStyleSheet("");
    ui->pushButton_6->setStyleSheet("");
    ui->pushButton_7->setStyleSheet("");
    ui->pushButton_8->setStyleSheet("");
    ui->pushButton_9->setStyleSheet("");
    ui->pushButton_A->setStyleSheet("");
    ui->pushButton_B->setStyleSheet("");
    ui->pushButton_C->setStyleSheet("");
    ui->pushButton_D->setStyleSheet("");
    ui->pushButton_star->setStyleSheet("");
    ui->pushButton_hashtag->setStyleSheet("");

    selectedButton = val;
    currentLine = "";
    editMode = false;

    QJsonObject jsonObject = btnCmds->at(selectedButton);
    currentIndex = jsonObject.count();

    const auto keys = jsonObject.keys();
    std::for_each(keys.begin(),
                  keys.end(),
                  [&](const auto &val){
                      const QString json_val = jsonObject[val].toString();
                      ui->listWidgetCommand->addItem(json_val);
                  });

    switch (val) {
    case 0:
        this->ui->pushButton_0->setStyleSheet("background-color:red");
        break;
    case 1:
        this->ui->pushButton_1->setStyleSheet("background-color:red");
        break;
    case 2:
        this->ui->pushButton_2->setStyleSheet("background-color:red");
        break;
    case 3:
        this->ui->pushButton_3->setStyleSheet("background-color:red");
        break;
    case 4:
        this->ui->pushButton_4->setStyleSheet("background-color:red");
        break;
    case 5:
        this->ui->pushButton_5->setStyleSheet("background-color:red");
        break;
    case 6:
        this->ui->pushButton_6->setStyleSheet("background-color:red");
        break;
    case 7:
        this->ui->pushButton_7->setStyleSheet("background-color:red");
        break;
    case 8:
        this->ui->pushButton_8->setStyleSheet("background-color:red");
        break;
    case 9:
        this->ui->pushButton_9->setStyleSheet("background-color:red");
        break;
    case 10:
        this->ui->pushButton_A->setStyleSheet("background-color:red");
        break;
    case 11:
        this->ui->pushButton_B->setStyleSheet("background-color:red");
        break;
    case 12:
        this->ui->pushButton_C->setStyleSheet("background-color:red");
        break;
    case 13:
        this->ui->pushButton_D->setStyleSheet("background-color:red");
        break;
    case 14:
        this->ui->pushButton_star->setStyleSheet("background-color:red");
        break;
    case 15:
        this->ui->pushButton_hashtag->setStyleSheet("background-color:red");
        break;
    default:
        break;
    }
}


void
Configurator::on_pushButton_1_clicked()
{
    select_button(1);
}


void
Configurator::on_pushButton_2_clicked()
{
    select_button(2);
}


void
Configurator::on_pushButton_3_clicked()
{
    select_button(3);
}


void
Configurator::on_pushButton_A_clicked()
{
    select_button(10);
}


void
Configurator::on_pushButton_4_clicked()
{
    select_button(4);
}


void
Configurator::on_pushButton_5_clicked()
{
    select_button(5);
}


void
Configurator::on_pushButton_6_clicked()
{
    select_button(6);
}


void
Configurator::on_pushButton_B_clicked()
{
    select_button(11);
}


void
Configurator::on_pushButton_7_clicked()
{
    select_button(7);
}


void
Configurator::on_pushButton_8_clicked()
{
    select_button(8);
}


void
Configurator::on_pushButton_9_clicked()
{
    select_button(9);
}


void
Configurator::on_pushButton_C_clicked()
{
    select_button(12);
}


void
Configurator::on_pushButton_star_clicked()
{
    select_button(14);
}


void
Configurator::on_pushButton_0_clicked()
{
    select_button(0);
}


void
Configurator::on_pushButton_hashtag_clicked()
{
    select_button(15);
}


void
Configurator::on_pushButton_D_clicked()
{
    select_button(13);
}


void
Configurator::on_pushButton_ctrl_clicked()
{
    const auto str = "ctrl ";
    cmd_button_pressed(str);
}


void
Configurator::on_pushButton_shift_clicked()
{
    const auto str = "shift ";
    cmd_button_pressed(str);
}


void
Configurator::on_pushButton_alt_clicked()
{
    const auto str = "alt ";
    cmd_button_pressed(str);
}


void
Configurator::cmd_button_pressed(const QString &val)
{
    QPushButton *pushBtn;
    if (val == "alt ")
        pushBtn = ui->pushButton_alt;
    if (val == "ctrl ")
        pushBtn = ui->pushButton_ctrl;
    if (val == "shift ")
        pushBtn = ui->pushButton_shift;


    if (editMode && currentLine.contains(val))
    {
        currentLine.replace(val, "");
        pushBtn->setStyleSheet("");
    }
    else
    {
        currentLine += val;
        pushBtn->setStyleSheet("background-color:red");
        if (!editMode)
            insert_or_update(currentIndex);
    }
}

void
Configurator::on_lineEditKey_returnPressed()
{
    currentLine += ui->lineEditKey->text();
    if (editMode)
    {
        ui->listWidgetCommand->item(selectedRow)->setText(currentLine);
        editMode = false;
    }
    else
    {
        insert_or_update(currentIndex);
        ++currentIndex;
    }

    ui->lineEditKey->clear();
    ui->pushButton_ctrl->setStyleSheet("");
    ui->pushButton_shift->setStyleSheet("");
    ui->pushButton_alt->setStyleSheet("");

    currentLine = "";
    pre_save_btn_cmds(selectedButton);
}


void
Configurator::init_db()
{
    std::for_each(btn_map->begin(),
                  btn_map->end(),
                  [&](const auto &val){
                      if (!stream_db->button_exists(val.second))
                      {
                          stream_db->insert_button(val.second);
                      }
                  });
}


void
Configurator::insert_or_update(const int &row)
{
    auto tmp = ui->listWidgetCommand->item(row);
    if (tmp)
        tmp->setText(currentLine);
    else
        ui->listWidgetCommand->insertItem(row, currentLine);
}


void
Configurator::init()
{
    connect(ui->listWidgetCommand,
            &QListWidget::itemDoubleClicked,
            this,
            &Configurator::change_selected_row);
}


void
Configurator::change_selected_row(QListWidgetItem *row)
{
    if (qApp->mouseButtons() & Qt::RightButton)
    {
        delete row;
        currentIndex = ui->listWidgetCommand->count();
        pre_save_btn_cmds(selectedButton);
        return;
    }
    ui->pushButton_ctrl->setStyleSheet("");
    ui->pushButton_shift->setStyleSheet("");
    ui->pushButton_alt->setStyleSheet("");

    selectedRow = row->listWidget()->currentRow();
    const auto line = row->text();
    const auto values = line.split(" ");
    std::for_each(values.begin(),
                  values.end(),
                  [&](const auto val){
                      re_select(val);
                  });
    editMode = true;
}


void
Configurator::re_select(const QString &val)
{
    if (val == "ctrl")
    {
        ui->pushButton_ctrl->setStyleSheet("background-color:red");
        currentLine += "ctrl ";
    }
    else if (val == "shift")
    {
        ui->pushButton_shift->setStyleSheet("background-color:red");
        currentLine += "shift ";
    }
    else if (val == "alt")
    {
        ui->pushButton_alt->setStyleSheet("background-color:red");
        currentLine += "alt ";
    }
    else
        ui->lineEditKey->setText(val);
}


void
Configurator::pre_save_btn_cmds(const int &btn)
{
    QJsonObject jsonObject;

    for (int i = jsonObject.count(); i < ui->listWidgetCommand->count(); ++i)
    {
        const auto *row = ui->listWidgetCommand->item(i);
        if (row != nullptr)
        {
            jsonObject.insert(QString::number(i), row->text());
        }
    }

    btnCmds->at(btn) = jsonObject;
}


void
Configurator::load_from_db()
{
    QJsonParseError *err = nullptr;
    QList<Command> *data = new QList<Command>();

    if (stream_db != nullptr)
    {
        stream_db->get_commands_by_program(program_name, data);

        for (auto &cmd : *data)
        {

            auto bytes = QByteArray(cmd.command.toStdString().c_str(), cmd.command.length());
            QJsonDocument jsonDocument = QJsonDocument::fromJson(bytes, err);

            if (err != nullptr)
            {
                qDebug() << err->errorString();
            }
            else
            {
                int btn_id = btn_map_str_int->at(cmd.btn_name);
                QJsonObject jsonObject = jsonDocument.object();

                btnCmds->at(btn_id) = jsonObject;
            }
        }

        QJsonObject jsonObject = btnCmds->at(1);

        const auto keys = jsonObject.keys();
        std::for_each(keys.begin(),
                      keys.end(),
                      [&](const auto &val){
                          const QString json_val = jsonObject[val].toString();
                          ui->listWidgetCommand->addItem(json_val);
                      });

    }
}
