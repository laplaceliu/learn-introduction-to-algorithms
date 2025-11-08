#include "rooted_tree.h"
#include <iostream>
#include <vector>

using namespace algorithms;

void test_basic_rooted_tree() {
    std::cout << "=== 测试基本有根树 ===" << std::endl;
    
    // 创建有根树
    RootedTree<std::string> tree("A");
    auto root = tree.get_root();
    
    std::cout << "创建根节点: " << root->data << std::endl;
    std::cout << "根节点深度: " << root->depth() << std::endl;
    std::cout << "根节点高度: " << root->height() << std::endl;
    std::cout << "根节点是否为叶子: " << (root->is_leaf() ? "是" : "否") << std::endl;
    
    // 添加子节点
    std::cout << "\n添加子节点 B, C, D 到根节点 A" << std::endl;
    auto nodeB = tree.insert_child(root, "B");
    auto nodeC = tree.insert_child(root, "C");
    auto nodeD = tree.insert_child(root, "D");
    
    tree.print_tree();
    std::cout << "树的大小: " << tree.get_size() << std::endl;
    std::cout << "树的高度: " << tree.height() << std::endl;
    
    // 继续构建树
    std::cout << "\n添加子节点 E, F 到节点 B" << std::endl;
    auto nodeE = tree.insert_child(nodeB, "E");
    auto nodeF = tree.insert_child(nodeB, "F");
    
    std::cout << "添加子节点 G 到节点 C" << std::endl;
    auto nodeG = tree.insert_child(nodeC, "G");
    
    std::cout << "添加子节点 H, I 到节点 D" << std::endl;
    auto nodeH = tree.insert_child(nodeD, "H");
    auto nodeI = tree.insert_child(nodeD, "I");
    
    tree.print_tree();
    std::cout << "树的大小: " << tree.get_size() << std::endl;
    std::cout << "树的高度: " << tree.height() << std::endl;
    
    // 测试节点属性
    std::cout << "\n节点属性测试:" << std::endl;
    std::cout << "节点B的深度: " << nodeB->depth() << std::endl;
    std::cout << "节点B的高度: " << nodeB->height() << std::endl;
    std::cout << "节点E的深度: " << nodeE->depth() << std::endl;
    std::cout << "节点E的高度: " << nodeE->height() << std::endl;
    std::cout << "节点E是否为叶子: " << (nodeE->is_leaf() ? "是" : "否") << std::endl;
    
    std::cout << std::endl;
}

