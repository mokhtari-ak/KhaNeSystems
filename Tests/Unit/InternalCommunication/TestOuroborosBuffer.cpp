#include "InternalCommunication/Inc/OuroborosBuffer.hpp"
#include <cassert>
#include <iostream>

struct DummyMessage {
    int id;
    float value;
};

void test_basic_push_pop() {
    Communication::EventBus::OuroborosBuffer<DummyMessage, 5> buffer;
    DummyMessage msg{1, 10.5f};
    
    buffer.push(msg);
    assert(buffer.size() == 1);
    
    DummyMessage out;
    bool success = buffer.pop(out);
    assert(success);
    assert(out.id == 1);
    assert(out.value == 10.5f);
    assert(buffer.size() == 0);
    
    std::cout << "test_basic_push_pop passed!" << std::endl;
}

void test_ouroboros_overwrite() {
    Communication::EventBus::OuroborosBuffer<DummyMessage, 2> buffer;
    
    buffer.push({1, 1.0f});
    buffer.push({2, 2.0f});
    assert(buffer.size() == 2);
    
    // This should overwrite id 1
    buffer.push({3, 3.0f});
    assert(buffer.size() == 2);
    
    DummyMessage out;
    buffer.pop(out);
    assert(out.id == 2); // First-in First-out? Or Last-in First-out?
    // Requirement says "typed internal event bus with fixed-size queues". 
    // Usually a queue is FIFO. Let's assume FIFO.
    
    buffer.pop(out);
    assert(out.id == 3);
    
    std::cout << "test_ouroboros_overwrite passed!" << std::endl;
}

int main() {
    test_basic_push_pop();
    test_ouroboros_overwrite();
    return 0;
}
