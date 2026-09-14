/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   main.c                                          \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/09 13:17:42 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/10 17:57:29 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"


int	main(int ac, char **av)
{
  t_data	data_table;
  char		*exit_message;

  if (ac == 9)
  {
    // 1 parsing input
    parse_input(&data_table, av);
    // 2 creating data table and coders and dongles
    codexion_init(&data_table);
    // 3 starting compiling simulation
	start_codexion(&data_table);
    // 4 cleaning everything up (no leaks)
    // clean_up(&data_table);
  }
  else
  {
	  exit_message = "Error: incorrect number of arguments.\n\n"
            "Usage: ./codexion <number_of_coders> <time_to_burnout> <time_to_compile> "
            "<time_to_debug> <time_to_refactor> <number_of_compiles_required> "
            "<dongle_cooldown> <scheduler>\n\n"
			"NOTE: all arguments except for 'scheduler' need to be positive integers\n"
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
            "  scheduler                    str    \"fifo\" or \"edf\"\n";
	  error_exit(exit_message);
  }
  return (0);
}
