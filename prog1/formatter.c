#include "List.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Helper constants for allocating string length
#define FILENAME_LEN 256
#define MAX_LINE_LEN 1024

void printString(void *str)
{
  char *s = str;       // "casting" void * to char *
  printf("%s    ", s); // print string starting from s
}

// Returns a filename string without the extension, as well
// as in the correct format for the assignment.
// The filename will need to be freed since we are using malloc.
// @param filename The filename with extension
// @param amt The line length
// @param label The label signifying the type of justification
char *createOutputFilename(char *filename, int amt, char *label)
{
  // strrchr finds the last occurence of a character in a string
  const char *dot = strrchr(filename, '.');
  if (!dot) // If no dot was found, return the input filename
  {
    return filename;
  }

  // Create a new string without the extension
  int lengthWithoutExtension = dot - filename;
  char baseFilename[lengthWithoutExtension + 1];

  strncpy(baseFilename, filename, lengthWithoutExtension);
  baseFilename[lengthWithoutExtension] = '\0';

  // Create the output filename and return
  char *outputFilename = (char *)malloc(sizeof(char) * (FILENAME_LEN));
  // Needed to add one here since adding a '.' was cutting off the last digit?
  snprintf(outputFilename, sizeof(outputFilename) + 1, "%s.%s%d", baseFilename, label, amt);

  // Will need to remember to free the filename since we are using malloc above.
  return outputFilename;
}

// Prints the left justified words list to a file
// @param filename The filename to save to
// @param L The list of words to format
// @param amt The line length
void formatLeft(ListPtr L, char *filename, int amt)
{
  if (L == NULL || L->head == NULL)
  {
    return; // Handle empty list
  }

  // Creating the output file name
  char *outputFilename = createOutputFilename(filename, amt, "L");

  FILE *file = fopen(outputFilename, "w");
  if (!file)
  {
    perror("Error opening file");
    return;
  }

  // output file name =  `filename`.L`amt`
  NodeObj *cur = L->head;
  NodeObj *next = cur->next;

  int curLength = 0;
  while (cur != NULL)
  {
    if (next == NULL)
    {
      fprintf(file, "%s ", cur->data);
      break;
    }

    curLength += strlen(cur->data) + 1; // add one to account for space
    int nextLength = curLength + strlen(next->data);
    if (nextLength > amt)
    {
      curLength = 0;
      fprintf(file, "%s\n", cur->data);
    }
    else
    {
      fprintf(file, "%s ", cur->data);
    }

    cur = next;
    next = cur->next;
  }

  free(outputFilename);
}

// Prints the right justified words list to a file
// @param filename The filename to save to
// @param L The list of words to format
// @param amt The line length
void formatRight(ListPtr L, char *filename, int amt)
{

  if (L == NULL || L->head == NULL)
  {
    return; // Handle empty list
  }

  // Creating the output file name
  char *outputFilename = createOutputFilename(filename, amt, "R");

  FILE *file = fopen(outputFilename, "w");
  if (!file)
  {
    perror("Error opening file");
    return;
  }

  NodeObj *current = L->head;
  char line[MAX_LINE_LEN] = "";
  int lineLength = 0;
  while (current != NULL)
  {
    char *word = current->data;
    int wordLength = strlen(word);

    // Check if adding the new word exceeds the limit
    if (lineLength + wordLength + (lineLength > 0 ? 1 : 0) > amt)
    {
      // Right justify and print the current line because the new word doesn't fit
      fprintf(file, "%*s\n", amt, line); // Right-justify and print

      // Start a new line with the current word
      strcpy(line, word);
      lineLength = wordLength;
    }
    else
    {
      // Add the current word to the line
      if (lineLength > 0)
      {
        // Add a space before the word if it's not the start of the line
        strcat(line, " ");
        lineLength++;
      }
      strcat(line, word);
      lineLength += wordLength;
    }

    current = current->next;
  }

  // Print the last line if it's not empty
  if (lineLength > 0)
  {
    fprintf(file, "%*s", amt, line);
  }

  free(outputFilename);
}

// Prints the center justified words list to a file
// @param filename The filename to save to
// @param L The list of words to format
// @param amt The line length
void formatCenter(ListPtr L, char *filename, int amt)
{

  if (L == NULL || L->head == NULL)
  {
    return; // Handle empty list
  }

  // Creating the output file name
  char *outputFilename = createOutputFilename(filename, amt, "C");

  FILE *file = fopen(outputFilename, "w");
  if (!file)
  {
    perror("Error opening file");
    return;
  }

  NodeObj *current = L->head;
  char line[MAX_LINE_LEN] = "";
  int lineLength = 0;
  while (current != NULL)
  {
    char *word = current->data;
    int wordLength = strlen(word);

    // Check if adding the new word exceeds the limit
    if (lineLength + wordLength + (lineLength > 0 ? 1 : 0) > amt)
    {
      // Center justify and print the current line because the new word doesn't fit
      int padding = (amt - lineLength) / 2;
      fprintf(file, "%*s%s\n", padding, " ", line);

      // Start a new line with the current word
      strcpy(line, word);
      lineLength = wordLength;
    }
    else
    {
      // Add the current word to the line
      if (lineLength > 0)
      {
        // Add a space before the word if it's not the start of the line
        strcat(line, " ");
        lineLength++;
      }
      strcat(line, word);
      lineLength += wordLength;
    }

    current = current->next;
  }

  // Print the last line if it's not empty
  if (lineLength > 0)
  {
    int padding = (amt - lineLength) / 2;
    fprintf(file, "%*s%s", padding, " ", line);
  }

  free(outputFilename);
}

int main(int argc, char **argv)
{

  // The first command is the input file name; at least one line of text
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
    char *tempWord = (char *)malloc(sizeof(char) * 256);
    strcpy(tempWord, word);
    appendList(L, tempWord);
  }

  fclose(file);

  // CLI command loop
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
    char formatCarg[] = "C";

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
        formatLeft(L, fileName, formatAmt);
      }
      else if (!strcmp(arg, formatRarg)) // i.e., format R
      {
        scanf("%d", &formatAmt);
        formatRight(L, fileName, formatAmt);
      }
      else if (!strcmp(arg, formatCarg)) // i.e., format C
      {
        scanf("%d", &formatAmt);
        formatCenter(L, fileName, formatAmt);
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

  // Free the memory for the list
  destroyList(&L);
}
