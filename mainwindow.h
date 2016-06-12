#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QApplication>
#include <QPushButton>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QString>
#include <QMenuBar>
#include <QAction>
#include <QIcon>
#include <QRadioButton>
#include <QButtonGroup>
#include "database.h"
#include "dbeditor.h"
#include "dbviewer.h"
#include "helpwindow.h"

#include <QDebug>



class DataBase;
class DbEditor;
class DbViewer;

class MainWindow : public QMainWindow {
    Q_OBJECT

    public:
        MainWindow(QMainWindow *parent = 0);
        ~MainWindow();

        QString frenchToEnglish(QString userWord, DataBase userDb);
        QString englishToFrench(QString userWord, DataBase userDb);
        void createWidgets();
        void disposeWindow();
        void createActions();
        QString randWord(DataBase userDb);
        QString doSearchQuery(QString query, DataBase userDb);
        void init();
        DataBase getDb();
        DbViewer* getDbv();
        void sepAnswers();
        void showSearchResult(QString quest, QList<QString> result);

   private slots:
        void enableEnterButton(QString text);
        void enableSearchButton(QString text);
        void validate();
        void setEF();
        void setFE();
        void about();
        void hideDbEditor();
        void showDbEditor();
        void hideDbViewer();
        void showDbViewer();
        void delDial();
        void showAnswer();
        void help();
        void search();
        void delSearch();
        void delResultDial();
        void manageRB(int id);
        void doSearch();

   private:
        bool selectedMode; //true=EF, false=FE
        int searchMode;
        DataBase db;
        DbEditor *dbed;
        DbViewer *dbv;
        vector<QString> answers;
        QDialog *dial, *searchDial, *resultDial;
        QWidget *window;
        QString word, titleMode, result;
        QPushButton *enter, *searchButton;
        QLabel *toFind,*toFindLabel, *textLineLabel, *image;
        QLineEdit *textLine, *searchText;
        QGroupBox *groupBox;
        QHBoxLayout *h1, *h2, *h3;
        QVBoxLayout *v1, *v2;
        QMenu *fileMenu, *editMenu, *helpMenu;
        QAction *englishFrench, *frenchEnglish, *quit, *helpAction, *aboutAction, *aboutQt, *editDb, *viewDb, *searchAction;
};

#endif // MAINWINDOW_H
