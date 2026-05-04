/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:25:21 by advacher          #+#    #+#             */
/*   Updated: 2026/05/04 10:22:34 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

int	ft_check_simulation_stop(t_data *data)
{
	int	status;

	pthread_mutex_lock(&data->sim_mutex);
	status = data->stop_simulation;
	pthread_mutex_unlock(&data->sim_mutex);
	return (status);
}

int	ft_usleep(long time_to_sleep, t_data *data)
{
	long	start;

	start = ft_get_time();
	while ((ft_get_time() - start) < time_to_sleep)
	{
		if (ft_check_simulation_stop(data) != 0)
			return (1);
		usleep(1000);
	}
	return (0);
}

void	ft_print_status(t_data *data, int id, char *status)
{
	long		time;
	long		start;
	t_sim_state	stop_reason;

	pthread_mutex_lock(&data->sim_mutex);
	start = data->start_time;
	stop_reason = data->stop_simulation;
	if (stop_reason == SIM_BURNOUT)
	{
		pthread_mutex_unlock(&data->sim_mutex);
		return ;
	}
	pthread_mutex_unlock(&data->sim_mutex);
	pthread_mutex_lock(&data->print_mutex);
	time = ft_get_time() - start;
	printf("%ld %d %s\n", time, id, status);
	pthread_mutex_unlock(&data->print_mutex);
}

long	ft_get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}
