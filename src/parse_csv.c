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
        printf("%s", pad_int__alloc(row_count, column_width, separator));
      }
    }
  }

  printf("\n");
}