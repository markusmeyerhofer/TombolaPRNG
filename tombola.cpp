#include "tombola.h"
#include "ui_tombola.h"
#include <QDebug>
#include <QtGui>
#include <chrono>
using namespace std::chrono;

Tombola::Tombola(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {

    ui->setupUi(this);
    textColor = ui->timeLCD->palette().color(QPalette::WindowText);
    backgroundColor = ui->resultLCD->palette().color(QPalette::Window);

    ui->timeLCD->display(ui->timeSB->value());

    prngThread = new QThread(this);
    prng = new PRNG();
    prng->moveToThread(prngThread);
    prngThread->start();

    connect(ui->startStopButton, SIGNAL(clicked()), this, SLOT(startStop()));
    connect(prng, &PRNG::updateGUI, this, &Tombola::setGUIValues);
    connect(prng, &PRNG::startedPRNG, this, &Tombola::generationHasStarted);
    connect(prng, &PRNG::stoppedPRNG, this, &Tombola::generationHasFinished);
    connect(prng, &PRNG::lotExhaustedError, this, &Tombola::lotExhaustedError);
    connect(this, &Tombola::createRandomNumber, prng, &PRNG::createRandomNumber);
    connect(this, &Tombola::setRange, prng, &PRNG::setRange);
    connect(this, &Tombola::shutDownGracefully, prng, &PRNG::timerDone);
    connect(this, &Tombola::reset, prng, &PRNG::reset);
}

Tombola::~Tombola() {

    emit shutDownGracefully();
    prngThread->quit();
    prngThread->wait();
    prngThread->deleteLater();
    delete prng;

    delete ui;
}

void Tombola::generationHasStarted() {
    ui->startStopButton->setText("Stop");
}

void Tombola::generationHasFinished() {
     ui->startStopButton->setText("Start");
}

void Tombola::startStop() {

    emit setRange(ui->rangeSB->value());

    if (ui->startStopButton->text() == "Start") {
        ui->startStopButton->setText("Stop");
        emit createRandomNumber(ui->timeSB->value());
    } else {
        ui->startStopButton->setText("Start");
        emit shutDownGracefully();
    }
}

void Tombola::setGUIValues(int currentNumber, int remainingTime, bool finished, QString lots) {

    if (finished) {

        QPalette palette = ui->resultLCD->palette();
        palette.setColor(QPalette::WindowText, Qt::red);
        ui->resultLCD->setPalette(palette);
        ui->resultLCD->display(currentNumber);
        ui->awardedLotsTF->setText(lots);

    } else {
        QPalette palette = ui->resultLCD->palette();
        palette.setColor(QPalette::WindowText, textColor);
        ui->resultLCD->setPalette(palette);
        ui->resultLCD->display(currentNumber);
    }

    if (remainingTime < 4 ) {
        QPalette palette = ui->timeLCD->palette();

        if (finished) {
            palette.setColor(QPalette::WindowText, textColor);
        } else {
            palette.setColor(QPalette::WindowText, Qt::red);
        }

        ui->timeLCD->setPalette(palette);
    } else {
        QPalette palette = ui->timeLCD->palette();
        palette.setColor(QPalette::WindowText, textColor);
        ui->timeLCD->setPalette(palette);
    }

    ui->timeLCD->display(remainingTime);

}

void Tombola::lotExhaustedError() {

    ui->startStopButton->setText("Start");
    emit shutDownGracefully();
    ui->errorLabel->setText("<font color='red'>Error: Alle Lose vergeben!</font>");

}

void Tombola::on_timeSB_valueChanged() {
    ui->timeLCD->display(ui->timeSB->value());
}

void Tombola::on_eventTF_textChanged() {
     ui->eventTextLabel->setText(ui->eventTF->toPlainText());
}


void Tombola::on_rangeSB_valueChanged() {
    resetLots();
    emit setRange(ui->rangeSB->value());
}


void Tombola::on_resetLots_clicked() {
    resetLots();
}

void Tombola::resetLots() {
    emit reset();
    ui->awardedLotsTF->setText("");
    ui->errorLabel->setText("");
}
