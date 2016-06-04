#include "mainwindow.h"
#include "database.h"
#include <iostream>



MainWindow::MainWindow(QMainWindow *parent) : QMainWindow(parent), word(""), titleMode("") {
    setWindowTitle(tr("WorkYourEnglish"));
    setWindowIcon(QIcon("./images/tit.png"));
    setFixedSize(375, 175);
    move(125, 150);

    db.connect();

    init();
    createActions();
    createWidgets();

    connect(textLine, SIGNAL(textChanged(QString)),
            this, SLOT(enableEnterButton(QString)));
    connect(textLine, SIGNAL(returnPressed()),
            this, SLOT(validate()));
    connect(enter, SIGNAL(clicked()),
            this, SLOT(validate()));

    disposeWindow();
}



/*********************************
Prog initialization
*********************************/
void MainWindow::init() {
    selectedMode = true;
    titleMode = "Anglais -> Français";
    word = randWord(db);
}



/********************************
Create all actions
********************************/
void MainWindow::createActions() {
    englishFrench = new QAction (tr("&Anglais->Français"), this);
    englishFrench->setIcon(QIcon("./images/eng.png"));
    englishFrench->setShortcut(tr("Ctrl+A"));
    englishFrench->setStatusTip(tr("Traduire des mots anglais en Français"));
    connect(englishFrench, SIGNAL(triggered()),
            this, SLOT(setEF()));

    frenchEnglish = new QAction (tr("&Français->Anglais"), this);
    frenchEnglish->setIcon(QIcon("./images/fre.png"));
    frenchEnglish->setShortcut(tr("Ctrl+F"));
    frenchEnglish->setStatusTip(tr("Traduire des mots Français en anglais"));
    connect(frenchEnglish, SIGNAL(triggered()),
            this, SLOT(setFE()));

    editDb = new QAction (tr("&Ajouter des mots"), this);
    editDb->setIcon(QIcon("./images/edb.png"));
    editDb->setShortcut(tr("Ctrl+R"));
    editDb->setStatusTip(tr("Ajouter des mots au dictionnaire"));
    connect(editDb, SIGNAL(triggered()),
            this, SLOT(showDbEditor()));

    viewDb = new QAction (tr("&Voir le dictionnaire"), this);
    viewDb->setIcon(QIcon("./images/vdb.png"));
    viewDb->setShortcut(tr("Ctrl+V"));
    viewDb->setStatusTip(tr("Voir le dictionnaire complet"));
    connect(viewDb, SIGNAL(triggered()),
            this, SLOT(showDbViewer()));

    quit = new QAction (tr("&Quitter"), this);
    quit->setIcon(QIcon("./images/qui.png"));
    quit->setShortcut(tr("Ctrl+Q"));
    quit->setStatusTip(tr("Quitter le programme"));
    connect(quit, SIGNAL(triggered()),
            qApp, SLOT(quit()));

    helpAction = new QAction (tr("&Aide"), this);
    helpAction->setIcon(QIcon("./images/hel.png"));
    helpAction->setStatusTip(tr("Aide du programme"));
    connect(helpAction, SIGNAL(triggered()),
            this, SLOT(help()));

    searchAction = new QAction (tr("&Rechercher"), this);
    searchAction->setIcon(QIcon("./images/sea.png"));
    searchAction->setStatusTip(tr("Aide du programme"));
    connect(searchAction, SIGNAL(triggered()),
            this, SLOT(search()));

    aboutAction = new QAction (tr("A &propos"), this);
    aboutAction->setIcon(QIcon("./images/abo.png"));
    aboutAction->setStatusTip(tr("A propos de ce programme"));
    connect(aboutAction, SIGNAL(triggered()),
            this, SLOT(about()));

    aboutQt = new QAction (tr("A propos de &Qt"), this);
    aboutQt->setIcon(QIcon("./images/qt4.png"));
    aboutQt->setStatusTip(tr("A propos de Qt"));
    connect(aboutQt, SIGNAL(triggered()),
            qApp, SLOT(aboutQt()));
}



