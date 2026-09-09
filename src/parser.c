/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   parser.c                                        \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/09 14:45:29 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/09 16:41:27 by odschreu            ..+::##::+..         */
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

static int	ft_strncmp(const char *s1, const char *s2, int n)
{
	if (n == 0)
		return (0);
	while (*s1 && *s2 && n > 1 && *s1 == *s2)
	{
		s1++;
		s2++;
		n--;
	}
	return (unsigned char *)s1 == (unsigned char *)s2;
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

	i = 0;
	sign = false;
	start = NULL;
	while (*str)
	{
		if (*str == '-')
			error_exit("No negative integers allowed.");
		else if (*str == '+' && sign++)
				error_exit("No double signs in arguments are allowed.");
		else if (*str == '.')
			error_exit("Only integers are allowed, no floats.");
		else if (*str >= '0' && *str <= '9')
			if (!start)
				start = str;
			i++;
		str++;
	}
	if (i > 10 || (i == 10 && ft_strncmp(start, "2147483647", 10) > 0))
		error_exit("All numeric arguments need to be <= INT_MAX.");
	return start;
}

static long	ft_atol(char *str)
{
	long	num;
	const char	*start;
	
	while (is_space(*str))
		str++;
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
	if (strcmp(av[8], "fifo") != 0 && strcmp(av[8], "efi") != 0)
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
