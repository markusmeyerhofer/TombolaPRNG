#ifndef PRNG_H
#define PRNG_H

#include <QObject>
#include <QTimer>

class PRNG : public QObject
{
    Q_OBJECT
public:
    explicit PRNG(QObject *parent = nullptr);
     ~PRNG();

private:
    int randomNumber;
    int remainingTime;
    bool stopped;
    int min;
    int max;
    QTimer *timer;
    QTimer *stopTimer;
    QTimer *timeCounter;

signals:
    void updateGUI(int, int, bool);
    void startedPRNG();
    void stoppedPRNG();

public slots:
    void createRandomNumber(int, int);
    void runPRNG();
    void timerDone();
    void decreaseTime();

};

#endif // PRNG_H
