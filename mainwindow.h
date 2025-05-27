#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QThread>
#include <QTime>
#include <QVector>
#include <QTableWidgetItem>
#include <atomic>

#include "worker.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void onDataReady(const QString& time, int threadId, int value, const QString& reason);
    void onThreadFinished();
    void onStartClicked();
    void onStopClicked();

private:
    Ui::MainWindow *ui;

    std::vector<QThread*> threads_;
    std::vector<Worker*> workers_;
    std::atomic<int> iterationCounter_;
    bool stopping_ = false;
    bool running_ = false;

    void clearThreads();
};
#endif // MAINWINDOW_H
