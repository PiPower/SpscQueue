#pragma once
#include "Queue.hpp"

void int_q_enqueue(std::shared_ptr<SpscQueue<int>> queue, const int& data);
int int_q_consume(std::shared_ptr<SpscQueue<int>> queue);