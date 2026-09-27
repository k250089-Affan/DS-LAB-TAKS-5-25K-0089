#include <iostream>
using namespace std;
 
const int MAX_SIZE = 100;

struct Node {
    int data;
    Node* next;
};
 
Node* head3 = nullptr;
Node* tail3 = nullptr;
int count3 = 0;

void pushToThirdStack(int val) {
    Node* newNode = new Node();
    newNode->data = val;
    newNode->next = nullptr;
 
    if (head3 == nullptr) {
        head3 = newNode;
        tail3 = newNode;
    } else {
        tail3->next = newNode;
        tail3 = newNode;
    }
    count3++;
}

void displayThirdStack() {
    Node* temp = head3;
    cout << "(Bottom -> Top): ";
    while (temp != nullptr) {
        cout << temp->data;
        if (temp->next != nullptr) cout << " -> ";
        temp = temp->next;
    }
    cout << endl;
}

void sortThirdStack() {
    for (Node* i = head3; i != nullptr; i = i->next) {
        Node* minNode = i;
        for (Node* j = i->next; j != nullptr; j = j->next) {
            if (j->data < minNode->data) {
                minNode = j;
            }
        }
        if (minNode != i) {
            int temp = i->data;
            i->data = minNode->data;
            minNode->data = temp;
        }
    }
}
 
struct CircularQueue {
    int arr[MAX_SIZE];
    int front;
    int rear;
    int capacity;
    int size;
};
 
void initQueue(CircularQueue &q, int capacity) {
    q.front = 0;
    q.rear = -1;
    q.capacity = capacity;
    q.size = 0;
}
 
bool isFull(CircularQueue &q) {
    return q.size == q.capacity;
}
 
void enqueue(CircularQueue &q, int val) {
    if (isFull(q)) {
        cout << "Circular queue is full, cannot insert " << val << endl;
        return;
    }
    q.rear = (q.rear + 1) % q.capacity;
    q.arr[q.rear] = val;
    q.size++;
}
 
void displayQueue(CircularQueue &q) {
    if (q.size == 0) {
        cout << "Circular queue is empty." << endl;
        return;
    }
    cout << "Circular Queue (Front -> Rear): ";
    int index = q.front;
    for (int i = 0; i < q.size; i++) {
        cout << q.arr[index];
        if (i != q.size - 1) cout << " -> ";
        index = (index + 1) % q.capacity;
    }
    cout << endl;
}
 
int main() {
    int size1, size2;
    int stack1[MAX_SIZE], stack2[MAX_SIZE];

    cout << "Enter number of elements in Stack 1: ";
    cin >> size1;
    cout << "Enter Stack 1 elements (first = bottom, last = top):" << endl;
    for (int i = 0; i < size1; i++) {
        cout << "Element " << (i + 1) << ": ";
        cin >> stack1[i];
    }

    cout << "\nEnter number of elements in Stack 2: ";
    cin >> size2;
    cout << "Enter Stack 2 elements (first = bottom, last = top):" << endl;
    for (int i = 0; i < size2; i++) {
        cout << "Element " << (i + 1) << ": ";
        cin >> stack2[i];
    }

    int i = 0, j = 0;
    while (i < size1 && j < size2) {
        pushToThirdStack(stack1[i]);
        i++;
        pushToThirdStack(stack2[j]);
        j++;
    }
    while (i < size1) {
        pushToThirdStack(stack1[i]);
        i++;
    }
    while (j < size2) {
        pushToThirdStack(stack2[j]);
        j++;
    }
 
    cout << "\nThird Stack (created by alternating Stack1/Stack2, linked list):" << endl;
    displayThirdStack();
 
    sortThirdStack();
    cout << "\nThird Stack after sorting in ascending order:" << endl;
    displayThirdStack();
 
    CircularQueue cq;
    initQueue(cq, count3);
 
    Node* temp = head3;
    while (temp != nullptr) {
        enqueue(cq, temp->data);
        temp = temp->next;
    }
 
    cout << "\nCircular Queue built from sorted elements:" << endl;
    displayQueue(cq);
 
    return 0;
}