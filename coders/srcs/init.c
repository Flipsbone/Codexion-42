/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:24:56 by advacher          #+#    #+#             */
/*   Updated: 2026/04/28 15:32:14 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	ft_init_print(t_data *data)
{
	if (pthread_mutex_init(&data->print_mutex, NULL) != 0)
	{
		fprintf(stderr, "Error init print_mutex\n");
		ft_release_simulation(data);
		return (-1);
	}
	return (0);
}

static int	ft_init_simulation(t_data *data)
{
	data->is_ready = 0;
	data->start_time = 0;
	data->stop_simulation = SIM_RUNNING;
	if (pthread_mutex_init(&data->sim_mutex, NULL) != 0)
	{
		fprintf(stderr, "Error init simulation mutex\n");
		ft_release_coders(data);
		return (-1);
	}
	if (pthread_cond_init(&data->start_cond, NULL) != 0)
	{
		fprintf(stderr, "Error init start_cond \n");
		ft_release_simulation_mutex(data);
		return (-1);
	}
	return (0);
}

static int	ft_init_coders(t_data *data)
{
	int	i;

	data->coders = malloc(sizeof(t_coder) * data->number_of_coders);
	if (!data->coders)
	{
		fprintf(stderr, "Error malloc coders\n");
		ft_release_dongle(data->number_of_coders, data);
		return (-1);
	}
	memset(data->coders, 0, sizeof(t_coder) * data->number_of_coders);
	i = 0;
	while (i < data->number_of_coders)
	{
		data->coders[i].id = i + 1;
		data->coders[i].data = data;
		data->coders[i].left_dongle = &data->dongles[i];
		data->coders[i].right_dongle = (&data->dongles[(i + 1)
				% data->number_of_coders]);
		i++;
	}
	return (0);
}

int	ft_init_variables(t_data *data)
{
	if (ft_init_dongles(data) == -1)
		return (-1);
	if (ft_init_coders(data) == -1)
		return (-1);
	if (ft_init_simulation(data) == -1)
		return (-1);
	if (ft_init_print(data) == -1)
		return (-1);
	return (0);
}
