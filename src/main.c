/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   main.c                                          \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/09 13:17:42 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/22 11:19:44 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

static void	print_exit_message(void)
{
	char	*exit_message;

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
	  fprintf(stderr, "%s", exit_message);
}

int	main(int ac, char **av)
{
  t_data	data_table;

  if (ac == 9)
  {
	memset(&data_table, 0, sizeof(t_data));
    if (parse_input(&data_table, av))
		return (1);
    if (codexion_init(&data_table))
	{
		clean_up(&data_table);
		return (1);
	}
	if (codexion(&data_table))
	{
		clean_up(&data_table);
		return (1);
	}
    clean_up(&data_table);
  }
  else
  {
	  print_exit_message();
	  return (1);
  }
  return (0);
}
