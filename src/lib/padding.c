#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "padding.h"

/**
 * @brief Right-pad an input string to length `width` and fill the characters
 * between `len(str)` and `width` with the `separator` character.
 * 
 * @param str 
 * @param width 
 * @param separator 
 */
void pad_string__mutate(char *str, int width, char separator)
{
  int length = strlen(str);
  str = realloc(str, width * sizeof(char)); // if width < length this will truncate the string
  while (length <= width)
  {
    str[length] = separator;
    length++;
  }
}

/**
 * @brief Convert an int to a string and apply padding to the right
 * 
 * @param val 
 * @param width 
 * @param separator 
 * @return char* 
 */
char *pad_int__alloc(int val, int width, char separator)
{
  width++; // Increment width to account for null end character
  char *buffer = malloc(sizeof(char) * (width)); // Allocate a new string/char-array
  snprintf(buffer, width, "%d", val); // Save the number into the string

  if (strnlen(buffer, width) < width) // If the width is not exceeded
    pad_string__mutate(buffer, width - 1, separator); // Pad the string until the end

  return buffer;
}