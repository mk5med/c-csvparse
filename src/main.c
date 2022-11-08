#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "main.h"
#include "./lib/padding.h"

typedef int bool;
static bool should_filter_columns;
static int no_index;

void print_help();
void parse_csv(char separator, int column_width, FILE *input_file, int *display_columns);

int main(int argc, char **argv)
{
  int column_width = 10;
  char separator = ' ';

  int column_count = 0;
  int max_column_count = 10;
  int *display_columns = malloc(max_column_count * sizeof(int));

  static struct option longoptions[] = {
      {"col-width", required_argument, 0, 'w'},
      {"columns", required_argument, 0, 'c'},
      {"help", no_argument, 0, 'h'},
      {"no-index", no_argument, &no_index, 1},
  };

  char incoming_flag = 0;
  int option_index = 0;
  while ((incoming_flag = getopt_long(argc, argv, "w:c:h", longoptions, &option_index)) != -1)
  {
    switch (incoming_flag)
    {
    case 'w':
      column_width = atoi(optarg);
      break;
    case 'c':
    {
      if (should_filter_columns == 0)
        should_filter_columns = 1;

      char *optionArgumentCharacter = optarg;
      while (*optionArgumentCharacter)
      {
        // If the current character is a space, skip it.
        // This works to remove empty spaces between strings of multiple numbers
        if (isspace(*optionArgumentCharacter))
        {
          optionArgumentCharacter++;
          continue;
        }

        // If the size exceeds the allocated size, request a larger memory size
        if (column_count >= max_column_count)
        {
          max_column_count *= 2;
          display_columns = realloc(display_columns, sizeof(int) * max_column_count);
        }

        display_columns[column_count] = atoi(optionArgumentCharacter);

        // Ensure that the read value is larger than the previous value
        if (column_count != 0 && display_columns[column_count] <= display_columns[column_count - 1])
        {
          printf("Column arguments should be in ascending order.\n");
          exit(1);
        }

        // After a character has been read-in, move the pointer forwards until it does not point to a non-space value
        while (*optionArgumentCharacter != '\0' && !isspace(*optionArgumentCharacter))
          optionArgumentCharacter++;

        // The column index has been saved
        column_count++;
      }
    }
    break;
    case 'h':
      print_help();
      exit(0);
      break;
    case '?':
      printf("Incorrect arguments. %d\n", incoming_flag);
      exit(1);
    }
  }

  parse_csv(separator, column_width, stdin, display_columns);
}