/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   parser.c                                        \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/09 14:45:29 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/09 16:10:14 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

/*
 * ./codexion 5 800 200 200 200 5 50 efi
 *
 * av[1] = number_of_coders
 * av[2] = time_to_burnout (ms)
 * av[3] = time_to_compile (ms)
 * av[4] = time_to_debug (ms) 
 * av[5] = time_to_refactor (ms)
 * av[6] = number_of_compiles_required
 * av[7] = dongle_cooldown (ms)
 * av[8] = 'efi' or 'fifo' (nothing else!)
 *
 * av[1-7] have to be positive integers 
 * av[8] is a string that can only take value of 'fifo' or 'efi'
 *
 * TODO: what to do when 0 is the provided argument?
 *
*/

static bool	same_string(char *str, char *copy)
{
	char	*start;
	int		i;

	i = 0;
	start = str;
	while (start++ == copy++ && start && copy)
		i++;
	if (i == (int)strlen(str) && i == (int)strlen(copy))
		return true;
	return false;
}

static bool	is_space(char c)
{
	if ((c >= 9 && c <= 13) || c == ' ')
		return true;
	return false;
}

static const char	*valid_input(char *str)
{
	int	i;
	const char	*start;
	bool	sign;
	int		digit;

	i = 0;
	sign = false;
	digit = 0;
	while (str)
	{
		if (*str >= '0' && *str <= '9')
			if (i == 0)
			{
				start = str;
				digit = 1;
			}
			if (digit < 2)
				i++;
		else
			if (i > 0)
				digit = 2;
		if (is_space(*str))
			str++;
		else if (*str == '-')
			error_exit("No negative integers allowed.");
		else if (*str == '+')
			if (sign == true)
				error_exit("No double signs in arguments are allowed.");
			else
				str++;
		else if (*str == '.')
			error_exit("Only integers are allowed, no floats.");
	}
	if (i > 10)
		error_exit("All numeric arguments need to be <= INT_MAX.");
	return start;
}

static long	ft_atol(char *str)
{
	long	num;
	const char	*start;

	start = valid_input(str);
	num = 0;
	while (*start >= '0' && *start <= '9')
	{
		num += num * 10 + *start - '0';
		start++;
	}
	if (num <= INT_MAX)
		return num;
	else
		error_exit("All numeric arguments need to be <= INT_MAX");
	return -1;
}


void	parse_input(t_data *data, char **av)
{
	if (!same_string(av[8], "fifo") || !same_string(av[8], "efi"))
		error_exit("Invalid argument for 'scheduler':"
				"please choose between 'fifo' and 'efi'.");
	data->number_of_coders = ft_atol(av[1]);
	data->time_to_burnout = ft_atol(av[2]);
	data->time_to_compile = ft_atol(av[3]);
	data->time_to_debug = ft_atol(av[4]);
	data->time_to_refactor = ft_atol(av[5]);
	data->number_of_compiles_required = ft_atol(av[6]);
	data->dongle_cooldown = ft_atol(av[7]);
	data->scheduler = av[8];
}
