#include "queue_test.hpp"

void int_q_enqueue(std::shared_ptr<SpscQueue<int>> queue, const int &data)
{
    produce(queue, data);
}

int int_q_consume(std::shared_ptr<SpscQueue<int>> queue)
{
    return consume(queue);
}