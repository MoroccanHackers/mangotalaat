#include "user.h"
#include "databasemanager.h"

namespace Database {

Model::User User::getById(int id)
{
    QList<Model::User> users ;

    DatabaseManager mgr;
    QSqlQuery query;

    query.prepare("SELECT * FROM users WHERE id = ?");
    query.addBindValue(id);

    while(query.next()){
        int id = query.value(0).toInt();
        QString username = query.value(1).toString();
        QString password = query.value(2).toString();
        QDateTime registerTime = query.value(3).toDateTime();

        Model::User user(id, username, password, registerTime);
        users.append(user);
    }

    return users.first();
}

QList<Model::User> User::getAll()
{
    QList<Model::User> users ;

    DatabaseManager mgr;
    QSqlQuery query("SELECT * FROM users");

    while(query.next()) {
        int id = query.value(0).toInt();
        QString username = query.value(1).toString();
        QString password = query.value(2).toString();
        QDateTime registerTime = query.value(3).toDateTime();

        Model::User user(id, username, password, registerTime);
        users.append(user);
    }

    return user;
}

}
