#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "Queue.h"
// Implemeting Queue

//------------------------------------------------------------------------

/*
TODO: implement the initializeQueue - this function instantiates the Queue struct using malloc and sets the values for the fields.
It returns a pointer to the instantiated Queue
*/
Queue *initializeQueue(int n, char **array)
{
  // Instantiate the queue
  Queue *q = (Queue *)malloc(sizeof(Queue));
  q->array = array;
  q->size = n;
  return q;
}

//------------------------------------------------------------------------

/*
TODO: implement Queue destructor - this function frees memory allocated to Queue
*/
void destructQueue(Queue **q)
{
  free(*q);
}

//------------------------------------------------------------------------

/* TODO: implement the 'enqueue' functionality
This function takes in a queue pointer and a username and adds the username to queue
*/
void enqueue(Queue *q, char *name)
{
  // Resizes the queue
  q->array = (char **)realloc(q->array, (q->size + 1) * sizeof(char *));
  // Enqueue the string
  q->array[q->size] = strdup(name);
  q->size++;
}

//------------------------------------------------------------------------

/* TODO: implement the 'dequeue' functionality
This function takes in a queue pointer and removes element from head of the queue
Print "Empty Queue" if there's not element to remove
*/

void dequeue(Queue *q)
{
  // Shift all items one to the left by 1
  for (int i = 1; i < q->size; i++)
  {
    q->array[i - 1] = q->array[i];
  }
  // Resizes the queue
  q->array = (char **)realloc(q->array, (q->size - 1) * sizeof(char *));
  q->size--;
}

//------------------------------------------------------------------------

/* TODO: implement the 'front' functionality
This function takes in a queue pointer and returns the element at the front of the queue without removing it. If the Queue is empty, return NULL
*/

char *front(Queue *q)
{
  return q->array[0];
}

//------------------------------------------------------------------------

/* TODO: implement the printQueue
This function takes in a queue pointer and prints the contents of the queue in order (head to tail)
*/
void printQueue(Queue *q)
{
  for (int i = 0; i < q->size; i++)
  {
    printf("%s\n", q->array[i]);
  }
}

//------------------------------------------------------------------------

/* TODO: implement the count
This function takes in a queue pointer and returns the 'count' field of the queue
*/
int count(Queue *q)
{
  return q->size;
}
