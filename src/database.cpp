#include "database.h"

Database::Database()
{
    auto run_init = !QFile(db_name).exists();
    db.setDatabaseName(db_name);
    stream_db = new QSqlDatabase(db);

    if (run_init)
        init_db();
}

void
Database::init_db()
{
    if (stream_db->open())
    {
        const QString create_table_stream_deck = "create table if not exists stream_deck"\
                                                 " ("\
                                                 " id       integer not null constraint stream_deck_pk primary key autoincrement,"\
                                                 " button   string"\
                                                 ");";
        QSqlQuery query_stream_deck;
        query_stream_deck.exec(create_table_stream_deck);
        stream_db->commit();

        const QString create_table_command = "create table command"\
                                             " ("\
                                             " id                   integer     not null constraint command_pk primary key autoincrement,"\
                                             " stream_deck_btn      integer     references stream_deck,"\
                                             " program              string          not null,"\
                                             " cmd                  text            not null"\
                                             " );";
        QSqlQuery query_command;
        query_command.exec(create_table_command);
        stream_db->commit();

        QSqlQuery query_command_id_uindex;
        query_command_id_uindex.exec("create unique index command_id_uindex on command (id);");
        stream_db->commit();

        QSqlQuery query_command_program_index;
        query_command_program_index.exec("create index command_program_index on command (program);");
        stream_db->commit();

        QSqlQuery query_stream_deck_button_uindex;
        query_stream_deck_button_uindex.exec("create unique index stream_deck_button_uindex on stream_deck (button);");
        stream_db->commit();

        QSqlQuery query_stream_deck_id_uindex;
        query_stream_deck_id_uindex.exec("create unique index stream_deck_id_uindex on stream_deck (id);");
        stream_db->commit();

        stream_db->close();

    }
    else
    {
        QMessageBox::warning(nullptr, "Database error", "The internal database couldn't be loaded.");
    }

}

bool
Database::insert_or_update_command(const QString &btn, const QString &program, const QString &cmd)
{
    if (stream_db->open())
    {
        QSqlQuery query;
        QSqlQuery exists;
        bool success = true;
        int count = -1;

        auto btnId = get_button_pk_by_name(btn);

        exists.prepare("SELECT count(*) FROM command "
                       "WHERE program=:program "
                       "AND stream_deck_btn=:button");

        exists.bindValue(":button", btnId);
        exists.bindValue(":program", program);
        exists.exec();
        exists.first();

        if (exists.isValid())
        {
            count = exists.value(0).toInt();
        }

        if (count != 0)
        {
            query.prepare("UPDATE command "
                          "SET cmd=:cmd "
                          "WHERE program=:program "
                          "AND stream_deck_btn=:button");
        }
        else if (count == 0)
        {
            query.prepare("INSERT INTO command (stream_deck_btn, program, cmd) "
                          "VALUES (:button, :program, :cmd)");
        }

        query.bindValue(":button", btnId);
        query.bindValue(":program", program);
        query.bindValue(":cmd", cmd);
        success = query.exec();

        if (!success)
        {
            auto err = query.lastError();
            qDebug() << "Database: " << err.databaseText();
            qDebug() << "Is valid: " << err.isValid();
            return false;

        }
        stream_db->close();
        return true;
    }
    return false;
}

bool
Database::button_exists(const QString &btn)
{
    if (stream_db->open())
    {
        QSqlQuery query;
        query.prepare("SELECT count(*) FROM stream_deck WHERE button=:button");
        query.bindValue(":button", btn);
        query.exec();
        query.first();
        auto result = query.value(0).toInt();

        stream_db->close();

        return result > 0;

    }
    return false;
}

bool
Database::insert_button(const QString &btn)
{
    if (stream_db->open())
    {
        QSqlQuery query;
        query.prepare("INSERT INTO stream_deck (button) VALUES (:button)");
        query.bindValue(":button", btn);
        auto result = query.exec();

        if (!result)
        {
            auto err = query.lastError();
            qDebug() << "Database: " << err.databaseText();
            qDebug() << "Is valid: " << err.isValid();
            return false;

        }
        stream_db->close();
        return true;
    }
    return false;
}

int
Database::get_button_pk_by_name(const QString &btn)
{
    QSqlQuery query;
    query.prepare("SELECT id from stream_deck WHERE button=:button");
    query.bindValue(":button", btn);
    query.exec();
    query.first();

    return query.value(0).toInt();
}

QString
Database::get_cmd_by_program_button(const QString &program, const QString &btn)
{
    if (stream_db->open())
    {
        QSqlQuery query;
        QString result = "";
        query.prepare("SELECT cmd "
                      "FROM command "
                      "JOIN stream_deck s on s.id = command.stream_deck_btn "
                      "WHERE s.button=:button "
                      "AND command.stream_deck_btn = s.id "
                      "AND program=:program");

        query.bindValue(":button", btn);
        query.bindValue(":program", program);

        query.exec();
        query.first();
        result = query.value(0).toString();
        stream_db->close();

        return result;
    }
    return "";
}

void
Database::get_commands_by_program(QString &program, QList<Command> *container)
{
    if (stream_db->open())
    {

        QSqlQuery query;
        query.prepare("SELECT program, cmd, sd.id, sd.button "
                      "FROM command, stream_deck AS sd "
                      "    JOIN stream_deck s on s.id = command.stream_deck_btn "
                      "    WHERE program=:program "
                      "      AND command.stream_deck_btn = sd.id");
        query.bindValue(":program", program);
        query.exec();

        while (query.next())
        {
            Command obj = Command();
            obj.program = query.value(0).toString();
            obj.command = query.value(1).toString();
            obj.btn_id = query.value(2).toInt();
            obj.btn_name = query.value(3).toString();

            container->emplaceBack(obj);
        }

        stream_db->close();
    }
}

QList<QString>
Database::get_all_programs()
{
    if (stream_db->open())
    {
        QList<QString> data = QList<QString>();
        QSqlQuery query("SELECT DISTINCT(program) FROM command ORDER BY program");
        query.exec();

        while (query.next())
        {
            auto program = query.value(0).toString();
            data.emplaceBack(program);
        }

        stream_db->close();

        return data;
    }
}
