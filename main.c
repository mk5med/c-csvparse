#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
#include "main.h"
#include "./Libraries/padding.h"

typedef int bool;
static bool should_filter_columns;
static int no_index;

void print_help()
{
  printf("CSV_PARSER Help\nAvailable commands:\n");
  printf("\t-w, --col-width\t\tSet column width (-1 for default separation between columns). 10 is the default.\n");
  printf("\t-c, --columns\t\tA column index to display. The argument can also be called in a string format.\n\t\t\t\tIndexes start at 0 and must be in increasing order.\n");
  printf("\t-h, --help\t\tPrint this help menu.\n");
}

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

void parse_csv(char separator, int column_width, FILE *input_file, int *display_columns)
{
  char char_in;
  int row_count = 0;
  bool is_header_row = 1;
  bool is_comma_escaped = 0;
  int current_cell_length = 0;
  int current_column_index = 0;
  int display_columns_index = 0;

  while ((char_in = fgetc(input_file)) != EOF)
  {
    // If the current row is a header..
    if (is_header_row)
    {
      // Print a gap between the start of line and the expected start of the next column
      int len = 0;
      while (len++ <= column_width && no_index == 0)
        putc(separator, stdout);
      is_header_row = 0;
    }

    // If a quote is detected, mark the incoming string as an escaped string.
    // TODO: This only handles the case where strings in cells are denoted with double-quotes
    // If the variable was 0 (falsy; not within a string) set to 1 (truthy; within a string).
    // Otherwise set it to 0
    if (char_in == '"')
      is_comma_escaped = is_comma_escaped ? 0 : 1;

    // If the cell length is equal to the column width at this point, it means that the cell may have more information to insert.
    // Cycle-through the current cell data and move onto the next one.
    if (current_cell_length == column_width)
    {
      // Check if currently in a string by checking if the current character is a non-escaped comma
      int in_a_string = char_in == ',' && is_comma_escaped == 0;

      // Skip this character in the loop
      if (!in_a_string && char_in != '\n')
        continue;
    }

    int column_should_display = should_filter_columns == 0 || display_columns[display_columns_index] == current_column_index;

    // If a comma is detected and the context is not currently within a string,
    int is_column_end = char_in == ',' && !is_comma_escaped;
    if (is_column_end)
    {
      if (column_should_display)
      {
        display_columns_index++;

        while (current_cell_length < column_width + 1) // Pad + 1 for visual separation
        {
          putc(separator, stdout);
          current_cell_length++;
        }
      }

      current_cell_length = 0;
      current_column_index++;
      continue;
    }

    if (column_should_display && char_in != '\n')
    {
      // If the previous checks did not catch, print the character and increment cell length
      putc(char_in, stdout);
      current_cell_length++;
    }

    // Reached the end of the row
    if (char_in == '\n')
    {
      putc('\n', stdout);
      row_count++;
      current_cell_length = 0;
      is_comma_escaped = 0;
      current_column_index = 0;
      display_columns_index = 0;

      if (no_index == 0) {
        // Print the padded row row_count in preparation for the next input
        printf("%s", pad_int(row_count, column_width, separator));
      }
    }
  }

  printf("\n");
}