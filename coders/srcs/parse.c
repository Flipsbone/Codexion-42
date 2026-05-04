/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 10:12:52 by advacher          #+#    #+#             */
/*   Updated: 2026/04/28 10:42:32 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "prototype.h"
#include <stdio.h>
#include <string.h>

static void	ft_fill_data(t_data *data, int *tmp_args, char *scheduler)
{
	data->number_of_coders = tmp_args[0];
	data->time_to_burnout = tmp_args[1];
	data->time_to_compile = tmp_args[2];
	data->time_to_debug = tmp_args[3];
	data->time_to_refactor = tmp_args[4];
	data->number_of_compiles_required = tmp_args[5];
	data->dongle_cooldown = tmp_args[6];
	data->scheduler = scheduler;
}

static int	ft_check_coders(int coders)
{
	if (coders == 0)
	{
		fprintf(stderr, "Error number_of_coders must be : > 0 \n");
		return (-1);
	}
	return (0);
}

static int	ft_str_to_numeric(const char *str)
{
	long	result;
	size_t	i;

	i = 0;
	result = 0;
	while (str[i])
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		result = result * 10 + (str[i] - '0');
		if (result > 2147483647)
			return (-1);
		i++;
	}
	return (result);
}

static int	ft_check_numb_arguments(int ac, char **av)
{
	(void)av;
	if (ac != 9)
	{
		fprintf(stderr,
			"Error args must be :\n number_of_coders "
			"time_to_burnout time_to_compile "
			"time_to_debug time_to_refactor number_of_compiles_required "
			"dongle_cooldown scheduler\n");
		return (-1);
	}
	return (0);
}

int	ft_parse_arguments(int ac, char **av, t_data *data)
{
	int	i;
	int	tmp_args[7];

	if (ft_check_numb_arguments(ac, av) != 0)
		return (-1);
	i = 1;
	while (av[i] && i <= 7)
	{
		tmp_args[i - 1] = ft_str_to_numeric(av[i]);
		if (tmp_args[i - 1] == -1)
		{
			fprintf(stderr, "Error args must be positif integer\n");
			return (-1);
		}
		i++;
	}
	if (ft_check_coders(tmp_args[0]) == -1)
		return (-1);
	if (strcmp(av[8], "fifo") != 0 && strcmp(av[8], "edf") != 0)
	{
		fprintf(stderr, "Error must be : edf, fifo\n");
		return (-1);
	}
	ft_fill_data(data, tmp_args, av[8]);
	return (0);
}
