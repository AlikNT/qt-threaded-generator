#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "worker.h"
#include <QDebug>
#include <QTime>
#include <QDateTime>
#include <QTableWidgetItem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(ui->btnStart, &QPushButton::clicked, this, &MainWindow::onStartClicked);
    connect(ui->btnStop, &QPushButton::clicked, this, &MainWindow::onStopClicked);
    ui->tableResults->setColumnCount(4);
    ui->tableResults->setHorizontalHeaderLabels({"Время", "Поток", "Число", "Причина"});
}

MainWindow::~MainWindow()
{
    clearThreads();
    delete ui;
}

void MainWindow::onStartClicked()
{
    if (running_) {
        qDebug() << "Уже выполняется!";
        return;
    }

    disconnect();
    clearThreads();

    stopping_ = false;
    iterationCounter_ = 0;
    running_ = true;

    int target = ui->spinTarget->value();
    int threadCount = ui->spinThreads->value();
    int maxIterations = ui->spinIterations->value();
    int quantum = ui->spinQuantum->value();

    ui->tableResults->setRowCount(0);

    for (int i = 0; i < threadCount; ++i) {
        QThread* thread = new QThread;
        Worker* worker = new Worker(i + 1, target, quantum, iterationCounter_, maxIterations);

        worker->moveToThread(thread);
        connect(thread, &QThread::started, worker, &Worker::process);
        connect(worker, &Worker::dataReady, this, &MainWindow::onDataReady);
        connect(worker, &Worker::finished, this, &MainWindow::onThreadFinished);

        threads_.push_back(thread);
        workers_.push_back(worker);

        thread->start();
    }
}

void MainWindow::onStopClicked()
{
    stopping_ = true;
    running_ = false;
    for (Worker* w : workers_) {
        if (w) w->stop();
    }
}

void MainWindow::onDataReady(const QString& time, int threadId, int value, const QString& reason)
{
    int row = ui->tableResults->rowCount();
    ui->tableResults->insertRow(row);
    ui->tableResults->setItem(row, 0, new QTableWidgetItem(time));
    ui->tableResults->setItem(row, 1, new QTableWidgetItem(QString::number(threadId)));
    ui->tableResults->setItem(row, 2, new QTableWidgetItem(QString::number(value)));
    ui->tableResults->setItem(row, 3, new QTableWidgetItem(reason));
}

void MainWindow::onThreadFinished()
{
    running_ = false;
}

void MainWindow::clearThreads()
{
    for (size_t i = 0; i < threads_.size(); ++i) {
        Worker* w = workers_[i];
        QThread* t = threads_[i];

        if (w) w->stop();
        if (t) {
            t->quit();
            t->wait();
        }

        delete w;
        delete t;
    }
    workers_.clear();
    threads_.clear();
}
