#ifndef PRNG_H
#define PRNG_H

#include <QObject>
#include <QTimer>
#include <random>

class PRNG : public QObject
{
    Q_OBJECT
public:
    explicit PRNG(QObject *parent = nullptr);
     ~PRNG();

private:
    int randomNumber;
    int remainingTime;
    int max;
    QList<int> *alreadyAwardedLots;
    QTimer *timer;
    QTimer *stopTimer;
    QTimer *timeCounter;
    std::mt19937 generator;
    std::uniform_int_distribution<> range;
    QString listToString();

signals:
    void updateGUI(int, int, bool, QString);
    void startedPRNG();
    void stoppedPRNG();
    void lotExhaustedError();

public slots:
    void setRange(int);
    void createRandomNumber(int);
    void runPRNG();
    void timerDone();
    void decreaseTime();
    void reset();

};

#endif // PRNG_H
