#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QElapsedTimer>
#include <QRandomGenerator>
#include <QThread>
#include <atomic>

class Worker : public QObject
{
    Q_OBJECT
public:
    Worker(int threadId, int target, int quantum, std::atomic<int>& counter, int maxIterations);

public slots:
    void process();
    void stop();

signals:
    void dataReady(const QString& time, int threadId, int value, const QString& reason);
    void finished();

private:
    int threadId_;
    int target_;
    int quantum_;
    std::atomic<int>& iterationCounter_;
    int maxIterations_;
    std::atomic<bool> stopped_;
};

#endif // WORKER_H
