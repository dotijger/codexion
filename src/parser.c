/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: otc <otc@student.42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/09 14:45:29 by odschreu          #+#    #+#             */
/*   Updated: 2026/09/26 15:56:30 by otc              ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <string.h>

static int	valid_int(char *str)
{
	int		i;
	char	*start;

	i = 0;
	while (*str)
	{
		if (*str >= '0' && *str <= '9')
		{
			if (i == 0)
				start = str;
			i++;
		}
		str++;
	}
	if (i == 0)
		return (fail("Argument must contain digits.\n"));
	if (i > 10 || (i == 10 && ft_strncmp(start, "2147483647", 10) > 0))
		return (fail("All numeric arguments need to be <= INT_MAX.\n"));
	return (0);
}

static int	valid_input(char *str)
{
	int	sign;

	sign = 0;
	while (*str)
	{
		if (*str == '-')
			return (fail("No negative integers allowed.\n"));
		else if (*str == '+' && sign++)
			return (fail("No double signs in arguments are allowed.\n"));
		else if (*str == '.')
			return (fail("Only integers are allowed, no floats.\n"));
		str++;
	}
	return (0);
}

static int	check_arguments(char **av)
{
	int	i;

	i = 0;
	while (++i < 8)
	{
		if (valid_input(av[i]))
			return (1);
		if (valid_int(av[i]))
			return (1);
	}
	return (0);
}

static int	check_parsed_arguments(t_data *data_table)
{
	if (data_table->number_of_coders == 0)
		return (fail("Number of coders has to be greater than 0.\n"));
	if (data_table->time_to_burnout == 0)
		return (fail("Time to burnout has to be greater than 0.\n"));
	if (data_table->time_to_compile == 0)
		return (fail("Time to compile has to be greater than 0.\n"));
	if (data_table->time_to_debug == 0)
		return (fail("Time to debug has to be greater than 0.\n"));
	if (data_table->time_to_refactor == 0)
		return (fail("Time to refactor has to be greater than 0.\n"));
	return (0);
}

int	parse_input(t_data *data_table, char **av)
{
	if (strcmp(av[8], "fifo") != 0 && strcmp(av[8], "edf") != 0)
		return (fail(
				"Invalid argument for 'scheduler': "
				"please choose between 'fifo' and 'edf'."));
	if (check_arguments(av))
		return (1);
	data_table->number_of_coders = (long)atoi(av[1]);
	data_table->time_to_burnout = (long)atoi(av[2]);
	data_table->time_to_compile = (long)atoi(av[3]);
	data_table->time_to_debug = (long)atoi(av[4]);
	data_table->time_to_refactor = (long)atoi(av[5]);
	data_table->compiles_required = (long)atoi(av[6]);
	data_table->dongle_cooldown = (long)atoi(av[7]);
	if (strcmp(av[8], "fifo") == 0)
		data_table->scheduler = FIFO;
	else
		data_table->scheduler = EDF;
	if (check_parsed_arguments(data_table))
		return (1);
	return (0);
}
