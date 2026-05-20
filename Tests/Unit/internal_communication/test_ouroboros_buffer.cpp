#include "internal_communication/Inc/ouroboros_buffer.hpp"
#include "common/sensor_types.hpp"
#include <cassert>
#include <iostream>

using namespace communication::event_bus;
using namespace Sensors;

struct dummy_message {
    int id;
    float value;
    message_priority priority = message_priority::normal;
};

void test_basic_push_pop() {
    ouroboros_buffer<dummy_message, 5> buffer;
    dummy_message msg{1, 10.5f};
    
    buffer.push(msg);
    assert(buffer.size() == 1);
    
    dummy_message out;
    bool success = buffer.pop(out);
    assert(success);
    assert(out.id == 1);
    assert(out.value == 10.5f);
    assert(buffer.size() == 0);
    
    std::cout << "test_basic_push_pop passed!" << std::endl;
}

void test_ouroboros_overwrite() {
    ouroboros_buffer<dummy_message, 2> buffer;
    
    buffer.push({1, 1.0f});
    buffer.push({2, 2.0f});
    assert(buffer.size() == 2);
    
    // This should overwrite id 1 (advance head)
    buffer.push({3, 3.0f});
    assert(buffer.size() == 2);
    
    dummy_message out;
    buffer.pop(out);
    // Since it's a FIFO queue, after overwriting 1, the oldest is 2
    assert(out.id == 2); 
    
    buffer.pop(out);
    assert(out.id == 3);
    
    std::cout << "test_ouroboros_overwrite passed!" << std::endl;
}

void test_pub_sub_multiple() {
    topic<IMUData> imu_topic;
    publisher<IMUData> imu_pub(imu_topic);
    
    buffered_subscriber<IMUData, 2> sub1;
    buffered_subscriber<IMUData, 5> sub2;
    
    imu_topic.subscribe(sub1);
    imu_topic.subscribe(sub2);
    
    IMUData msg1{.accel_z = 9.81f, .timestamp_us = 1000};
    imu_pub.publish(msg1);
    
    assert(sub1.size() == 1);
    assert(sub2.size() == 1);
    
    IMUData out1, out2;
    assert(sub1.receive(out1));
    assert(sub2.receive(out2));
    assert(out1.accel_z == 9.81f);
    assert(out1.timestamp_us == 1000);
    assert(out2.accel_z == 9.81f);
    
    std::cout << "test_pub_sub_multiple passed!" << std::endl;
}

void test_subscriber_overflow() {
    topic<dummy_message> topic_inst;
    publisher<dummy_message> pub(topic_inst);
    buffered_subscriber<dummy_message, 2> sub;
    
    topic_inst.subscribe(sub);
    
    pub.publish({1, 1.0f});
    pub.publish({2, 2.0f});
    pub.publish({3, 3.0f}); // Overwrites 1
    
    assert(sub.size() == 2);
    
    dummy_message out;
    sub.receive(out);
    assert(out.id == 2); // 1 was dropped
    
    std::cout << "test_subscriber_overflow passed!" << std::endl;
}

int main() {
    test_basic_push_pop();
    test_ouroboros_overwrite();
    test_pub_sub_multiple();
    test_subscriber_overflow();
    return 0;
}
