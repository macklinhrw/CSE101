#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void printString(void *str)
{
  char *s = str;       // "casting" void * to char *
  printf("%s    ", s); // print string starting from s
}

// Formats the input List, L, by ammount amt.
// then outputs a file with the formatted text.
void formatLeft(ListPtr L, char *filename, int amt)
{
  // output file name =  `filename`.L`amt`
  NodeObj *cur = L->head;
  NodeObj *next = cur->next;

  int curLength = 0;
  while (cur != NULL)
  {
    if (next == NULL)
    {
      printf("%s ", cur->data);
      break;
    }

    curLength += strlen(cur->data) + 1; // add one to account for space
    int nextLength = curLength + strlen(next->data);
    if (nextLength > amt)
    {
      curLength = 0;
      printf("%s\n", cur->data);
    }
    else
    {
      printf("%s ", cur->data);
    }

    cur = next;
    next = cur->next;
  }

  printf("\n");
}

// Formats the input List, L, by ammount amt.
// then outputs a file with the formatted text.
void formatRight(ListPtr L, char *filename, int amt)
{

  if (L == NULL || L->head == NULL)
  {
    return; // Handle empty list scenario
  }

  // output file name =  `filename`.L`amt`
  // NodeObj *next = cur->next;

  // char line[amt];
  // line[0] = '\0';

  // int curLength = 0;

  NodeObj *current = L->head;
  char line[1024] = "";
  int lineLength = 0;
  while (current != NULL)
  {
    char *word = current->data;
    int wordLength = strlen(word);

    if (lineLength + wordLength + (lineLength > 0 ? 1 : 0) > amt)
    {
      int padding = amt - lineLength;

      printf("%*s\n", amt, line); // Right-justify and print

      strcpy(line, word);
      lineLength = wordLength;
    }
    else
    {
      if (lineLength > 0)
      {
        strcat(line, " ");
        lineLength++;
      }
      strcat(line, word);
      lineLength += wordLength;
    }

    current = current->next;

    // if (next == NULL)
    // {
    //   printf("%s\n", cur->data);
    //   break;
    // }

    // curLength += strlen(cur->data) + 1; // add one to account for space
    // int nextLength = curLength + strlen(next->data) + 1;
    // if (nextLength > amt)
    // {
    //   int numWhitespaces = amt - curLength;

    //   char whitespaces[numWhitespaces];
    //   whitespaces[0] = '\0';

    //   for (int count = 0; count < numWhitespaces; count++)
    //   {
    //     char whitespace[] = " ";
    //     strcat(whitespaces, whitespace);
    //   }

    //   char finalLine[amt];
    //   finalLine[0] = '\0';

    //   strcat(finalLine, whitespaces);
    //   strcat(finalLine, line);

    //   printf("%s\n", finalLine);
    //   line[0] = '\0';

    //   curLength = 0;
    // }
    // else
    // {
    //   char *word = cur->data;
    //   char whitespace[] = " ";
    //   strcat(word, whitespace);
    //   strcat(line, word);
    // }

    // cur = next;
    // next = cur->next;
  }

  // Print the last line if it's not empty
  if (lineLength > 0)
  {
    printf("%*s\n", amt, line);
  }

  printf("\n");
}

int main(int argc, char **argv)
{

  // The first command is the input file name; at least one line of text which
  // is this
  char *fileName;
  scanf("%s", fileName);

  FILE *file;
  file = fopen(fileName, "r");

  if (file == NULL)
  {
    // If fopen() returns NULL, an error occurred
    perror("Error opening file");
    return (-1);
  }

  ListPtr L = createList(printString);

  char word[256];
  while (fscanf(file, "%255s", word) == 1)
  {
    printf("Word: %s\n", word);

    char *tempWord = (char *)malloc(sizeof(char) * 256);
    strcpy(tempWord, word);
    appendList(L, tempWord);
  }

  printList(L);
  printf("\n\n");
  formatRight(L, "test", 50);

  fclose(file);

  while (true)
  {
    char *command;
    // Base commands
    char exitCommand[] = "exit";
    char quitCommand[] = "quit";
    // Formatting commands and arguments
    char formatCommand[] = "format";
    char formatLarg[] = "L";
    char formatRarg[] = "R";
    // char formatCarg[] = "C";

    scanf("%s", command);

    // Exit the loop
    if (!strcmp(command, exitCommand) || !strcmp(command, quitCommand))
    {
      break;
    }
    else if (!strcmp(command, formatCommand)) // command with secondary arguments
    {
      int formatAmt;
      char *arg;
      scanf("%s", arg);
      if (!strcmp(arg, formatLarg)) // i.e., format L
      {
        scanf("%d", &formatAmt);
      }
      else if (!strcmp(arg, formatRarg)) // i.e., format R
      {
        scanf("%d", &formatAmt);
      }
      else // invalid argument, print error
      {
        printf("Invalid command '%s'. Please use the 'format <Justification> <Width>' format, such as 'format L 50', or exit using 'quit'/'exit'.\n", arg);
      }
    }
    else
    {
      printf("Invalid command '%s'. Please use the 'format <Justification> <Width>' format, such as 'format L 50', or exit using 'quit'/'exit'.\n", command);
    }
  }

  destroyList(&L);

  // read input and create a list of "words"

  // keep asking for user command to process
  // Note:  you must use your List ADT functions to process format commands

  // be sure to release memory
}
