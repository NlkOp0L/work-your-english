#include "helpwindow.h"



HelpWindow::HelpWindow(const QString &path, const QString &page, QWidget *parent) : QDialog(parent) {
    setAttribute(Qt::WA_DeleteOnClose);
    setAttribute(Qt::WA_GroupLeader);

    text = new QTextBrowser;

    home = new QPushButton(tr("&Acceuil"));
    back = new QPushButton(tr("&<--"));
    close = new QPushButton(tr("&Fermer"));
    close->setShortcut(tr("Esc"));

    QHBoxLayout *buttonLayout = new QHBoxLayout;
    buttonLayout->addWidget(home);
    buttonLayout->addWidget(back);
    buttonLayout->addStretch();
    buttonLayout->addWidget(close);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(text);
    setLayout(mainLayout);

    connect(home, SIGNAL(clicked()),
            text, SLOT(home()));
    connect(back, SIGNAL(clicked()),
            text, SLOT(backward()));
    connect(close, SIGNAL(clicked()),
            this, SLOT(close()));
    connect(text, SIGNAL(sourceChanged(const QUrl &)),
            this, SLOT(updateWindowTitle()));

    text->setSearchPaths(QStringList() << path << "./images");
    text->setSource(page);
}



void HelpWindow::updateWindowTitle() {
    setWindowTitle(tr("Help: %1").arg(text->documentTitle()));
}



void HelpWindow::showPage(const QString &page, QMainWindow *main) {
    QString path = QApplication::applicationDirPath() + "/doc";
    HelpWindow *help = new HelpWindow(path, page, main);
    help->setFixedSize(800, 600);
    help->show();
}



HelpWindow::~HelpWindow() {}
