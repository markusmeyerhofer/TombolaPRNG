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

private slots:
    void startStop();
    void setGUIValues(int, int, bool);
    void generationHasStarted();
    void generationHasFinished();
    void on_timeSB_valueChanged();
    void on_yearSpinBox_valueChanged();

private:
    Ui::MainWindow *ui;
    int min;
    int max;

signals:
    void createRandomNumber(int, int);
    void shutDownGracefully();

};
#endif // TOMBOLA_H
