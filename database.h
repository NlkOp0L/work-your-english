#ifndef DATABASE_H
#define DATABASE_H

#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QString>
#include <QVariant>



using namespace std;

class DataBase {
    public:
        DataBase();
        ~DataBase();

        void connect();
        QString query(QString userQuery, QString type);
};

#endif // DATABASE_H
