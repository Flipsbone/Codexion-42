/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 10:38:32 by advacher          #+#    #+#             */
/*   Updated: 2026/05/04 09:54:51 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"
#include <stdio.h>

void	ft_wake_up_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_lock(&data->dongles[i].mutex);
		pthread_mutex_unlock(&data->dongles[i].mutex);
		i++;
	}
}

int	ft_check_burnout(t_data *data, int i, long now)
{
	long		time;
	long		start;

	if (now - data->coders[i].last_compile_start > data->time_to_burnout)
	{
		data->stop_simulation = SIM_BURNOUT;
		start = data->start_time;
		pthread_mutex_unlock(&data->sim_mutex);
		ft_wake_up_all(data);
		pthread_mutex_lock(&data->print_mutex);
		time = ft_get_time() - start;
		printf("%ld %d burned out\n", time, data->coders[i].id);
		pthread_mutex_unlock(&data->print_mutex);
		return (1);
	}
	return (0);
}

int	ft_evaluate_coders(t_data *data, long now)
{
	int	i;
	int	finished;

	i = 0;
	finished = 0;
	while (i < data->number_of_coders)
	{
		if (ft_check_burnout(data, i, now))
			return (1);
		if (data->coders[i].nb_compiles >= data->number_of_compiles_required)
			finished++;
		i++;
	}
	if (finished == data->number_of_coders)
	{
		data->stop_simulation = SIM_FINISHED;
		pthread_mutex_unlock(&data->sim_mutex);
		ft_wake_up_all(data);
		return (1);
	}
	return (0);
}
