#ifndef queue
#include<stdio.h>
#include<stdlib.h>

typedef struct Node{
    int num_proceso;
    struct Node *next;
}Node;

typedef struct Queue{
	Node *head;
	Node *tail;
}Queue;

Queue createQueue();
void printQueue(Queue);
void enqueue(Queue*, int);
int dequeue(Queue*, int);

Queue createQueue(){
	Queue queue;
	queue.head = NULL;
	queue.tail = NULL;
	return queue;
}

void printQueue(Queue queue) {
	if (queue.head == NULL) {
		printf("La cola está vacía");
		return;
	}
	Node *current = queue.head;
	while (current != NULL) {
		printf("%d ", current->num_proceso);
		current = current->next;
	}
}

void enqueue(Queue *queue, int num_proceso) {
	if (queue->head == NULL) {
		Node *node = (Node*)malloc(sizeof(Node));
		node->num_proceso = num_proceso;
		node->next = NULL;
		queue->head = node;
		queue->tail = node;
		return;
	}
	Node *new_node = (Node*)malloc(sizeof(Node));
	new_node->num_proceso = num_proceso;
	new_node->next = NULL;
	queue->tail->next = new_node;
	queue->tail = new_node;
}

int dequeue(Queue *queue) {
	int num_proceso;
	if (queue->head == NULL) {
		return -1;
	}
	Node *new_head = NULL;
	Node *temp = queue->head;
	new_head = temp->next;
	num_proceso = temp->num_proceso;
	free(queue->head);
	queue->head = new_head;
	return num_proceso;
}

#define queue
#endif