/********************************
Create the elements of the window
********************************/
void MainWindow::createWidgets() {
    image = new QLabel(this);
    image->setPixmap(QPixmap::fromImage(QImage("./images/log.png")));

    enter = new QPushButton(tr("&Valider"), this);
    enter->setEnabled(false);

    toFindLabel = new QLabel(tr("Traduire : "), this);
    toFind = new QLabel(word, this);

    textLineLabel = new QLabel(tr("Réponse : "), this);
    textLine = new QLineEdit(this);

    dbed = new DbEditor(this, this);
    dbv = new DbViewer(this, this);

    fileMenu = menuBar()->addMenu(tr("&Fichier"));
    fileMenu->addAction(englishFrench);
    fileMenu->addAction(frenchEnglish);
    fileMenu->addSeparator();
    fileMenu->addAction(quit);

    editMenu = menuBar()->addMenu(tr("&Edition"));
    editMenu->addAction(viewDb);
    editMenu->addAction(editDb);
    editMenu->addSeparator();
    editMenu->addAction(searchAction);

    helpMenu = menuBar()->addMenu(tr("&Aide"));
    helpMenu->addAction(helpAction);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQt);
}



/*********************************
Arrange the elements of the window
*********************************/
void MainWindow::disposeWindow() {
    window = new QWidget;
    groupBox = new QGroupBox(titleMode);
    setCentralWidget(window);

    h1 = new QHBoxLayout;
    h1->addWidget(textLineLabel);
    h1->addWidget(textLine);

    h2 = new QHBoxLayout;
    h2->addWidget(toFindLabel);
    h2->addWidget(toFind);


    v1 = new QVBoxLayout;
    v1->addLayout(h2);
    v1->addLayout(h1);
    groupBox->setLayout(v1);

    v2 = new QVBoxLayout;
    v2->addWidget(groupBox);
    v2->addWidget(enter);

    h3 = new QHBoxLayout;
    h3->addWidget(image);
    h3->addLayout(v2);
    window->setLayout(h3);
}



/*********************************
Select a random word in the Database
*********************************/
QString MainWindow::randWord(DataBase userDb) {
    QString table;

    if(selectedMode) {
        table = "english";
    }
    else {
        table = "french";
    }

    QString query = "SELECT " + table + " FROM words ORDER BY RANDOM() LIMIT 1";
    return userDb.query(query, "SELECT");
}



/*********************************
Seprate the answers
*********************************/
void MainWindow::sepAnswers() {
    answers.clear();
    if(result.contains(",")) {
        int coma = result.count(",");
        for(int i = 0; i <= coma; i++) {
            answers.push_back(result.section(",",i,i).replace(" ", ""));
        }
    }
}



/*********************************
Verify if the answer is correct
*********************************/
void MainWindow::validate() {
    dial = new QDialog(this);
    dial->setModal(true);


    if(enter->isEnabled()) {
        if(selectedMode) {
            result = englishToFrench(word, db);
        }
        else {
            result = frenchToEnglish(word, db);
        }

        sepAnswers();

        if(textLine->text() == result) {
            dial->setWindowTitle(tr("Correct !"));
            QLabel *lab = new QLabel(tr("Correct !"), dial);
            lab->setAlignment(Qt::AlignCenter);
            QPushButton *but = new QPushButton(tr("Continuer"), dial);

            QVBoxLayout *v = new QVBoxLayout();
            v->addWidget(lab);
            v->addWidget(but);
            dial->setLayout(v);

            dial->setFixedSize(100, 75);
            dial->show();

            connect(but, SIGNAL(clicked()),
                    this, SLOT(delDial()));

            word = randWord(db);
            toFind->setText(word);
        }
        else {
            dial->setWindowTitle(tr("Faux !"));
            QLabel *lab = new QLabel(tr("! FAUX !"), dial);
            lab->setAlignment(Qt::AlignCenter);
            QPushButton *butAgain = new QPushButton(tr("Recommencer"), dial);
            QPushButton *butSol = new QPushButton(tr("Réponse"), dial);
            butAgain->setFocus();

            QHBoxLayout *h = new QHBoxLayout();
            h->addWidget(butAgain);
            h->addWidget(butSol);

            QVBoxLayout *v = new QVBoxLayout();
            v->addWidget(lab);
            v->addLayout(h);
            dial->setLayout(v);

            dial->setFixedSize(200, 75);
            dial->show();

            connect(butAgain, SIGNAL(clicked()),
                    this, SLOT(delDial()));
            connect(butSol, SIGNAL(clicked()),
                    this, SLOT(showAnswer()));
        }
    }
}



/*********************************
Search translation of a french word
*********************************/
QString MainWindow::frenchToEnglish(QString userWord, DataBase userDb) {
    QSqlQuery query;
    query.prepare("SELECT english FROM words WHERE french = ?");
    query.addBindValue(userWord.replace("'", "''"));
    query.exec();

    while(query.next()) {
        return query.value(0).toString();
    }
}



