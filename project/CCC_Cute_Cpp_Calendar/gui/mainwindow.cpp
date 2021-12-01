#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    client = new Client();
    dialog = NULL;
    eventDialog = NULL;
    _calBoxesLayout = new QVBoxLayout();
    _calBoxesLayout->setAlignment(Qt::AlignTop);
    _calBoxes = new QWidget(ui->scrollArea);
}

MainWindow::~MainWindow()
{
    delete _calBoxesLayout;
    delete _calBoxes;
    delete ui;
    if(dialog != NULL)
        delete dialog;
    if(eventDialog != NULL)
        delete eventDialog;
    delete client;
}


void MainWindow::on_actionApri_calendario_triggered()
{
    if(dialog == NULL) {
        dialog = new Dialog();
        connect(dialog, &Dialog::eventAddCalendar, client, &Client::handleAddCalendar);
        connect(client, &Client::dialogErrorMessage, dialog, &Dialog::handleResponse);
        connect(client, &Client::closeDialog, this, &MainWindow::handleCloseDialog);
    }

    dialog->setModal("true");
    dialog->show();
}

void MainWindow::handleCloseDialog(Calendar* cal) {
    dialog->hide();
    disconnect(dialog, &Dialog::eventAddCalendar, client, &Client::handleAddCalendar);
    disconnect(client, &Client::dialogErrorMessage, dialog, &Dialog::handleResponse);
    disconnect(client, &Client::closeDialog, this, &MainWindow::handleCloseDialog);

    //o si fanno le disconnect perché si vuole cancellare il dialog
    //oppure si mantiene sempre lo stesso dialog ma a quel punto vanno puliti i campi

    delete dialog;
    dialog = NULL;

    /**
     * In this phase there will be the creation of the box to be shown on the UI
     *
     **/

    // The first line will show the name of the calendar
    QHBoxLayout* firstLine = new QHBoxLayout();
    QLabel* name = new QLabel(cal->displayName());
    name->setStyleSheet("font-weight: bold;");
    firstLine->addWidget(name);

    // The second line will show the URL of the calendar
    QHBoxLayout* secondLine = new QHBoxLayout();
    QLabel* defaultUrl = new QLabel(QString("URL:"));
    secondLine->addWidget(defaultUrl);
    QLabel* urlCal = new QLabel(cal->url());
    secondLine->addWidget(urlCal);

    // The third line will show the button to remove the calendar
    QHBoxLayout* thirdLine = new QHBoxLayout();
    QPushButton* removeButton = new QPushButton("Rimuovi", this);
    //Al click del tasto remove:
    //Segnalo al calendario che lo sto rimuovendo: così lui potrà lanciare una signal  contenente anche il proprio riferimento
    connect(removeButton, SIGNAL(clicked()), cal, SLOT(handleRemoveCalendar()));
    //La mainwindow aggiorna la lista di layout
    connect(cal, &Calendar::removeCalendar, this, &MainWindow::handleRemoveCalendarBox);
    //Il client aggiorna la lista di calendari
    connect(cal, &Calendar::removeCalendar, client, &Client::handleRemoveCalendarFromList);

    thirdLine->addWidget(removeButton);

    // Build now the whole box
    QVBoxLayout* fullBox = new QVBoxLayout();
    fullBox->setAlignment(Qt::AlignLeft);
    fullBox->addLayout(firstLine);
    fullBox->addLayout(secondLine);
    fullBox->addLayout(thirdLine);

    // Add the new box to the map
    _boxesMap.insert(cal->url(),fullBox);

    // Refresh
//    QVBoxLayout* calBoxesLayout = new QVBoxLayout(_calBoxes);
//    calBoxesLayout->setAlignment(Qt::AlignTop);

//    QMap<QString,QVBoxLayout*>::iterator it;
//    for (it = _boxesMap.begin(); it != _boxesMap.end(); it++) {
//        calBoxesLayout->addLayout(it.value());
//        qDebug() << "Itero";
//    }

    //un fullBox (che è anch'esso una collezione di layout) per ogni calendario
    _calBoxesLayout->addLayout(fullBox);

    // Show the whole list to the UI
    _calBoxes->setLayout(_calBoxesLayout);
    ui->scrollArea->setWidget(_calBoxes);
}

void MainWindow::on_scrollArea_customContextMenuRequested(const QPoint &pos)
{

}


void MainWindow::on_calendarWidget_clicked(const QDate &date)
{
    //TODO: User selected a date, show Events and ToDo for that date
}


void MainWindow::on_pushButton_clicked()
{
    if(eventDialog == NULL) {
        eventDialog = new EventDialog();
        connect(eventDialog, &EventDialog::eventAddEvent, client, &Client::handleAddEvent);
        connect(client, &Client::eventDialogErrorMessage, eventDialog, &EventDialog::handleEventResponse);
        connect(client, &Client::closeEventDialog, this, &MainWindow::handleCloseEventDialog);
    }

    eventDialog->setModal("true");
    eventDialog->show();
}

//TODO: implement this
void MainWindow::handleCloseEventDialog(Event* event) {

}

void MainWindow::handleRemoveCalendarBox(Calendar* cal) {

    //rimuovo una entry cal dalla mappa di boxes
    _boxesMap.remove(cal->url());
    //nascondo il contenuto della scrollbar
    ui->scrollArea->widget()->hide();
    //

    for(int i =0; i<_calBoxesLayout->count(); i++){
        QWidget *url = _calBoxesLayout->itemAt(i)->layout()->itemAt(1)->layout()->itemAt(1)->widget();
        QLabel *label = qobject_cast<QLabel *>(url);
        qDebug() << "Url:" << label->text();
        if(cal->url() == label->text()){
            QLayoutItem * tobeRemoved = _calBoxesLayout->itemAt(i)->layout();
            _calBoxesLayout->removeItem(tobeRemoved);
            delete tobeRemoved;
        }
    }

    // Show the whole list to the UI
    QWidget* mybox = new QWidget();
    mybox->setLayout(_calBoxesLayout);
    delete _calBoxes;
    _calBoxes = mybox;
    ui->scrollArea->setWidget(mybox);

    //mostro di nuovo il contenuto della scrollbar
    ui->scrollArea->widget()->show();
}



void MainWindow::on_pushButton_2_clicked()
{
    ui->scrollArea->widget()->hide();
    delete ui->scrollArea->widget();
}

