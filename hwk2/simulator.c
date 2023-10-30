#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include "Queue.h"
#include "List.h"

typedef struct station
{
  char *name;
  int quantity;
  Queue *q;
} Station;

void printStationNotFoundError()
{
  printf("Error: Station not found.\n");
}

// Finds and returns a pointer to a Station given a station name
// takes the stationsList as input
Station *findStation(ListPtr stationsList, char *station)
{
  NodePtr cur = stationsList->head;
  while (cur != NULL)
  {
    Station *curStation = (Station *)cur->data;
    char *curName = curStation->name;

    // If a match is found, append to queue then break out of loop
    if (!strcmp(curName, station))
    {
      return curStation;
      break;
    }

    cur = cur->next;
  }
  return NULL;
}

void leaveStation(ListPtr stationsList, ListPtr peopleList, char *name, char *station)
{
  Station *foundStation = findStation(stationsList, station);
  if (foundStation == NULL)
  {
    printStationNotFoundError();
  }
  else
  {
    // Check front for name
    char *frontName = front(foundStation->q);
    if (strcmp(frontName, name))
    {
      dequeue(foundStation->q);
      appendList(peopleList, name);
    }
    else
    {
      printf("Error: %s is not at the front of the queue (or is not in the queue).", name);
    }
  }
}

// Prints station given a pointer to a station
void printStation(void *station)
{
  Station *s = station;
  printf("Station {name: %s, quantity: %d, q_count: %d}\n", s->name, s->quantity, count(s->q));
  // Print the station queue
  printQueue(s->q);
}

// Prints a string given a pointer to a string
void printString(void *str)
{
  char *s = str;       // "casting" void * to char *
  printf("%s    ", s); // print string starting from s
}

