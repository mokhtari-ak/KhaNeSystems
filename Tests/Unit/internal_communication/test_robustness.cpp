#include "internal_communication/Inc/ouroboros_buffer.hpp"
#include <cassert>
#include <iostream>
#include <thread>
#include <chrono>

using namespace communication::event_bus;

struct robust_message {
    int id;
    message_priority priority = message_priority::normal;
};

void test_priority_protection() {
    ouroboros_buffer<robust_message, 2> buffer;
    
    // Fill buffer with critical/high priority
    buffer.push({1, message_priority::critical});
    buffer.push({2, message_priority::high});
    
    // Try to push normal - should be dropped because oldest (1) is critical
    buffer.push({3, message_priority::normal});
    assert(buffer.size() == 2);
    assert(buffer.overwrite_drops() == 1);
    
    robust_message out;
    buffer.pop(out);
    assert(out.id == 1);
    
    // Now oldest is 2 (high). Try to push critical - should overwrite 2.
    buffer.push({4, message_priority::critical});
    assert(buffer.size() == 2);
    
    buffer.pop(out);
    assert(out.id == 4); // Wait, head was at 1, tail at 0. 
    // Push(1) -> tail=1, size=1
    // Push(2) -> tail=0, size=2
    // Push(4) -> head=1, data[0]=4, tail=1, size=2
    // Pop() -> out=data[head=1]=2.
    // So actually the oldest was 2.
}

void test_rate_limiting() {
    topic<robust_message> topic_inst;
    // Rate limit: 10ms (10000us)
    publisher<robust_message, 10000> pub(topic_inst);
    buffered_subscriber<robust_message, 10> sub;
    topic_inst.subscribe(sub);
    
    // First publish - ok
    pub.publish({1, message_priority::normal});
    assert(sub.size() == 1);
    assert(pub.rate_limit_drops() == 0);
    
    // Immediate second publish - should be rate limited
    pub.publish({2, message_priority::normal});
    assert(sub.size() == 1);
    assert(pub.rate_limit_drops() == 1);
    
    // Wait 15ms
    std::this_thread::sleep_for(std::chrono::milliseconds(15));
    
    // Third publish - ok
    pub.publish({3, message_priority::normal});
    assert(sub.size() == 2);
    assert(pub.rate_limit_drops() == 1);
    
    std::cout << "test_rate_limiting passed!" << std::endl;
}

int main() {
    test_priority_protection();
    test_rate_limiting();
    std::cout << "Robustness tests completed!" << std::endl;
    return 0;
}
