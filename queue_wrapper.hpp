#pragma once
#include "Queue.hpp"

void int_q_enqueue(Spsc::Queue<int>* queue, const int& data);
int int_q_consume(Spsc::Queue<int>* queue);

bool int_q_register_producer(Spsc::Queue<int>* q);
bool int_q_unregister_producer(Spsc::Queue<int>* q);

bool int_q_register_consumer(Spsc::Queue<int>* q);
bool int_q_unregister_consumer(Spsc::Queue<int>* q);
