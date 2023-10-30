#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"
#include <assert.h>

ListPtr createList(void (*dataPrinter)(void *data))
{
  // Allocate memory for the list object
  // It's a pointer to the space for the new ListObj; ListPtr
  ListPtr newList = (ListPtr)malloc(sizeof(ListObj));

  // Initialize the ListObj fields
  newList->dataPrinter = dataPrinter;
  newList->head = NULL;
  newList->length = 0;

  return newList;
}

void destroyList(ListPtr *pL)
{
  // If the pointer is not pointing to a list
  // or the list it is pointing to is null
  if (pL == NULL || *pL == NULL)
  {
    return;
  }

  // Iterate over list nodes and free the nodes with a temporary pointer
  ListPtr L = *pL;
  NodeObj *ptr = L->head;
  while (ptr != NULL)
  {
    NodeObj *toFree = ptr;
    ptr = ptr->next;
    free(toFree);
  }
  free(L);
  *pL = NULL;
}

int lengthList(ListPtr L)
{
  if (L == NULL)
  {
    return 0;
  }

  return L->length;
}

void printList(ListPtr L)
{
  if (L == NULL)
  {
    return;
  }

  // Iterate over list nodes
  NodeObj *ptr = L->head;
  while (ptr != NULL)
  {
    L->dataPrinter(ptr->data);
    ptr = ptr->next;
  }
  printf("\n");
}

void *getList(ListPtr L, int i)
{
  // 1. check if list is valid and not empty
  if (L == NULL)
  {
    return NULL;
  }
  // 2. index out of range
  if (i >= L->length || i < 0)
  {
    return NULL; // return error state, null in our case
  }
  // 3. set pointer to 1, then count starting at 1 till i-1
  NodeObj *ptr = L->head;
  for (int count = 0; count < i; count++)
  {
    ptr = ptr->next;
  }

  return ptr->data;
}

void *deleteList(ListPtr L, int i)
{
  // 1. check if list is valid and not empty
  if (L == NULL)
  {
    return NULL;
  }

  // 2. check if index out of range
  if (i >= L->length || i < 0)
  {
    return NULL; // return error state, null in our case
  }

  NodeObj *ptr = L->head;

  // Add if/else check for i=0 edge case (the node to delete is the head of the list)
  if (i == 0 && L->length == 1)
  {
    void *ret = ptr->data;
    free(ptr);
    L->head = NULL;
    L->length = 0;
    return ret;
  }
  else
  {
    // 3. set pointer to 1, then count starting at 1 till i-1
    for (int count = 1; count < i; count++)
    {
      ptr = ptr->next;
    }

    void *ret = ptr->data;
    if (ptr->next != NULL)
    {
      NodeObj *toFree = ptr->next;
      ptr->next = ptr->next->next;
      free(toFree);
      L->length--;
    }

    return ret;
  }
}

bool appendList(ListPtr L, void *data)
{
  // if list is not initialized or has no head, fail
  if (L == NULL)
  {
    return false;
  }

  // Instantiate new node
  NodeObj *newNode = (NodeObj *)malloc(sizeof(NodeObj));
  newNode->data = data;
  newNode->next = NULL;

  // Add case for if the list is empty, append as list head then return early
  if (L->head == NULL)
  {
    L->head = newNode;
    L->length = 1;
    return true;
  }

  // Find the last node
  NodeObj *ptr = L->head;
  while (ptr->next != NULL)
  {
    ptr = ptr->next;
  }

  // Append new node to the end, update length
  ptr->next = newNode;
  L->length++;

  return true;
}