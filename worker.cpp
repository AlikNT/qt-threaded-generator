#include "worker.h"
#include <QTime>
#include <QDateTime>

Worker::Worker(int threadId, int target, int quantum, std::atomic<int>& counter, int maxIterations)
    : threadId_(threadId)
    , target_(target)
    , quantum_(quantum)
    , iterationCounter_(counter)
    , maxIterations_(maxIterations)
    , stopped_(false)
{}

void Worker::stop()
{
    stopped_ = true;
}

void Worker::process()
{
    while (!stopped_) {
        // Жесткий контроль итераций
        int current = iterationCounter_.load();
        while (current < maxIterations_) {
            if (iterationCounter_.compare_exchange_weak(current, current + 1)) {
                break; // увеличили успешно
            }
            // current обновляется внутри compare_exchange
        }

        if (current >= maxIterations_) {
            break; // превышен лимит — выход
        }

        int value = QRandomGenerator::global()->bounded(1, 11);
        QString reason;

        if (value == target_) {
            reason = "Совпадение";
        } else if (current + 1 >= maxIterations_) {
            reason = "Достигнут лимит";
        }

        emit dataReady(QTime::currentTime().toString("HH:mm:ss"), threadId_, value, reason);

        if (!reason.isEmpty()) {
            break;
        }

        QThread::sleep(QRandomGenerator::global()->bounded(1, quantum_ + 1));
    }

    emit finished();
}
