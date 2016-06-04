#include "dbviewer.h"



DbViewer::DbViewer(QWidget *parent, MainWindow *main) : QDialog(parent) {
    setWindowTitle(tr("Dictionnaire"));
    setFixedSize(425, 500);
    move(525, 150);

    mainWindow = main;

    createWidgets();

    connect(quit, SIGNAL(clicked()),
            mainWindow, SLOT(hideDbViewer()));
    connect(remove, SIGNAL(clicked()),
            this, SLOT(removeWord()));
    connect(add, SIGNAL(clicked()),
            main, SLOT(showDbEditor()));
    connect(search, SIGNAL(clicked()),
            main, SLOT(search()));

    disposeWidget();
}



/********************************
Create the elements of the dialogBox
********************************/
void DbViewer::createWidgets() {
    table = new QTableWidget(0, 2, this);
    table->setSelectionBehavior(QAbstractItemView::SelectRows);

    quit = new QPushButton(tr("&Quitter"), this);

    search = new QPushButton(tr("R&echercher"), this);

    remove = new QPushButton(tr("&Suprimer"), this);
    remove->setToolTip(tr("Suprimer le mot sélectionné du dictionnaire"));

    add = new QPushButton(tr("&Ajouter"), this);
    add->setToolTip(tr("Ajouter un mot au dictionnaire"));
}



/*********************************
Arrange the elements of the dialogBox
*********************************/
void DbViewer::disposeWidget() {
    groupBox = new QGroupBox("Dictionnaire");

    h = new QHBoxLayout;
    h->addWidget(table);
    groupBox->setLayout(h);

    h1 = new QHBoxLayout;
    h1->addWidget(add);
    h1->addWidget(remove);
    h1->addWidget(search);
    h1->addStretch();
    h1->addWidget(quit);

    v = new QVBoxLayout;
    v->addWidget(groupBox);
    v->addLayout(h1);
    this->setLayout(v);
}



void DbViewer::fillTable() {
    emptyTable();

    QStringList list = (QStringList() << "Français" << "Anglais");
    table->setHorizontalHeaderLabels(list);

    int64_t i(0);
    QSqlQuery query("SELECT french, english FROM words");
    while(query.next()) {

        table->insertRow(i);

        QTableWidgetItem *temp0 = new QTableWidgetItem;
        temp0->setFlags(temp0->flags() & ~Qt::ItemIsEditable);
        temp0->setBackground(Qt::green);
        table->setItem(i, 0, temp0);
        QTableWidgetItem *temp1 = new QTableWidgetItem;
        temp1->setFlags(temp1->flags() & ~Qt::ItemIsEditable);
        temp1->setBackground(Qt::red);
        table->setItem(i, 1, temp1);
        table->setCurrentItem(temp0);

        table->item(i, 0)->setText(query.value(0).toString());
        table->item(i, 1)->setText(query.value(1).toString());

        i++;
    }

    table->setCurrentCell(0, 1);
}



void DbViewer::emptyTable() {
    table->clear();
    table->setRowCount(0);
}



/*********************************
Delete a word from the DB and set
the id of the other words.
*********************************/
void DbViewer::removeWord() {
    QSqlQuery query;
    query.prepare("DELETE FROM words WHERE french = ?");
    query.addBindValue(table->selectedItems().first()->text());
    query.exec();

    fillTable();
}




QTableWidget* DbViewer::getTable() {
    return table;
}



DbViewer::~DbViewer() {}
