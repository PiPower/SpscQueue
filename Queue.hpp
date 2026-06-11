#pragma once
#include <deque>
#include <condition_variable>
#include "queue_test.hpp"

template<typename T>
struct SpscQueue
{
    std::deque<T> queue;
    size_t maxSize;
    std::mutex access;
    std::condition_variable emptyVar;
    std::condition_variable fullVar;

    SpscQueue(size_t maxSize)
    :
    maxSize(maxSize)
    {
        queue.resize(maxSize);
        queue.clear();
    }
};

template<typename T>
void produce(std::shared_ptr<SpscQueue<T>> queue, const T& data)
{
    std::unique_lock lock(queue->access);
    while (queue->queue.size() >= queue->maxSize)
    {
        queue->fullVar.wait(lock);
    }
    queue->queue.push_back(data);

    queue->emptyVar.notify_one();
}

template <typename T>
T consume(std::shared_ptr<SpscQueue<T>> queue)
{
    std::unique_lock lock(queue->access);
    while (queue->queue.size() == 0)
    {
        queue->emptyVar.wait(lock);
    }
    T data =  queue->queue.front();
    queue->queue.pop_front();
    
    queue->fullVar.notify_one();

    return data;
}
