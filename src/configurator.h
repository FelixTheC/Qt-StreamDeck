#ifndef CONFIGURATOR_H
#define CONFIGURATOR_H

#include <QByteArray>
#include <QListWidgetItem>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QMouseEvent>
#include <QSqlDatabase>
#include <QWidget>
#include "utils.h"
#include <database.h>


namespace Ui {
class Configurator;
}

class Configurator : public QWidget
{
    Q_OBJECT

Q_SIGNALS:
    void close_config();

public:
    explicit Configurator(QWidget *parent = nullptr);
    Configurator(const QString &programName, Database *db, QWidget *parent = nullptr);
    ~Configurator();
    void display();

private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_A_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_6_clicked();
    void on_pushButton_B_clicked();
    void on_pushButton_7_clicked();
    void on_pushButton_8_clicked();
    void on_pushButton_9_clicked();
    void on_pushButton_C_clicked();
    void on_pushButton_star_clicked();
    void on_pushButton_0_clicked();
    void on_pushButton_hashtag_clicked();
    void on_pushButton_D_clicked();

    void on_pushButton_ctrl_clicked();
    void on_pushButton_shift_clicked();
    void on_pushButton_alt_clicked();
    void on_lineEditKey_returnPressed();

    void change_selected_row(QListWidgetItem *row);

private:
    Ui::Configurator *ui;

    Database *stream_db = nullptr;

    int selectedButton = 1;
    int currentIndex = 0;
    int selectedRow = 0;
    bool editMode = false;

    void select_button(const int &val);
    void insert_or_update(const int &row);
    void re_select(const QString &val);
    void init_db();
    void init();
    void cmd_button_pressed(const QString &val);
    void pre_save_btn_cmds(const int &btn);
    void load_from_db();

    QString currentLine = "";
    QString program_name = "";
    QList<QString> *lines = new QList<QString>();

    std::map<int, QJsonObject> *btnCmds = new std::map<int, QJsonObject> {
                                                                         {0, QJsonObject()}, {1, QJsonObject()},
                                                                         {2, QJsonObject()}, {3, QJsonObject()},
                                                                         {4, QJsonObject()}, {5, QJsonObject()},
                                                                         {6, QJsonObject()}, {7, QJsonObject()},
                                                                         {8, QJsonObject()}, {9, QJsonObject()},
                                                                         {10, QJsonObject()}, {11, QJsonObject()},
                                                                         {12, QJsonObject()}, {13, QJsonObject()},
                                                                         {14, QJsonObject()}, {15, QJsonObject()}};
    std::map<int, QString> *btn_map = new std::map<int, QString>{
                                                                 {0, "0"}, {1, "1"}, {2, "2"}, {3, "3"}, {4, "4"},
                                                                 {5, "5"}, {6, "6"}, {7, "7"}, {8, "8"}, {9, "9"},
                                                                 {10, "A"}, {11, "B"},{12, "C"}, {13, "D"}, {14, "*"},
                                                                 {15, "#"}
                                                                };
    std::map<QString, int> *btn_map_str_int = new std::map<QString, int>{
                                                                         {"0", 0}, {"1", 1}, {"2", 2}, {"3", 3}, {"4", 4},
                                                                         {"5", 5}, {"6", 6}, {"7", 7}, {"8", 8}, {"9", 9},
                                                                         {"A", 10}, {"B", 11},{"C", 12}, {"D", 13}, {"*", 14},
                                                                         {"#", 15}
    };
};

#endif // CONFIGURATOR_H
