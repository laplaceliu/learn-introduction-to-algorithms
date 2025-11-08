#include "linked_list.h"
#include <iostream>
#include <memory>

using namespace algorithms;

void test_singly_linked_list() {
    std::cout << "=== 测试单向链表 ===" << std::endl;
    
    SinglyLinkedList<int> list;
    
    std::cout << "初始链表是否为空: " << (list.empty() ? "是" : "否") << std::endl;
    
    // LIST-INSERT操作
    std::cout << "执行 LIST-INSERT(L, 10)" << std::endl;
    list.insert(10);
    list.print();
    
    std::cout << "执行 LIST-INSERT(L, 20)" << std::endl;
    list.insert(20);
    list.print();
    
    std::cout << "执行 LIST-INSERT(L, 30)" << std::endl;
    list.insert(30);
    list.print();
    
    // LIST-SEARCH操作
    std::cout << "执行 LIST-SEARCH(L, 20): ";
    auto node20 = list.search(20);
    if (node20 != nullptr) {
        std::cout << "找到节点，值为 " << node20->data << std::endl;
    } else {
        std::cout << "未找到" << std::endl;
    }
    
    std::cout << "执行 LIST-SEARCH(L, 40): ";
    auto node40 = list.search(40);
    if (node40 != nullptr) {
        std::cout << "找到节点，值为 " << node40->data << std::endl;
    } else {
        std::cout << "未找到" << std::endl;
    }
    
    // LIST-DELETE操作
    std::cout << "执行 LIST-DELETE(L, 找到的20节点)" << std::endl;
    if (node20 != nullptr) {
        list.remove(node20);
    }
    list.print();
    
    std::cout << "执行 LIST-DELETE(L, 值为10的节点)" << std::endl;
    list.remove_value(10);
    list.print();
    
    // 反转链表
    std::cout << "反转链表" << std::endl;
    list.reverse();
    list.print();
    
    std::cout << "链表大小: " << list.get_size() << std::endl;
    
    std::cout << std::endl;
}

void test_doubly_linked_list() {
    std::cout << "=== 测试双向链表 ===" << std::endl;
    
    DoublyLinkedList<int> dlist;
    
    std::cout << "初始双向链表是否为空: " << (dlist.empty() ? "是" : "否") << std::endl;
    
    // 前端插入
    std::cout << "在链表前端插入: 10, 20, 30" << std::endl;
    dlist.insert_front(10);
    dlist.insert_front(20);
    dlist.insert_front(30);
    dlist.print_forward();
    dlist.print_backward();
    
    // 后端插入
    std::cout << "在链表后端插入: 40, 50" << std::endl;
    dlist.insert_back(40);
    dlist.insert_back(50);
    dlist.print_forward();
    dlist.print_backward();
    
    // 搜索操作
    std::cout << "搜索值30: ";
    auto node30 = dlist.search(30);
    if (node30 != nullptr) {
        std::cout << "找到节点，值为 " << node30->data << std::endl;
    } else {
        std::cout << "未找到" << std::endl;
    }
    
    // 删除操作
    std::cout << "删除值30的节点" << std::endl;
    if (node30 != nullptr) {
        dlist.remove(node30);
    }
    dlist.print_forward();
    
    std::cout << "删除值50的节点" << std::endl;
    dlist.remove_value(50);
    dlist.print_forward();
    
    std::cout << "双向链表大小: " << dlist.get_size() << std::endl;
    
    std::cout << std::endl;
}

void test_circular_linked_list() {
    std::cout << "=== 测试循环链表 ===" << std::endl;
    
    CircularLinkedList<int> clist;
    
    std::cout << "初始循环链表是否为空: " << (clist.empty() ? "是" : "否") << std::endl;
    
    // 插入操作
    std::cout << "插入元素: 10, 20, 30, 40" << std::endl;
    clist.insert(10);
    clist.insert(20);
    clist.insert(30);
    clist.insert(40);
    clist.print();
    
    // 搜索操作
    std::cout << "搜索值20: ";
    auto node20 = clist.search(20);
    if (node20 != nullptr) {
        std::cout << "找到节点，值为 " << node20->data << std::endl;
    } else {
        std::cout << "未找到" << std::endl;
    }
    
    std::cout << "搜索值50: ";
    auto node50 = clist.search(50);
    if (node50 != nullptr) {
        std::cout << "找到节点，值为 " << node50->data << std::endl;
    } else {
        std::cout << "未找到" << std::endl;
    }
    
    // 删除操作
    std::cout << "删除值20的节点" << std::endl;
    if (node20 != nullptr) {
        clist.remove(node20);
    }
    clist.print();
    
    std::cout << "删除值40的节点" << std::endl;
    clist.remove_value(40);
    clist.print();
    
    std::cout << "循环链表大小: " << clist.get_size() << std::endl;
    
    std::cout << std::endl;
}

