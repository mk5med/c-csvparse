#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <getopt.h>
void print_help()
{
  printf("CSV_PARSER Help\nAvailable commands:\n");
  printf("\t-w, --col-width\t\tSet column width (-1 for default separation between columns). 10 is the default.\n");
  printf("\t-c, --columns\t\tA column index to display. The argument can also be called in a string format.\n\t\t\t\tIndexes start at 0 and must be in increasing order.\n");
  printf("\t-h, --help\t\tPrint this help menu.\n");
}