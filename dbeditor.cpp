#include "dbeditor.h"





DbEditor::DbEditor(QWidget *parent, MainWindow *main) : QDialog(parent){
    setWindowTitle(tr("Ajout de mots"));
    setFixedSize(250, 150);
    mainWindow = main;
    createWidgets();

    connect(add, SIGNAL(clicked()),
            this, SLOT(addDb()));
    connect(quit, SIGNAL(clicked()),
            mainWindow, SLOT(hideDbEditor()));

    disposeWidget();
}



/*********************************
Create the elements of the dialogBox
*********************************/
void DbEditor::createWidgets() {
    add = new QPushButton(tr("&Ajouter"), this);
    quit = new QPushButton(tr("&Retour"), this);

    frenchLabel = new QLabel(tr("Français : "), this);
    french = new QLineEdit(this);

    englishLabel = new QLabel(tr("Anglais : "), this);
    english = new QLineEdit(this);
}



/*********************************
Arrange the elements of the dialogBox
*********************************/
void DbEditor::disposeWidget() {
    groupBox = new QGroupBox("Ajouter un mot");

    v1 = new QVBoxLayout;
    v1->addWidget(frenchLabel);
    v1->addWidget(englishLabel);

    v2 = new QVBoxLayout;
    v2->addWidget(french);
    v2->addWidget(english);

    h1 = new QHBoxLayout;
    h1->addLayout(v1);
    h1->addLayout(v2);
    groupBox->setLayout(h1);

    h2 = new QHBoxLayout;
    h2->addWidget(add);
    h2->addWidget(quit);

    v3 = new QVBoxLayout;
    v3->addWidget(groupBox);
    v3->addLayout(h2);
    this->setLayout(v3);
}



/*********************************
Add a word to the DB
*********************************/
void DbEditor::addDb() {
    QSqlQuery query;
    query.prepare("INSERT INTO words (french, english) VALUES (?, ?)");
    query.addBindValue(french->text().replace("'", "''"));
    query.addBindValue(english->text().replace("'", "''"));
    query.exec();

    QString mess = "le mot \"" + french->text() + "\" -> \"" + english->text() + "\" a bien été ajouté dans le dictionnaire";
    int r = QMessageBox::information(this, tr("Ajout réussi"), mess,
                                     QMessageBox::Ok | QMessageBox::Default);

     french->clear();
     english->clear();
     mainWindow->getDbv()->fillTable();
}



DbEditor::~DbEditor() {}