void test_tree_traversal() {
    std::cout << "=== 测试树遍历算法 ===" << std::endl;
    
    // 创建测试树
    RootedTree<int> tree(1);
    auto root = tree.get_root();
    
    auto node2 = tree.insert_child(root, 2);
    auto node3 = tree.insert_child(root, 3);
    auto node4 = tree.insert_child(root, 4);
    
    auto node5 = tree.insert_child(node2, 5);
    auto node6 = tree.insert_child(node2, 6);
    
    auto node7 = tree.insert_child(node3, 7);
    
    auto node8 = tree.insert_child(node4, 8);
    auto node9 = tree.insert_child(node4, 9);
    
    tree.print_tree();
    
    // 前序遍历
    std::vector<int> preorder_result;
    tree.preorder_traversal(root, preorder_result);
    std::cout << "前序遍历: ";
    for (int val : preorder_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 后序遍历
    std::vector<int> postorder_result;
    tree.postorder_traversal(root, postorder_result);
    std::cout << "后序遍历: ";
    for (int val : postorder_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 层序遍历
    std::vector<int> level_order_result = tree.level_order_traversal();
    std::cout << "层序遍历: ";
    for (int val : level_order_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << std::endl;
}

void test_tree_search() {
    std::cout << "=== 测试树搜索算法 ===" << std::endl;
    
    RootedTree<std::string> tree("Apple");
    auto root = tree.get_root();
    
    auto banana = tree.insert_child(root, "Banana");
    auto cherry = tree.insert_child(root, "Cherry");
    
    auto date = tree.insert_child(banana, "Date");
    auto elderberry = tree.insert_child(banana, "Elderberry");
    
    auto fig = tree.insert_child(cherry, "Fig");
    
    tree.print_tree();
    
    // BFS搜索
    std::cout << "BFS搜索 'Fig': ";
    auto bfs_result = tree.bfs("Fig");
    if (bfs_result != nullptr) {
        std::cout << "找到节点 " << bfs_result->data << std::endl;
    } else {
        std::cout << "未找到" << std::endl;
    }
    
    std::cout << "BFS搜索 'Grape': ";
    bfs_result = tree.bfs("Grape");
    if (bfs_result != nullptr) {
        std::cout << "找到节点 " << bfs_result->data << std::endl;
    } else {
        std::cout << "未找到" << std::endl;
    }
    
    // DFS搜索
    std::cout << "DFS搜索 'Date': ";
    auto dfs_result = tree.dfs("Date");
    if (dfs_result != nullptr) {
        std::cout << "找到节点 " << dfs_result->data << std::endl;
    } else {
        std::cout << "未找到" << std::endl;
    }
    
    std::cout << std::endl;
}

void test_binary_tree() {
    std::cout << "=== 测试二叉树（有根树的特例） ===" << std::endl;
    
    BinaryTree<std::string> btree("A");
    auto root = btree.get_root();
    
    // 构建二叉树
    auto B = btree.insert_left(root, "B");
    auto C = btree.insert_right(root, "C");
    
    auto D = btree.insert_left(B, "D");
    auto E = btree.insert_right(B, "E");
    
    auto F = btree.insert_left(C, "F");
    auto G = btree.insert_right(C, "G");
    
    std::cout << "二叉树结构:" << std::endl;
    std::cout << "      A" << std::endl;
    std::cout << "     / \\" << std::endl;
    std::cout << "    B   C" << std::endl;
    std::cout << "   / \\ / \\" << std::endl;
    std::cout << "  D  E F  G" << std::endl;
    
    // 中序遍历
    std::vector<std::string> inorder_result;
    btree.inorder_traversal(root, inorder_result);
    std::cout << "中序遍历: ";
    for (const auto& val : inorder_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 前序遍历
    std::vector<std::string> preorder_result;
    btree.preorder_traversal(root, preorder_result);
    std::cout << "前序遍历: ";
    for (const auto& val : preorder_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    // 后序遍历
    std::vector<std::string> postorder_result;
    btree.postorder_traversal(root, postorder_result);
    std::cout << "后序遍历: ";
    for (const auto& val : postorder_result) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
    
    std::cout << "二叉树大小: " << btree.get_size() << std::endl;
    
    std::cout << std::endl;
}

void test_edge_cases() {
    std::cout << "=== 测试边界情况 ===" << std::endl;
    
    // 测试空树
    RootedTree<int> empty_tree;
    std::cout << "空树是否为空: " << (empty_tree.empty() ? "是" : "否") << std::endl;
    std::cout << "空树大小: " << empty_tree.get_size() << std::endl;
    std::cout << "空树高度: " << empty_tree.height() << std::endl;
    
    // 测试单节点树
    RootedTree<int> single_node_tree(42);
    auto root = single_node_tree.get_root();
    std::cout << "\n单节点树:" << std::endl;
    std::cout << "根节点值: " << root->data << std::endl;
    std::cout << "根节点深度: " << root->depth() << std::endl;
    std::cout << "根节点高度: " << root->height() << std::endl;
    std::cout << "根节点是否为叶子: " << (root->is_leaf() ? "是" : "否") << std::endl;
    std::cout << "树大小: " << single_node_tree.get_size() << std::endl;
    
    // 测试只有一层子节点的树
    RootedTree<int> one_level_tree(1);
    auto r = one_level_tree.get_root();
    one_level_tree.insert_child(r, 2);
    one_level_tree.insert_child(r, 3);
    one_level_tree.insert_child(r, 4);
    
    std::cout << "\n只有一层子节点的树:" << std::endl;
    one_level_tree.print_tree();
    std::cout << "树高度: " << one_level_tree.height() << std::endl;
    
    std::cout << std::endl;
}

void test_tree_applications() {
    std::cout << "=== 测试树的应用场景 ===" << std::endl;
    
    // 文件系统树示例
    std::cout << "文件系统树示例:" << std::endl;
    RootedTree<std::string> file_system("/");
    auto root = file_system.get_root();
    
    auto home = file_system.insert_child(root, "home");
    auto etc = file_system.insert_child(root, "etc");
    auto var_dir = file_system.insert_child(root, "var");
    
    auto user1 = file_system.insert_child(home, "user1");
    auto user2 = file_system.insert_child(home, "user2");
    
    auto documents = file_system.insert_child(user1, "Documents");
    auto downloads = file_system.insert_child(user1, "Downloads");
    
    file_system.print_tree();
    
    // 组织结构树示例
    std::cout << "\n组织结构树示例:" << std::endl;
    RootedTree<std::string> org_structure("CEO");
    auto ceo = org_structure.get_root();
    
    auto cto = org_structure.insert_child(ceo, "CTO");
    auto cfo = org_structure.insert_child(ceo, "CFO");
    auto coo = org_structure.insert_child(ceo, "COO");
    
    auto dev_manager = org_structure.insert_child(cto, "开发经理");
    auto qa_manager = org_structure.insert_child(cto, "测试经理");
    
    auto dev1 = org_structure.insert_child(dev_manager, "开发工程师1");
    auto dev2 = org_structure.insert_child(dev_manager, "开发工程师2");
    
    org_structure.print_tree();
    
    std::cout << std::endl;
}

int main() {
    std::cout << "=== 算法导论 10.4节 - 有根树表示演示 ===" << std::endl;
    std::cout << std::endl;
    
    test_basic_rooted_tree();
    test_tree_traversal();
    test_tree_search();
    test_binary_tree();
    test_edge_cases();
    test_tree_applications();
    
    std::cout << "=== 演示结束 ===" << std::endl;
    
    return 0;
}