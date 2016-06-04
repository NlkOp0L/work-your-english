#ifndef HELPWINDOW_H
#define HELPWINDOW_H

#include "MainWindow.h"
#include <QTextBrowser>



class QPushButton;



class HelpWindow :public QDialog {
    Q_OBJECT

    public:
        HelpWindow(const QString &path, const QString &page, QWidget *parent = 0);
        ~HelpWindow();

        static void showPage(const QString &page, QMainWindow *main);

    private slots:
        void updateWindowTitle();

    private:
        QTextBrowser *text;
        QPushButton *home, *back, *close;
};

#endif // HELPWINDOW_H
