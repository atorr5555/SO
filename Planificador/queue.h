#ifndef queue_h
#define queue_h
#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
	int num_process;
	struct Node *next;
} Node; //Estructura de un nodo que cuenta con el numero de procesos

typedef struct Queue
{
	Node *head;
	Node *tail;
} Queue; //Estructura que cuenta con los nodos necesarios para la cola

Queue create_queue(); //Crea una cola
void print_queue(Queue); //Imprime la cola
void enqueue(Queue *, int); //Agrega un elemento a la cola
int dequeue(Queue *);  //Elimina el primer proceso en la cola.

Queue create_queue()
{
	Queue queue;
	queue.head = NULL;
	queue.tail = NULL;
	return queue;
}

void print_queue(Queue queue)
{
	printf("-------------------------------------------\n");
	printf("Cola de procesos\n");
	if (queue.head == NULL)
	{
		printf("La cola está vacía");
		return;
	}
	Node *current = queue.head;
	printf("<- ");
	while (current != NULL)
	{
		printf("%d, ", current->num_process);
		current = current->next;
	}
	printf("\n");
}

void enqueue(Queue *queue, int num_process)
{
	if (queue->head == NULL)
	{
		Node *node = (Node *)malloc(sizeof(Node));
		node->num_process = num_process;
		node->next = NULL;
		queue->head = node;
		queue->tail = node;
		return;
	}
	Node *new_node = (Node *)malloc(sizeof(Node));
	new_node->num_process = num_process;
	new_node->next = NULL;
	queue->tail->next = new_node;
	queue->tail = new_node;
}

int dequeue(Queue *queue)
{
	int num_process;
	if (queue->head == NULL)
	{
		return -1;
	}
	Node *new_head = NULL;
	Node *temp = queue->head;
	new_head = temp->next;
	num_process = temp->num_process;
	free(queue->head);
	queue->head = new_head;
	return num_process;
}
#endif
