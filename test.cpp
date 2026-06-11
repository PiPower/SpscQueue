#include <gtest/gtest.h>
#include <thread>
#include "queue_wrapper.hpp"
#include <chrono>
// Check if queue works at all
static constexpr int DataSize = 5000;
static constexpr int QueueSize = 20;
static std::atomic<int> barrier;

void ProducerTest(std::shared_ptr<Spsc::Queue<int>> queue, bool waitOnBarrier)
{
    if(waitOnBarrier)
    {
        //spinlock to make consumer and producer work overlap
        while (barrier.load(std::memory_order_relaxed) != 0) {}    
    }

    bool isRegistered = int_q_register_producer(queue.get());
    EXPECT_EQ(true, isRegistered);
    EXPECT_EQ(std::this_thread::get_id(), queue->producerId);
    // double register check
    EXPECT_EQ(false , int_q_register_producer(queue.get()));
    EXPECT_EQ(std::this_thread::get_id(), queue->producerId);

    for(int i =0; i < DataSize; i++)
    {
        if(!int_q_enqueue(queue.get(), i))
        {
            break;
        }
    }

    bool isUnregistered = int_q_unregister_producer(queue.get());
    EXPECT_EQ(true, isUnregistered);
    EXPECT_EQ(std::thread::id{}, queue->producerId);
    // double unregister check
    EXPECT_EQ(false, int_q_unregister_producer(queue.get()));
    EXPECT_EQ(std::thread::id{}, queue->producerId);
}

void ConsumerTest(std::shared_ptr<Spsc::Queue<int>> queue, bool waitOnBarrier = false)
{
    if(waitOnBarrier)
    {
        //spinlock to make consumer and producer work overlap
        while (barrier.load(std::memory_order_relaxed) != 0){}    
    }

    bool isRegistered = int_q_register_consumer(queue.get());
    EXPECT_EQ(true, isRegistered);
    EXPECT_EQ(std::this_thread::get_id(), queue->consumerId);
    // double register check
    EXPECT_EQ(false , int_q_register_consumer(queue.get()));
    EXPECT_EQ(std::this_thread::get_id(), queue->consumerId);

    for(int i =0; i < DataSize; i++)
    {
        int data;
        if(!int_q_consume(queue.get(), &data))
        {
            break;
        }
        EXPECT_EQ(i, data);
    }

    bool isUnregistered = int_q_unregister_consumer(queue.get());
    EXPECT_EQ(true, isUnregistered);
    EXPECT_EQ(std::thread::id{}, queue->consumerId);
    // double unregister check
    EXPECT_EQ(false, int_q_unregister_consumer(queue.get()));
    EXPECT_EQ(std::thread::id{}, queue->consumerId);
}
TEST(QueueTest, SpscTest) 
{
    std::shared_ptr<Spsc::Queue<int>> queue = std::make_shared<Spsc::Queue<int>>(QueueSize);

    {
        barrier.store(2);
        std::thread prod(ProducerTest, queue, true);
        barrier--;
        std::thread cons(ConsumerTest, queue, true);
        barrier--;

        prod.join();
        cons.join();
    }
    // check reusability
    {
        std::thread prod(ProducerTest, queue, false);
        std::thread cons(ConsumerTest, queue, false);

        prod.join();
        cons.join();
    }
}

TEST(QueueTest, DelayedTest) 
{
    using namespace std::chrono_literals;

    std::shared_ptr<Spsc::Queue<int>> queue = std::make_shared<Spsc::Queue<int>>(QueueSize);
    
        std::thread cons(ConsumerTest, queue, false);
        std::this_thread::sleep_for(200ms);
        std::thread prod(ProducerTest, queue, false);

        prod.join();
        cons.join();
}

TEST(QueueTest, ShutdownTest) 
{
    using namespace std::chrono_literals;
    // producer shutdown test
    {
        std::shared_ptr<Spsc::Queue<int>> queue = std::make_shared<Spsc::Queue<int>>(QueueSize);
        
        std::thread prod(ProducerTest, queue, false);
        std::this_thread::sleep_for(200ms);
        int_q_shutdown(queue.get());
        // if join happends then queue is terminated
        prod.join();
    }
    // consumer shutdown test
    {
        std::shared_ptr<Spsc::Queue<int>> queue = std::make_shared<Spsc::Queue<int>>(QueueSize);
        
        std::thread cons(ConsumerTest, queue, false);
        std::this_thread::sleep_for(200ms);
        int_q_shutdown(queue.get());
        // if join happends then queue is terminated
        cons.join();
    }
}