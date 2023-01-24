#include "prng.h"
#include <unistd.h>
#include <QDebug>
#include <random>
using namespace std;

PRNG::PRNG(QObject *parent) : QObject{parent} {
    min = 1;
    stopped = true;
    remainingTime = 0;
    randomNumber = 0;
}

PRNG::~PRNG() {
    delete timer;
    delete stopTimer;
    delete timeCounter;
}

void PRNG::createRandomNumber(int max, int time) {

    timer = new QTimer(this);
    stopTimer = new QTimer(this);
    timeCounter = new QTimer(this);

    remainingTime = time;
    this->max = max;
    stopped = false;
    emit updateGUI(0, time, false);
    emit startedPRNG();

    connect(timer, SIGNAL(timeout()), this, SLOT(runPRNG()));
    connect(stopTimer, SIGNAL(timeout()), this, SLOT(timerDone()));
    connect(timeCounter, SIGNAL(timeout()), this, SLOT(decreaseTime()));

    timer->start(100);
    stopTimer->start(1000 * time);
    timeCounter->start(1000);
}

void PRNG::runPRNG() {

    random_device rd;
    mt19937 generator(rd());
    uniform_int_distribution<> range(min, max);

    randomNumber = range(generator);
    emit updateGUI(randomNumber, remainingTime, false);
}

void PRNG::timerDone() {

    timer->stop();
    stopTimer->stop();
    timeCounter->stop();
    stopped = true;
    emit updateGUI(randomNumber, 0, true);
    emit stoppedPRNG();
}

void PRNG::decreaseTime() {

    remainingTime--;
}
