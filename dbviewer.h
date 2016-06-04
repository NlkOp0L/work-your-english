#ifndef DBVIEWER_H
#define DBVIEWER_H

#include "MainWindow.h"
#include <QTableWidget>
#include <vector>
#include <QHeaderView>

class MainWindow;

class DbViewer : public QDialog {
    Q_OBJECT

    public:
        DbViewer(QWidget *parent, MainWindow *main);
        ~DbViewer();

        void createWidgets();
        void disposeWidget();
        void fillTable();
        void emptyTable();
        QTableWidget* getTable();

    private slots:
        void removeWord();

    private:
        MainWindow *mainWindow;
        QString query;
        QTableWidget *table;
        QPushButton *quit, *remove, *add, *search;
        QGroupBox *groupBox;
        QHBoxLayout *h, *h1;
        QVBoxLayout *v;
};

#endif // DBVIEWER_H
