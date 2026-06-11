#pragma once
#include <deque>
#include <condition_variable>
#include <atomic>

namespace Spsc
{

template<typename T>
struct Queue
{
    std::deque<T> queue;
    size_t maxSize;
    std::mutex access;
    std::atomic<bool> isProducerUsed;
    std::atomic<bool> isConsumerUsed;
    std::condition_variable emptyVar;
    std::condition_variable fullVar;

    Queue(size_t maxSize)
    :
    maxSize(maxSize)
    {
        queue.resize(maxSize);
        queue.clear();
        isProducerUsed.store(false, std::memory_order_relaxed);
        isConsumerUsed.store(false, std::memory_order_relaxed);
    }
};

template<typename T>
bool registerProducer(Queue<T>* q)
{
    bool expected = false;
    return q->isProducerUsed.compare_exchange_strong(expected, true);
}

template<typename T>
bool unregisterProducer(Queue<T>* q)
{
    bool expected = true;
    return q->isProducerUsed.compare_exchange_strong(expected, false);
}

template<typename T>
bool registerConsumer(Queue<T>* q)
{
    bool expected = false;
    return q->isConsumerUsed.compare_exchange_strong(expected, true);
}

template<typename T>
bool unregisterConsumer(Queue<T>* q)
{
    bool expected = true;
    return q->isConsumerUsed.compare_exchange_strong(expected, false);
}

template<typename T>
void produce(Queue<T>* q, const T& data)
{
    std::unique_lock lock(q->access);
    while (q->queue.size() >= q->maxSize)
    {
        q->fullVar.wait(lock);
    }
    q->queue.push_back(data);

    q->emptyVar.notify_one();
}

template<typename T>
T consume(Queue<T>* q)
{
    std::unique_lock lock(q->access);
    while (q->queue.size() == 0)
    {
        q->emptyVar.wait(lock);
    }
    T data =  q->queue.front();
    q->queue.pop_front();
    
    q->fullVar.notify_one();

    return data;
}

} // Spsc namespace end