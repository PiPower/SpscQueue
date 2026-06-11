#include <gtest/gtest.h>
#include <thread>
#include "queue_wrapper.hpp"
// Check if queue works at all
static constexpr int DataSize = 27;
static constexpr int QueueSize = 20;
static int TestData[DataSize] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26};

void ProducerTest(std::shared_ptr<Spsc::Queue<int>> queue)
{
    bool isRegistered = int_q_register_producer(queue.get());
    EXPECT_EQ(true, isRegistered);
    EXPECT_EQ(true, queue->isProducerUsed.load());

    for(int i =0; i < DataSize; i++)
    {
        int_q_enqueue(queue.get(), TestData[i]);
    }

    bool isUnregistered = int_q_unregister_producer(queue.get());
    EXPECT_EQ(true, isUnregistered);
    EXPECT_EQ(false, queue->isProducerUsed.load());
}

void ConsumerTest(std::shared_ptr<Spsc::Queue<int>> queue)
{
    bool isRegistered = int_q_register_consumer(queue.get());
    EXPECT_EQ(true, isRegistered);
    EXPECT_EQ(true, queue->isConsumerUsed.load());

    for(int i =0; i < DataSize; i++)
    {
        int data = int_q_consume(queue.get());
        EXPECT_EQ(TestData[i], data);
    }

    bool isUnregistered = int_q_unregister_consumer(queue.get());
    EXPECT_EQ(true, isUnregistered);
    EXPECT_EQ(false, queue->isConsumerUsed.load());
}
TEST(QueueTest, SpscTest) 
{
    std::shared_ptr<Spsc::Queue<int>> queue = std::make_shared<Spsc::Queue<int>>(QueueSize);
    std::thread prod(ProducerTest, queue);
    std::thread cons(ConsumerTest, queue);

    prod.join();
    cons.join();
}