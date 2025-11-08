#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <iostream>
#include <stdexcept>
#include <memory>

namespace algorithms {

/**
 * @brief 链表节点结构
 */
template<typename T>
struct ListNode {
    T data;
    std::shared_ptr<ListNode<T>> next;
    std::shared_ptr<ListNode<T>> prev; // 用于双向链表
    
    ListNode(const T& value) : data(value), next(nullptr), prev(nullptr) {}
};

/**
 * @brief 单向链表实现
 * 
 * 实现算法导论10.2节的链表操作：
 * - LIST-SEARCH(L, k): 搜索包含关键字k的元素
 * - LIST-INSERT(L, x): 在链表前端插入元素
 * - LIST-DELETE(L, x): 删除元素x
 */
template<typename T>
class SinglyLinkedList {
private:
    std::shared_ptr<ListNode<T>> head;
    int size;

public:
    /**
     * @brief 默认构造函数
     */
    SinglyLinkedList() : head(nullptr), size(0) {}
    
    /**
     * @brief 搜索包含指定值的节点（LIST-SEARCH操作）
     * @param key 要搜索的值
     * @return 包含该值的节点指针，如果未找到返回nullptr
     */
    std::shared_ptr<ListNode<T>> search(const T& key) {
        auto current = head;
        while (current != nullptr && current->data != key) {
            current = current->next;
        }
        return current;
    }
    
    /**
     * @brief 在链表前端插入元素（LIST-INSERT操作）
     * @param value 要插入的值
     */
    void insert(const T& value) {
        auto new_node = std::make_shared<ListNode<T>>(value);
        new_node->next = head;
        head = new_node;
        size++;
    }
    
    /**
     * @brief 删除指定节点（LIST-DELETE操作）
     * @param node 要删除的节点
     * @throws std::invalid_argument 如果节点为空或不在链表中
     */
    void remove(std::shared_ptr<ListNode<T>> node) {
        if (node == nullptr) {
            throw std::invalid_argument("节点不能为空");
        }
        
        // 如果要删除的是头节点
        if (node == head) {
            head = head->next;
            size--;
            return;
        }
        
        // 查找前驱节点
        auto current = head;
        while (current != nullptr && current->next != node) {
            current = current->next;
        }
        
        if (current == nullptr) {
            throw std::invalid_argument("节点不在链表中");
        }
        
        current->next = node->next;
        size--;
    }
    
    /**
     * @brief 删除包含指定值的第一个节点
     * @param value 要删除的值
     * @return 是否成功删除
     */
    bool remove_value(const T& value) {
        auto node = search(value);
        if (node != nullptr) {
            remove(node);
            return true;
        }
        return false;
    }
    
    /**
     * @brief 获取链表的大小
     * @return 链表中元素的数量
     */
    int get_size() const {
        return size;
    }
    
    /**
     * @brief 检查链表是否为空
     * @return 如果链表为空返回true，否则返回false
     */
    bool empty() const {
        return head == nullptr;
    }
    
    /**
     * @brief 获取头节点
     * @return 头节点指针
     */
    std::shared_ptr<ListNode<T>> get_head() const {
        return head;
    }
    
    /**
     * @brief 反转链表
     */
    void reverse() {
        std::shared_ptr<ListNode<T>> prev = nullptr;
        auto current = head;
        
        while (current != nullptr) {
            auto next = current->next;
            current->next = prev;
            prev = current;
            current = next;
        }
        
        head = prev;
    }
    
    /**
     * @brief 打印链表的内容
     */
    void print() const {
        auto current = head;
        std::cout << "单向链表: ";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " -> ";
            }
            current = current->next;
        }
        std::cout << " -> NULL" << std::endl;
    }
};

/**
 * @brief 双向链表实现
 * 
 * 实现算法导论10.2节的双向链表操作
 */
template<typename T>
class DoublyLinkedList {
private:
    std::shared_ptr<ListNode<T>> head;
    std::shared_ptr<ListNode<T>> tail;
    int size;

public:
    /**
     * @brief 默认构造函数
     */
    DoublyLinkedList() : head(nullptr), tail(nullptr), size(0) {}
    
    /**
     * @brief 在链表前端插入元素
     * @param value 要插入的值
     */
    void insert_front(const T& value) {
        auto new_node = std::make_shared<ListNode<T>>(value);
        
        if (empty()) {
            head = tail = new_node;
        } else {
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        size++;
    }
    
    /**
     * @brief 在链表尾端插入元素
     * @param value 要插入的值
     */
    void insert_back(const T& value) {
        auto new_node = std::make_shared<ListNode<T>>(value);
        
        if (empty()) {
            head = tail = new_node;
        } else {
            tail->next = new_node;
            new_node->prev = tail;
            tail = new_node;
        }
        size++;
    }
    
    /**
     * @brief 搜索包含指定值的节点
     * @param key 要搜索的值
     * @return 包含该值的节点指针，如果未找到返回nullptr
     */
    std::shared_ptr<ListNode<T>> search(const T& key) {
        auto current = head;
        while (current != nullptr && current->data != key) {
            current = current->next;
        }
        return current;
    }
    
    /**
     * @brief 删除指定节点
     * @param node 要删除的节点
     * @throws std::invalid_argument 如果节点为空或不在链表中
     */
    void remove(std::shared_ptr<ListNode<T>> node) {
        if (node == nullptr) {
            throw std::invalid_argument("节点不能为空");
        }
        
        // 更新前驱节点的next指针
        if (node->prev != nullptr) {
            node->prev->next = node->next;
        } else {
            head = node->next;
        }
        
        // 更新后继节点的prev指针
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        } else {
            tail = node->prev;
        }
        
        size--;
    }
    
