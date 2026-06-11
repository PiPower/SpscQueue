#include "queue_wrapper.hpp"

void int_q_enqueue(Spsc::Queue<int>* queue, const int &data)
{
   produce(queue, data);
}

int int_q_consume(Spsc::Queue<int>* queue)
{
    return consume(queue);
}

bool int_q_register_producer(Spsc::Queue<int>* q)
{
    return Spsc::registerProducer<int>(q);
}

bool int_q_unregister_producer(Spsc::Queue<int>* q)
{
    return Spsc::unregisterProducer<int>(q);
}

bool int_q_register_consumer(Spsc::Queue<int>* q)
{
    return Spsc::registerConsumer<int>(q);
}

bool int_q_unregister_consumer(Spsc::Queue<int>* q)
{
    return Spsc::unregisterConsumer<int>(q);
}