/*********************************
Search translation of an english word
*********************************/
QString MainWindow::englishToFrench(QString userWord, DataBase userDb) {
    QSqlQuery query;
    query.prepare("SELECT french FROM words WHERE english = ?");
    query.addBindValue(userWord.replace("'", "''"));
    query.exec();

    while(query.next()) {
        return query.value(0).toString();
    }
}



/*********************************
Show the answer
*********************************/
void MainWindow::showAnswer() {
    delDial();

    dial = new QDialog(this);
    dial->setWindowTitle(tr("Réponse"));
    dial->setModal(true);

    QLabel *lab = new QLabel(result, dial);
    lab->setAlignment(Qt::AlignCenter);
    QPushButton *but = new QPushButton(tr("Continuer"), dial);

    QVBoxLayout *v = new QVBoxLayout();
    v->addWidget(lab);
    v->addWidget(but);
    dial->setLayout(v);

    dial->setFixedSize(200, 75);
    dial->show();

    word = randWord(db);
    toFind->setText(word);

    connect(but, SIGNAL(clicked()),
            this, SLOT(delDial()));
}



/*********************************
delete validation dialog box
*********************************/
void MainWindow::delDial() {
    delete(dial);
    textLine->clear();
    enter->setEnabled(false);
}


/*********************************
Mode choice
*********************************/
void MainWindow::setEF() {
    selectedMode = true;
    titleMode = "Anglais -> Français";
    word = randWord(db);
    toFind->setText(word);
    groupBox->setTitle(titleMode);
}



void MainWindow::setFE() {
    selectedMode = false;
    titleMode = "Français -> Anglais";
    word = randWord(db);
    toFind->setText(word);
    groupBox->setTitle(titleMode);
}



/*********************************
Slot about
*********************************/
void MainWindow::about() {
    QMessageBox::about(this, tr("A propos de WorkYourEnglish"),
                       tr("<h2> WorkYourEnglish 0.3</h2>"
                          "<p> WorkYourEnglish est un logiciel réalisé avec Qt/c++/SQLite permettant de travailler son vocabulaire Anglais/Français. </p>"));
}



/*********************************
Enable the validation button
*********************************/
void MainWindow::enableEnterButton(QString text) {
    enter->setEnabled(true);
}



/*********************************
Enable the search button
*********************************/
void MainWindow::enableSearchButton(QString text) {
    searchButton->setEnabled(true);
}



/*********************************
Return the used Database
*********************************/
DataBase MainWindow::getDb() {
    return db;
}



/*********************************
Return the DatabaseViewer
*********************************/
DbViewer* MainWindow::getDbv() {
    return dbv;
}



/*********************************
Show the Database Editor
*********************************/
void MainWindow::showDbEditor() {
    dbed->show();
}



/*********************************
Hide the Database Editor
*********************************/
void MainWindow::hideDbEditor() {
    dbed->hide();
}



/*********************************
Show the Database Viewer
*********************************/
void MainWindow::showDbViewer() {
    dbv->fillTable();
    dbv->show();
}



/*********************************
Hide the Database Viewer
*********************************/
void MainWindow::hideDbViewer() {
    dbv->hide();
}



/*********************************
Bring the help browser
*********************************/
void MainWindow::help() {
    HelpWindow::showPage("index.html", this);
}



/*********************************
Bring the search dialog
*********************************/
void MainWindow::search() {
    searchDial = new QDialog(this);
    searchDial->setWindowTitle(tr("Rechercher"));
    searchDial->setFixedSize(200, 150);
    searchDial->setModal(true);

    QRadioButton *searchAF = new QRadioButton(tr("&Anglais->Français"), searchDial);
    QRadioButton *searchFA = new QRadioButton(tr("&Français->Anglais"), searchDial);

    QButtonGroup *buttonGroup = new QButtonGroup(this);
    buttonGroup->addButton(searchFA, 1);
    buttonGroup->addButton(searchAF, 2);

    searchAF->setChecked(true);
    manageRB(2);

    QLabel *searchLab = new QLabel(tr("Mot :"), searchDial);
    searchText = new QLineEdit(searchDial);
    searchButton = new QPushButton(tr("&Go!"), searchDial);
    searchButton->setEnabled(false);
    searchButton->setDefault(true);
    QPushButton *searchQuit = new QPushButton(tr("&Retour"), searchDial);

    QGroupBox *searchGroupBox = new QGroupBox(tr("Rechercher"));
    QHBoxLayout *hs = new QHBoxLayout();
    hs->addWidget(searchLab);
    hs->addWidget(searchText);

    QVBoxLayout *vs = new QVBoxLayout();
    vs->addWidget(searchFA);
    vs->addWidget(searchAF);
    vs->addLayout(hs);
    searchGroupBox->setLayout(vs);

    QHBoxLayout *hs1 = new QHBoxLayout();
    hs1->addWidget(searchButton);
    hs1->addWidget(searchQuit);

    QVBoxLayout *vs1 = new QVBoxLayout();
    vs1->addWidget(searchGroupBox);
    vs1->addLayout(hs1);
    searchDial->setLayout(vs1);

    searchDial->show();

    connect(searchButton, SIGNAL(clicked()),
            this, SLOT(doSearch()));
    connect(searchQuit, SIGNAL(clicked()),
            this, SLOT(delSearch()));
    connect(searchText, SIGNAL(textChanged(QString)),
            this, SLOT(enableSearchButton(QString)));
    connect(buttonGroup, SIGNAL(buttonClicked(int)),
            this, SLOT(manageRB(int)));
}



