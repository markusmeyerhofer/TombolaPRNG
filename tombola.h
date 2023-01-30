#ifndef TOMBOLA_H
#define TOMBOLA_H

#include <QMainWindow>
#include <QThread>
#include "prng.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Tombola : public QMainWindow
{
    Q_OBJECT

public:
    Tombola(QWidget *parent = nullptr);
    ~Tombola();

private:
    QThread *prngThread;
    PRNG *prng;
    QColor textColor;
    QColor backgroundColor;
    Ui::MainWindow *ui;
    int max;

private slots:
    void startStop();
    void setGUIValues(int, int, bool, QString);
    void generationHasStarted();
    void generationHasFinished();
    void resetLots();
    void lotExhaustedError();
    void on_timeSB_valueChanged();
    void on_eventTF_textChanged();
    void on_rangeSB_valueChanged();
    void on_resetLots_clicked();

signals:
    void runPRNG();
    void createRandomNumber(int);
    void setRange(int);
    void shutDownGracefully();
    void reset();

};
#endif // TOMBOLA_H
