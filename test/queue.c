#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 5  // 定义队列的最大容量

/* queue is FIFO */
typedef struct {
    int data[MAX_SIZE];  // 用于存储队列元素的数组
    int front;  // 队头指针，指向队头元素
    int rear;  // 队尾指针，指向队尾元素的下一个位置
} Queue;

void initQueue(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

int isFull(Queue *q) {
    return (q->rear + 1) % MAX_SIZE == q->front;
}

int isEmpty(Queue *q) {
    return q->front == q->rear;
}

// 入队操作
void enqueue(Queue *q, int value) {
    if (isFull(q)) {
        printf("队列已满，无法入队\n");
        return;
    }
    q->data[q->rear] = value;
    q->rear = (q->rear + 1) % MAX_SIZE;
}

// 出队操作
int dequeue(Queue *q) {
    if (isEmpty(q)) {
        printf("队列已空，无法出队\n");
        return -1; 
    }
    int result = q->data[q->front];
    q->front = (q->front + 1) % MAX_SIZE;
    return result;
}

// 获取队头元素
int front(Queue *q) {
    if (isEmpty(q)) {
        printf("队列已空，无队头元素\n");
        return -1;  
    }
    return q->data[q->front];
}

int main() {
    Queue q;
    initQueue(&q);

    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);

    printf("the first: %d\n", front(&q));
    printf("the second: %d\n", front(&q+1));

    int dequeued = dequeue(&q);
    printf("dequeue: %d\n", dequeued);

    return 0;
}