/*********************************
Delete search dialog box
*********************************/
void MainWindow::delSearch() {
    delete(searchDial);
}



/*********************************
Delete search dialog box
*********************************/
void MainWindow::delResultDial() {
    delete(resultDial);
}



/*********************************
Manage Radio buttons
*********************************/
void MainWindow::manageRB(int id) {
    searchMode = id;
}



/*********************************
Do the research
*********************************/
void MainWindow::doSearch() {
    QString query, searched, toSearch, ret;

    if(searchMode == 1) {
        searched = "french";
        toSearch = "english";
    }
    if(searchMode == 2) {
        searched = "english";
        toSearch = "french";
    }

    query = "SELECT COUNT (*) FROM french";
    int nbC = db.query(query, "SELECT").toInt();
    QList<QString> questList, ansList;
    QList<int> ids;

    //Select all the words corresponding to the user's research, and put the IDs in the ids list
    for(int i = 0; i<nbC; i++) {
        query = "SELECT word FROM " + searched + " WHERE id = " + QString::number(i + 1);
        ret = db.query(query, "SELECT");
        questList = ret.split(QRegExp(", *"), QString::SkipEmptyParts);
        for(int j = 0; j<questList.size(); j++) {
            if(questList.at(j) == searchText->text()) {
                ids << (i + 1);
            }
        }
    }

    //Select all the translations of the word asked
    for(int i = 0; i<ids.size(); i++) {
        query = "SELECT word FROM " + toSearch + " WHERE id = " + QString::number(ids.at(i));
        ret = db.query(query, "SELECT");
        ansList << ret;
    }

    showSearchResult(searchText->text(), ansList);
}



/*********************************
Do the research
*********************************/
void MainWindow::showSearchResult(QString quest, QList<QString> result) {
    resultDial = new QDialog(searchDial);
    resultDial->setWindowTitle(tr("Résultat de recherche"));
    resultDial->setModal(true);

    if(!result.isEmpty()) {
        QString qs = "<b>" + quest + "</b>";
        QLabel *resultLab = new QLabel(qs, resultDial);

        QList<QLabel *> ansList;
        for(int i = 0; i<result.size(); i++) {
            ansList << new QLabel("-> " + result.at(i) + "\t", resultDial);
        }

        QPushButton *but = new QPushButton(tr("&Continuer"), resultDial);
        but->setFocus();

        QGridLayout *v = new QGridLayout;
        v->addWidget(resultLab, 0, 0);
        for(int i = 0; i<ansList.size(); i++) {
            v->addWidget(ansList.at(i), i, 1);
        }
        v->addWidget(but, ansList.size(), 0, 1, 2, Qt::AlignCenter);
        resultDial->setLayout(v);

        resultDial->show();

        connect(but, SIGNAL(clicked()),
                this, SLOT(delResultDial()));
    }
    else {
        QLabel *lab = new QLabel(tr("Ce mot n'est pas dans le dictionaire."), resultDial);
        lab->setAlignment(Qt::AlignCenter);
        QPushButton *but = new QPushButton(tr("&Ok"), resultDial);
        but->setFocus();

        QVBoxLayout *v = new QVBoxLayout;
        v->addWidget(lab);
        v->addWidget(but);
        resultDial->setLayout(v);

        resultDial->setFixedSize(250, 75);
        resultDial->show();

        connect(but, SIGNAL(clicked()),
                this, SLOT(delResultDial()));
    }
}



MainWindow::~MainWindow() {}
