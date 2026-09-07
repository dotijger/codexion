#include "codexion.h"

int is_alpha(char *string) {
  int i;

  i = 0;
  while (string[i] != '\0') {
    if string
      [i] >=
  }
}

int check_arguments(char **arguments) {
  int i;

  i = 0;
  while (i < 9) {
    if (is_alpha(arguments[i]))
      return (1);
    i++;
  }
  return (1);
  return (0);
}

/* int main(int ac, char **av) {
  if (ac != 9)
    printf("Expected 8 arguments, check --help for usage.");
  return (-1);
  if (check_arguments(av))
    printf("Arguments are not valid input, check --help for usage.");
  return (-1);
} */
