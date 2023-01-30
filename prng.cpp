#include "prng.h"
#include <unistd.h>
#include <QDebug>
#include <random>
using namespace std;

PRNG::PRNG(QObject *parent) : QObject{parent} {
    remainingTime = 0;
    randomNumber = 0;
    alreadyAwardedLots = new QList<int>();

    random_device rd =  random_device();
    generator = mt19937(rd());
}

PRNG::~PRNG() {
    delete timer;
    delete stopTimer;
    delete timeCounter;
    delete alreadyAwardedLots;
}

void PRNG::setRange(int max) {
    range = uniform_int_distribution(1, max);
    this->max = max;
}

void PRNG::createRandomNumber(int time) {

    if (alreadyAwardedLots->size() >= max) {
        emit lotExhaustedError();
        return;
    }

    timer = new QTimer(this);
    stopTimer = new QTimer(this);
    timeCounter = new QTimer(this);

    remainingTime = time;
    emit updateGUI(0, time, false, listToString());
    emit startedPRNG();

    connect(timer, SIGNAL(timeout()), this, SLOT(runPRNG()));
    connect(stopTimer, SIGNAL(timeout()), this, SLOT(timerDone()));
    connect(timeCounter, SIGNAL(timeout()), this, SLOT(decreaseTime()));

    timer->start(100);
    stopTimer->start(1000 * time);
    timeCounter->start(1000);
}

void PRNG::runPRNG() {

    randomNumber = range(generator);
    while (alreadyAwardedLots->contains(randomNumber)) {
        randomNumber = range(generator);
    }

    emit updateGUI(randomNumber, remainingTime, false, listToString());
}

void PRNG::timerDone() {

    timer->stop();
    stopTimer->stop();
    timeCounter->stop();
    if (alreadyAwardedLots->size() < max) alreadyAwardedLots->append(randomNumber);
    emit updateGUI(randomNumber, 0, true, listToString());
    emit stoppedPRNG();
}

void PRNG::decreaseTime() {

    remainingTime--;
}

QString PRNG::listToString() {

    QString outString;
    for(int i=0; i<alreadyAwardedLots->size(); i++)
    {
        outString += QString::number(alreadyAwardedLots->at(i));
        if(i<alreadyAwardedLots->size()-1) outString += ", " ;
    }
    return outString;
}

void PRNG::reset() {
    alreadyAwardedLots->clear();
}
