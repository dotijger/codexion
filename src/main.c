#include "codexion.h"


void	parse_input(char **av)
{
	int	i;

	i = 1;
	while (i <= 8)
	{

		i++;
	}

}


int	main(int ac, char **av) {
  char	*name;
  t_data	data_table;

  name = "codexion";

  if (ac == 9)
  {
    // 1 parsing input
    parse_input(av);
    // 2 creating data table
    // create_data_table(&data_table);
    // 3 starting compiling simulation
    // start_coding(&data_table);
    // 4 cleaning everything up (no leaks)
    // total_free(&data_table);
  }
  else
  {
	  fprintf(stderr,
            "Error: incorrect number of arguments.\n\n"
            "Usage: %s <number_of_coders> <time_to_burnout> <time_to_compile> "
            "<time_to_debug> <time_to_refactor> <number_of_compiles_required> "
            "<dongle_cooldown> <scheduler>\n\n"
            "  number_of_coders             int    number of coders / dongles\n"
            "  time_to_burnout              int    ms before an idle coder "
            "burns out\n"
            "  time_to_compile              int    ms spent compiling\n"
            "  time_to_debug                int    ms spent debugging\n"
            "  time_to_refactor             int    ms spent refactoring\n"
            "  number_of_compiles_required  int    stop once every coder "
            "reaches this count\n"
            "  dongle_cooldown              int    ms a dongle stays unusable "
            "after release\n"
            "  scheduler                    str    \"fifo\" or \"edf\"\n",
            name);
	  exit(EXIT_FAILURE);
  }
  return (0);
}
