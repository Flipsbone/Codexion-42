/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 10:38:42 by advacher          #+#    #+#             */
/*   Updated: 2026/04/28 15:32:34 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"
#include <unistd.h>

static int	ft_check_status(t_data *data)
{
	long	now;

	now = ft_get_time();
	pthread_mutex_lock(&data->sim_mutex);
	if (ft_evaluate_coders(data, now) == 1)
		return (0);
	pthread_mutex_unlock(&data->sim_mutex);
	return (0);
}

void	*ft_monitor_routine(void *param)
{
	t_data	*data;

	data = (t_data *)param;
	pthread_mutex_lock(&data->sim_mutex);
	while (data->is_ready == 0)
		pthread_cond_wait(&data->start_cond, &data->sim_mutex);
	if (data->is_ready == -1)
	{
		pthread_mutex_unlock(&data->sim_mutex);
		return (NULL);
	}
	pthread_mutex_unlock(&data->sim_mutex);
	while (ft_check_simulation_stop(data) == 0)
	{
		if (ft_check_status(data) == -1)
			return ((void *)-1);
		usleep(1000);
	}
	return (NULL);
}
