#ifndef DATABASE_H
#define DATABASE_H

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include "utils.h"


class Database
{
public:
    Database();
    QSqlDatabase *stream_db;

    [[ nodiscard ]] bool button_exists(const QString &btn);
    [[ nodiscard ]] bool insert_button(const QString &btn);
    [[ nodiscard ]] int get_button_pk_by_name(const QString &btn);
    [[ nodiscard ]] bool insert_or_update_command(const QString &btn,
                                                const QString &program,
                                                const QString &cmd);
    [[ nodiscard ]] QString get_cmd_by_program_button(const QString &program,
                                                    const QString &btn);
    [[ nodiscard ]] QList<QString> get_all_programs();

    void get_commands_by_program(QString &program, QList<Command> *container);

private:
    void init_db();
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");

    const QString db_name = "streamDeck.db3";

};

#endif // DATABASE_H
