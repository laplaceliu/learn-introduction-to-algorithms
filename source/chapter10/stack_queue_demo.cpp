#include "stack_queue.h"
#include <iostream>

using namespace algorithms;

void test_stack_operations() {
    std::cout << "=== 测试栈的基本操作 ===" << std::endl;
    
    Stack stack(5); // 创建容量为5的栈
    
    std::cout << "初始栈是否为空: " << (stack.empty() ? "是" : "否") << std::endl;
    
    // PUSH操作
    std::cout << "执行 PUSH(S, 10)" << std::endl;
    stack.push(10);
    stack.print();
    
    std::cout << "执行 PUSH(S, 20)" << std::endl;
    stack.push(20);
    stack.print();
    
    std::cout << "执行 PUSH(S, 30)" << std::endl;
    stack.push(30);
    stack.print();
    
    std::cout << "栈顶元素: " << stack.top() << std::endl;
    std::cout << "栈大小: " << stack.size() << std::endl;
    
    // POP操作
    std::cout << "执行 POP(S): " << stack.pop() << std::endl;
    stack.print();
    
    std::cout << "执行 POP(S): " << stack.pop() << std::endl;
    stack.print();
    
    std::cout << "栈是否为空: " << (stack.empty() ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void test_queue_operations() {
    std::cout << "=== 测试队列的基本操作 ===" << std::endl;
    
    Queue queue(5); // 创建容量为5的队列
    
    std::cout << "初始队列是否为空: " << (queue.empty() ? "是" : "否") << std::endl;
    
    // ENQUEUE操作
    std::cout << "执行 ENQUEUE(Q, 10)" << std::endl;
    queue.enqueue(10);
    queue.print();
    
    std::cout << "执行 ENQUEUE(Q, 20)" << std::endl;
    queue.enqueue(20);
    queue.print();
    
    std::cout << "执行 ENQUEUE(Q, 30)" << std::endl;
    queue.enqueue(30);
    queue.print();
    
    std::cout << "队头元素: " << queue.front() << std::endl;
    std::cout << "队列大小: " << queue.size() << std::endl;
    
    // DEQUEUE操作
    std::cout << "执行 DEQUEUE(Q): " << queue.dequeue() << std::endl;
    queue.print();
    
    std::cout << "执行 DEQUEUE(Q): " << queue.dequeue() << std::endl;
    queue.print();
    
    std::cout << "队列是否为空: " << (queue.empty() ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void test_deque_operations() {
    std::cout << "=== 测试双端队列操作 ===" << std::endl;
    
    Deque deque;
    
    // 在两端插入元素
    std::cout << "在队尾插入: 10, 20, 30" << std::endl;
    deque.push_back(10);
    deque.push_back(20);
    deque.push_back(30);
    deque.print();
    
    std::cout << "在队头插入: 5, 3" << std::endl;
    deque.push_front(5);
    deque.push_front(3);
    deque.print();
    
    std::cout << "队头元素: " << deque.front() << std::endl;
    std::cout << "队尾元素: " << deque.back() << std::endl;
    
    // 从两端删除元素
    std::cout << "从队头删除: " << deque.pop_front() << std::endl;
    deque.print();
    
    std::cout << "从队尾删除: " << deque.pop_back() << std::endl;
    deque.print();
    
    std::cout << std::endl;
}

void test_queue_with_two_stacks() {
    std::cout << "=== 测试用两个栈实现队列（练习10.1-6） ===" << std::endl;
    
    QueueWithTwoStacks queue(5);
    
    std::cout << "初始队列是否为空: " << (queue.empty() ? "是" : "否") << std::endl;
    
    // 入队操作
    std::cout << "入队: 10, 20, 30" << std::endl;
    queue.enqueue(10);
    queue.enqueue(20);
    queue.enqueue(30);
    queue.print();
    
    std::cout << "队头元素: " << queue.front() << std::endl;
    
    // 出队操作
    std::cout << "出队: " << queue.dequeue() << std::endl;
    queue.print();
    
    std::cout << "出队: " << queue.dequeue() << std::endl;
    queue.print();
    
    // 继续入队
    std::cout << "入队: 40, 50" << std::endl;
    queue.enqueue(40);
    queue.enqueue(50);
    queue.print();
    
    // 继续出队
    std::cout << "出队: " << queue.dequeue() << std::endl;
    queue.print();
    
    std::cout << "队列大小: " << queue.size() << std::endl;
    
    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== 测试边界情况 ===" << std::endl;
    
    // 测试栈边界
    Stack small_stack(2);
    std::cout << "测试栈边界:" << std::endl;
    small_stack.push(1);
    small_stack.push(2);
    std::cout << "栈已满，大小: " << small_stack.size() << std::endl;
    
    try {
        small_stack.push(3); // 应该抛出异常
    } catch (const std::overflow_error& e) {
        std::cout << "捕获栈溢出异常: " << e.what() << std::endl;
    }
    
    // 测试队列边界
    Queue small_queue(2);
    std::cout << "\n测试队列边界:" << std::endl;
    small_queue.enqueue(1);
    small_queue.enqueue(2);
    std::cout << "队列已满，大小: " << small_queue.size() << std::endl;
    
    try {
        small_queue.enqueue(3); // 应该抛出异常
    } catch (const std::overflow_error& e) {
        std::cout << "捕获队列溢出异常: " << e.what() << std::endl;
    }
    
    // 测试空栈/队列操作
    Stack empty_stack;
    std::cout << "\n测试空栈操作:" << std::endl;
    try {
        empty_stack.pop(); // 应该抛出异常
    } catch (const std::underflow_error& e) {
        std::cout << "捕获栈下溢异常: " << e.what() << std::endl;
    }
    
    Queue empty_queue;
    std::cout << "测试空队列操作:" << std::endl;
    try {
        empty_queue.dequeue(); // 应该抛出异常
    } catch (const std::underflow_error& e) {
        std::cout << "捕获队列下溢异常: " << e.what() << std::endl;
    }
    
    std::cout << std::endl;
}

void test_algorithm_operations() {
    std::cout << "=== 测试算法导论标准操作序列 ===" << std::endl;
    
    // 栈的标准操作序列
    std::cout << "栈操作序列:" << std::endl;
    Stack stack;
    
    std::cout << "PUSH(S, 5)" << std::endl;
    stack.push(5);
    
    std::cout << "PUSH(S, 8)" << std::endl;
    stack.push(8);
    
    std::cout << "POP(S): " << stack.pop() << std::endl;
    
    std::cout << "PUSH(S, 3)" << std::endl;
    stack.push(3);
    
    std::cout << "POP(S): " << stack.pop() << std::endl;
    
    std::cout << "POP(S): " << stack.pop() << std::endl;
    
    std::cout << "STACK-EMPTY(S): " << (stack.empty() ? "是" : "否") << std::endl;
    
    // 队列的标准操作序列
    std::cout << "\n队列操作序列:" << std::endl;
    Queue queue;
    
    std::cout << "ENQUEUE(Q, 5)" << std::endl;
    queue.enqueue(5);
    
    std::cout << "ENQUEUE(Q, 8)" << std::endl;
    queue.enqueue(8);
    
    std::cout << "DEQUEUE(Q): " << queue.dequeue() << std::endl;
    
    std::cout << "ENQUEUE(Q, 3)" << std::endl;
    queue.enqueue(3);
    
    std::cout << "DEQUEUE(Q): " << queue.dequeue() << std::endl;
    
    std::cout << "DEQUEUE(Q): " << queue.dequeue() << std::endl;
    
    std::cout << "QUEUE-EMPTY(Q): " << (queue.empty() ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "=== 算法导论 10.1节 - 栈和队列演示 ===" << std::endl;
    std::cout << std::endl;
    
    test_stack_operations();
    test_queue_operations();
    test_deque_operations();
    test_queue_with_two_stacks();
    test_edge_cases();
    test_algorithm_operations();
    
    std::cout << "=== 演示结束 ===" << std::endl;
    
    return 0;
}