void test_algorithm_operations() {
    std::cout << "=== 测试算法导论标准操作序列 ===" << std::endl;
    
    // 单向链表的标准操作序列
    std::cout << "单向链表操作序列:" << std::endl;
    SinglyLinkedList<int> list;
    
    std::cout << "LIST-INSERT(L, 5)" << std::endl;
    list.insert(5);
    list.print();
    
    std::cout << "LIST-INSERT(L, 8)" << std::endl;
    list.insert(8);
    list.print();
    
    std::cout << "LIST-SEARCH(L, 5): ";
    auto node5 = list.search(5);
    if (node5 != nullptr) {
        std::cout << "找到" << std::endl;
    } else {
        std::cout << "未找到" << std::endl;
    }
    
    std::cout << "LIST-DELETE(L, 找到的5节点)" << std::endl;
    if (node5 != nullptr) {
        list.remove(node5);
    }
    list.print();
    
    std::cout << "LIST-INSERT(L, 3)" << std::endl;
    list.insert(3);
    list.print();
    
    std::cout << "LIST-DELETE(L, 值为8的节点)" << std::endl;
    list.remove_value(8);
    list.print();
    
    std::cout << "链表是否为空: " << (list.empty() ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== 测试边界情况 ===" << std::endl;
    
    // 测试空链表操作
    SinglyLinkedList<int> empty_list;
    std::cout << "空链表搜索值10: ";
    auto result = empty_list.search(10);
    if (result == nullptr) {
        std::cout << "未找到（正确）" << std::endl;
    }
    
    std::cout << "空链表删除值10: ";
    bool removed = empty_list.remove_value(10);
    std::cout << (removed ? "成功" : "失败（正确）") << std::endl;
    
    // 测试单节点链表
    SinglyLinkedList<int> single_list;
    single_list.insert(42);
    std::cout << "单节点链表大小: " << single_list.get_size() << std::endl;
    single_list.print();
    
    std::cout << "删除单节点" << std::endl;
    single_list.remove_value(42);
    std::cout << "删除后链表是否为空: " << (single_list.empty() ? "是" : "否") << std::endl;
    
    // 测试删除不存在的节点
    SinglyLinkedList<int> test_list;
    test_list.insert(1);
    test_list.insert(2);
    test_list.insert(3);
    
    std::cout << "删除不存在的值99: ";
    removed = test_list.remove_value(99);
    std::cout << (removed ? "成功" : "失败（正确）") << std::endl;
    
    std::cout << std::endl;
}

void test_linked_list_applications() {
    std::cout << "=== 测试链表应用场景 ===" << std::endl;
    
    // 使用链表实现简单的任务队列
    std::cout << "使用单向链表实现任务队列:" << std::endl;
    SinglyLinkedList<std::string> task_queue;
    
    task_queue.insert("任务3");
    task_queue.insert("任务2");
    task_queue.insert("任务1");
    task_queue.print();
    
    // 处理任务（从头部开始）
    auto current_task = task_queue.get_head();
    while (current_task != nullptr) {
        std::cout << "处理任务: " << current_task->data << std::endl;
        current_task = current_task->next;
    }
    
    // 使用双向链表实现浏览器历史记录
    std::cout << "\n使用双向链表实现浏览器历史记录:" << std::endl;
    DoublyLinkedList<std::string> browser_history;
    
    browser_history.insert_back("页面A");
    browser_history.insert_back("页面B");
    browser_history.insert_back("页面C");
    
    std::cout << "当前页面: " << browser_history.get_tail()->data << std::endl;
    std::cout << "可以向前导航到: " << browser_history.get_tail()->prev->data << std::endl;
    
    std::cout << std::endl;
}

int main() {
    std::cout << "=== 算法导论 10.2节 - 链表演示 ===" << std::endl;
    std::cout << std::endl;
    
    test_singly_linked_list();
    test_doubly_linked_list();
    test_circular_linked_list();
    test_algorithm_operations();
    test_edge_cases();
    test_linked_list_applications();
    
    std::cout << "=== 演示结束 ===" << std::endl;
    
    return 0;
}