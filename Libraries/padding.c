#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "padding.h"

void pad_string(char *str, int width, char separator)
{
  int length = strlen(str);
  str = realloc(str, width * sizeof(char));
  while (length <= width)
  {
    str[length] = separator;
    length++;
  }
}

char *pad_int(int val, int width, char separator)
{
  // Increment width to account for null end character
  width++;
  char *buffer = malloc(sizeof(char) * (width));
  snprintf(buffer, width, "%d", val);

  if (strnlen(buffer, width) < width)
    pad_string(buffer, width - 1, separator);

  return buffer;
}