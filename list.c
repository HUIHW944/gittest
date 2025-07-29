#include <stdio.h>
#include <stdlib.h>

// 定义链表节点结构
struct Node {
    int data;           // 数据域（可根据需求改为其他类型）
    struct Node* next;  // 指向下一个节点的指针
};
struct Node* createNode(int data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) {
        printf("内存分配失败！\n");
        exit(1);
    }
    newNode->data = data;
    newNode->next = NULL;  // 初始化为NULL
    return newNode;
}
struct Node* insertAtHead(struct Node* head, int data) {
    struct Node* newNode = createNode(data);
    newNode->next = head;  // 新节点指向原头节点
    return newNode;        // 返回新的头节点
}
struct Node* insertAtTail(struct Node* head, int data) {
    struct Node* newNode = createNode(data);
    if (head == NULL) return newNode;  // 空链表直接返回新节点
    
    struct Node* temp = head;
    while (temp->next != NULL) {
        temp = temp->next;  // 找到最后一个节点
    }
    temp->next = newNode;   // 插入到尾部
    return head;
}
struct Node* deleteNode(struct Node* head, int key) {
    struct Node* temp = head;
    struct Node* prev = NULL;

    // 找到要删除的节点
    while (temp != NULL && temp->data != key) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) return head;  // 未找到目标节点

    if (prev == NULL) {
        head = temp->next;  // 删除头节点
    } else {
        prev->next = temp->next;  // 删除中间/尾部节点
    }
    free(temp);  // 释放内存
    return head;
}
void printList(struct Node* head) {
    struct Node* temp = head;
    while (temp != NULL) {
        printf("%d -> ", temp->data);
        temp = temp->next;
    }
    printf("NULL\n");
}
void freeList(struct Node* head) {
    struct Node* temp;
    while (head != NULL) {
        temp = head;
        head = head->next;
        free(temp);
    }
}
int main() {
    struct Node* head = NULL;  
    // 插入节点
    head = insertAtHead(head, 10);    // 10 -> NULL
    head = insertAtTail(head, 20);    // 10 -> 20 -> NULL
    head = insertAtHead(head, 5);     // 5 -> 10 -> 20 -> NULL
    // 遍历链表
    printf("初始链表: ");
    printList(head);  // 输出: 5 -> 10 -> 20 -> NULL
    // 删除节点
    head = deleteNode(head, 10);      // 5 -> 20 -> NULL
    printf("删除10后的链表: ");
    printList(head);  // 输出: 5 -> 20 -> NULL
    // 释放内存
    freeList(head);
    return 0;
}