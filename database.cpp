#include "database.h"



DataBase::DataBase() {}



/*********************************
Create connection to the Database
*********************************/
void DataBase::connect() {
    QSqlDatabase db;
    db= QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("dico.db");

    if (!db.open()) {
        QMessageBox::critical(0, "Erreur", "Impossible de se connecter à la base de donnée");
    }
}



/*********************************
Execute a query
*********************************/
QString DataBase::query(QString userQuery, QString type) {
    QSqlQuery query;
    QString result;

    query.exec(userQuery);

    if(type == "SELECT") {
        while (query.next()) {
             result = query.value(0).toString();
         }
        return result;
    }
    else return "query applied";
}



DataBase::~DataBase() {}