    /**
     * @brief 删除包含指定值的第一个节点
     * @param value 要删除的值
     * @return 是否成功删除
     */
    bool remove_value(const T& value) {
        auto node = search(value);
        if (node != nullptr) {
            remove(node);
            return true;
        }
        return false;
    }
    
    /**
     * @brief 获取链表的大小
     * @return 链表中元素的数量
     */
    int get_size() const {
        return size;
    }
    
    /**
     * @brief 检查链表是否为空
     * @return 如果链表为空返回true，否则返回false
     */
    bool empty() const {
        return head == nullptr;
    }
    
    /**
     * @brief 获取头节点
     * @return 头节点指针
     */
    std::shared_ptr<ListNode<T>> get_head() const {
        return head;
    }
    
    /**
     * @brief 获取尾节点
     * @return 尾节点指针
     */
    std::shared_ptr<ListNode<T>> get_tail() const {
        return tail;
    }
    
    /**
     * @brief 正向打印链表的内容
     */
    void print_forward() const {
        auto current = head;
        std::cout << "双向链表(正向): NULL <-> ";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->next != nullptr) {
                std::cout << " <-> ";
            }
            current = current->next;
        }
        std::cout << " <-> NULL" << std::endl;
    }
    
    /**
     * @brief 反向打印链表的内容
     */
    void print_backward() const {
        auto current = tail;
        std::cout << "双向链表(反向): NULL <-> ";
        while (current != nullptr) {
            std::cout << current->data;
            if (current->prev != nullptr) {
                std::cout << " <-> ";
            }
            current = current->prev;
        }
        std::cout << " <-> NULL" << std::endl;
    }
};

/**
 * @brief 循环链表实现
 * 
 * 实现算法导论10.2节的循环链表
 */
template<typename T>
class CircularLinkedList {
private:
    std::shared_ptr<ListNode<T>> head;
    int size;

public:
    /**
     * @brief 默认构造函数
     */
    CircularLinkedList() : head(nullptr), size(0) {}
    
    /**
     * @brief 在链表前端插入元素
     * @param value 要插入的值
     */
    void insert(const T& value) {
        auto new_node = std::make_shared<ListNode<T>>(value);
        
        if (empty()) {
            new_node->next = new_node; // 指向自己
            head = new_node;
        } else {
            // 找到尾节点
            auto tail = head;
            while (tail->next != head) {
                tail = tail->next;
            }
            
            new_node->next = head;
            tail->next = new_node;
            head = new_node;
        }
        size++;
    }
    
    /**
     * @brief 搜索包含指定值的节点
     * @param key 要搜索的值
     * @return 包含该值的节点指针，如果未找到返回nullptr
     */
    std::shared_ptr<ListNode<T>> search(const T& key) {
        if (empty()) return nullptr;
        
        auto current = head;
        do {
            if (current->data == key) {
                return current;
            }
            current = current->next;
        } while (current != head);
        
        return nullptr;
    }
    
    /**
     * @brief 删除指定节点
     * @param node 要删除的节点
     * @throws std::invalid_argument 如果节点为空或不在链表中
     */
    void remove(std::shared_ptr<ListNode<T>> node) {
        if (node == nullptr) {
            throw std::invalid_argument("节点不能为空");
        }
        
        if (empty()) {
            throw std::invalid_argument("链表为空");
        }
        
        // 如果要删除的是头节点
        if (node == head) {
            if (size == 1) {
                head = nullptr;
            } else {
                // 找到尾节点
                auto tail = head;
                while (tail->next != head) {
                    tail = tail->next;
                }
                
                head = head->next;
                tail->next = head;
            }
        } else {
            // 查找前驱节点
            auto prev = head;
            while (prev->next != node && prev->next != head) {
                prev = prev->next;
            }
            
            if (prev->next != node) {
                throw std::invalid_argument("节点不在链表中");
            }
            
            prev->next = node->next;
        }
        
        size--;
    }
    
    /**
     * @brief 删除包含指定值的第一个节点
     * @param value 要删除的值
     * @return 是否成功删除
     */
    bool remove_value(const T& value) {
        auto node = search(value);
        if (node != nullptr) {
            remove(node);
            return true;
        }
        return false;
    }
    
    /**
     * @brief 获取链表的大小
     * @return 链表中元素的数量
     */
    int get_size() const {
        return size;
    }
    
    /**
     * @brief 检查链表是否为空
     * @return 如果链表为空返回true，否则返回false
     */
    bool empty() const {
        return head == nullptr;
    }
    
    /**
     * @brief 打印循环链表的内容
     */
    void print() const {
        if (empty()) {
            std::cout << "循环链表: 空" << std::endl;
            return;
        }
        
        auto current = head;
        std::cout << "循环链表: ";
        do {
            std::cout << current->data;
            if (current->next != head) {
                std::cout << " -> ";
            }
            current = current->next;
        } while (current != head);
        std::cout << " -> [回到头节点]" << std::endl;
    }
};

} // namespace algorithms

#endif // LINKED_LIST_H