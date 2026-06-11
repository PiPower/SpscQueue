#pragma once
#include "Queue.hpp"

bool int_q_enqueue(Spsc::Queue<int>* queue, const int& data);
bool int_q_consume(Spsc::Queue<int>* queue, int* data);
bool int_q_register_producer(Spsc::Queue<int>* q);
bool int_q_unregister_producer(Spsc::Queue<int>* q);
bool int_q_register_consumer(Spsc::Queue<int>* q);
bool int_q_unregister_consumer(Spsc::Queue<int>* q);
void int_q_shutdown(Spsc::Queue<int>* q);