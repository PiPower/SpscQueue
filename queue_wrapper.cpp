#include "queue_wrapper.hpp"

bool int_q_enqueue(Spsc::Queue<int>* queue, const int &data)
{
   return produce(queue, data);
}

bool int_q_consume(Spsc::Queue<int>* queue, int* data)
{
    return consume(queue, data);
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

void int_q_shutdown(Spsc::Queue<int> *q)
{
    Spsc::shutdown<int>(q);
}
