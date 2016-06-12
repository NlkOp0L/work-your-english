#ifndef DBEDITOR_H
#define DBEDITOR_H

#include "mainwindow.h"

class MainWindow;

class DbEditor : public QDialog {
    Q_OBJECT

    public:
        DbEditor(QWidget *parent, MainWindow *main);
        ~DbEditor();

        void createWidgets();
        void disposeWidget();

    private slots:
        void addDb();

    private:
        MainWindow *mainWindow;
        QString query;
        QPushButton *add, *quit;
        QLabel *frenchLabel,*englishLabel;
        QLineEdit *french, *english;
        QGroupBox *groupBox;
        QHBoxLayout *h1, *h2;
        QVBoxLayout *v1, *v2, *v3;
};

#endif // DBEDITOR_H
