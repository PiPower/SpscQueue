#pragma once
#include <deque>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <atomic>

namespace Spsc
{

template<typename T>
struct Queue
{
    std::deque<T> queue;
    size_t maxSize;
    std::mutex access;
    std::thread::id producerId;
    std::thread::id consumerId;
    bool shutdown;
    std::condition_variable emptyVar;
    std::condition_variable fullVar;

    Queue(size_t maxSize)
    :
    maxSize(maxSize > 0 ? maxSize : 1)
    {
        // probe preallocation
        queue.resize(maxSize);
        shutdown = false;
        queue.clear();
        producerId = {};
        consumerId = {};
    }
};

template<typename T>
bool registerProducer(Queue<T>* q)
{
    std::lock_guard lock(q->access);
    std::thread::id empty = {};
    if(q->producerId != empty)
    {
        return false;
    }
    q->producerId = std::this_thread::get_id();
    return true;
}

template<typename T>
bool unregisterProducer(Queue<T>* q)
{
    std::lock_guard  lock(q->access);
    if(q->producerId != std::this_thread::get_id())
    {
        return false;
    }
    q->producerId = {};
    return true;
}

template<typename T>
bool registerConsumer(Queue<T>* q)
{
    std::lock_guard lock(q->access);
    std::thread::id empty = {};
    if(q->consumerId != empty)
    {
        return false;
    }
    q->consumerId = std::this_thread::get_id();
    return true;
}

template<typename T>
bool unregisterConsumer(Queue<T>* q)
{
    std::lock_guard lock(q->access);
    if(q->consumerId != std::this_thread::get_id())
    {
        return false;
    }
    q->consumerId = {};
    return true;
}

template<typename T>
bool produce(Queue<T>* q, const T& data)
{
    std::unique_lock lock(q->access);
    if(q->producerId != std::this_thread::get_id())
    {
        return false;
    }

    while (q->queue.size() >= q->maxSize && !q->shutdown)
    {
        q->fullVar.wait(lock);
    }
    if(q->shutdown)
    {
        return false;
    }

    q->queue.push_back(data);
    q->emptyVar.notify_one();
    return true;
}

template<typename T>
bool consume(Queue<T>* q, T* data)
{
    std::unique_lock lock(q->access);
    if(q->consumerId != std::this_thread::get_id())
    {
        return false;
    }

    while (q->queue.size() == 0 && !q->shutdown)
    {
        q->emptyVar.wait(lock);
    }
    if(q->shutdown)
    {
        return false;
    }

    *data = std::move(q->queue.front());
    q->queue.pop_front();
    
    q->fullVar.notify_one();

    return true;
}

template<typename T>
void shutdown(Queue<T>* q)
{
    {
        std::lock_guard lock(q->access);
        q->shutdown = true;
    }
    q->emptyVar.notify_one();
    q->fullVar.notify_one();
}

} // Spsc namespace end