int main(int argc, char **argv)
{
  // code to read and process input

  // System Init Phase ======

  // Initialize the stations_list linked list
  ListPtr stationsList = createList(printStation);

  // 1. The first input is then number of stations
  int numStations;
  scanf("%d", &numStations);
  for (int i = 0; i < numStations; i++)
  {
    // 2. The station name and the quantity of food at that station

    // Input the name and the quantitiy
    char name[256];
    int quantity;

    scanf("%255s", name);
    scanf("%d", &quantity);

    // Initialize the queue and array to use with the station
    char **arr = (char **)malloc(0 * sizeof(char *));
    Queue *q = initializeQueue(0, arr);

    // Initialize the station
    Station *s = (Station *)malloc(sizeof(Station));
    s->q = q;
    s->quantity = quantity;
    s->name = strdup(name);

    // Add the station to the linked list of stations
    appendList(stationsList, s);
  }

  // 3. Read the input filename for people
  char peopleFilename[256];
  scanf("%255s", peopleFilename);

  // printList(stationsList);

  // Commands Phase ======

  // Initialization

  FILE *peopleFile;
  peopleFile = fopen(peopleFilename, "r");

  if (peopleFile == NULL)
  {
    // If fopen() returns NULL, an error occurred
    perror("Error opening file");
    return (-1);
  }

  ListPtr peopleList = createList(printString);

  int numPeople;
  fscanf(peopleFile, "%d", &numPeople);

  // Scan the person name and the station
  for (int i = 0; i < numPeople; i++)
  {
    char person[256];
    char station[256];
    fscanf(peopleFile, "%255s", person);
    fscanf(peopleFile, "%255s", station);

    // Find the matching station in the stationsList and add the person to
    // the queue

    bool found = false;
    NodePtr cur = stationsList->head;
    while (cur != NULL)
    {
      Station *curStation = (Station *)cur->data;
      char *curName = curStation->name;

      // If a match is found, append to queue then break out of loop
      if (!strcmp(curName, station))
      {
        enqueue(curStation->q, person);
        found = true;
        break;
      }

      cur = cur->next;
    }
    if (!found)
      printf("Station not found: %s\n", station);
  }

  // printList(stationsList);

  // Post Initialization

  while (true)
  {
    int inputCommand;
    int result = scanf("%d", &inputCommand);

    if (result == 0)
    {
      printf("Not a valid command, please enter a number from 1-7.\n");

      // Clear the input buffer
      while (getchar() != '\n')
        ;
      continue;
    }

    // Commands
    if (inputCommand == 1) // Join a station
    {
      // Command format: Alex Dessert
      char name[256];
      char station[256];
      scanf("%255s", name);
      scanf("%255s", station);

      Station *foundStation = findStation(stationsList, station);
      if (foundStation == NULL)
      {
        printStationNotFoundError();
      }
      else
      {
        enqueue(foundStation->q, name);
      }
    }
    else if (inputCommand == 2) // Restock a station
    {
      // Command format: Dessert 20
      int amt;
      char station[256];
      scanf("%255s", station);
      scanf("%d", &amt);

      Station *foundStation = findStation(stationsList, station);
      if (foundStation == NULL)
      {
        printStationNotFoundError();
      }
      else
      {
        foundStation->quantity = foundStation->quantity + amt;
      }
    }
    else if (inputCommand == 3) // Leave a station
    {
      // Command format: Dessert Alex
      char station[256];
      char name[256];
      scanf("%255s", station);
      scanf("%255s", name);

      leaveStation(stationsList, peopleList, name, station);
    }
    else if (inputCommand == 4) // Depart from the Dining hall
    {
      // Command format: Alex
      char name[256];
      scanf("%255s", name);

      // If in peopleList remove them, otherwise check fronts of queues and call leave
      // station if found.

      // Check if name is in the peopleList
      NodePtr cur = peopleList->head;
      bool found = false;
      int index = 0;
      while (cur != NULL)
      {
        char *curPerson = cur->data;
        if (strcmp(curPerson, name))
        {
          found = true;
          break;
        }
        cur = cur->next;
        index++;
      }

      if (found)
      {
        deleteList(peopleList, index);
      }
      else // check front of each queue
      {
        // If not found, delete from end after calling leaveStation
        // and finidng in queues
        NodePtr cur = stationsList->head;
        bool foundInQueues = false;
        while (cur != NULL)
        {
          Station *curStation = cur->data;
          char *frontQ = front(curStation->q);
          if (strcmp(frontQ, name))
          {
            leaveStation(stationsList, peopleList, name, curStation->name);
            found = true;
            break;
          }
          cur = cur->next;
        }

        if (foundInQueues)
        {
          deleteList(peopleList, peopleList->length - 1);
        }
        else
        {
          printf("Error: %s not found in queue list or in dining hall.", name);
        }
      }
    }
    else if (inputCommand == 5) // Serve Queue
    {
      // Command format: Dessert
      char station[256];
      scanf("%255s", station);

      Station *foundStation = findStation(stationsList, station);
      if (foundStation == NULL)
      {
        printStationNotFoundError();
      }
      else
      {

        NodePtr cur = peopleList->head;
        while (cur != NULL)
        {
          if (foundStation->quantity > 0)
          {
            leaveStation(stationsList, peopleList, (char *)cur->data, foundStation->name);
            foundStation->quantity = foundStation->quantity - 1;
            cur = peopleList->head;
          }
          else
          {
            printf("Apologies for the inconvenience. Please wait for the queue to be filled");
            break;
          }
        }
      }
    }
    else if (inputCommand == 6) // Write state
    {
      // Command format: filename
      char filename[256];
      scanf("%255s", filename);

      FILE *file;
      file = fopen(filename, "w");
      if (!file)
      {
        perror("Error opening file");
        return (-1);
      }

      NodePtr cur = stationsList->head;
      int index = 0;
      while (cur != NULL)
      {
        Queue *q = ((Station *)cur->data)->q;

        if (index > 0)
          fprintf(file, "\n\n");

        for (int i = 0; i < q->size; i++)
        {
          if (i > 0)
            fprintf(file, "\n");

          fprintf(file, "%s", q->array[i]);
        }

        cur = cur->next;
        index++;
      }

      fclose(file);
    }
    else if (inputCommand == 7)
    {
      break;
    }
    else
    {
      printf("Not a valid command, please enter a number from 1-7.\n");
    }
  }

  // code to free memory allocated to queue
  fclose(peopleFile);

  // Go through and free all arrays, queues, and stations
  NodePtr cur = stationsList->head;
  while (cur != NULL)
  {
    Station *s = cur->data;
    free(s->q->array);
    free(s->q);
    free(s->name);
    free(s);
    cur = cur->next;
  }

  destroyList(&stationsList);
  destroyList(&peopleList);
}