/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine_coder.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 10:39:25 by advacher          #+#    #+#             */
/*   Updated: 2026/05/04 10:29:21 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"
#include <unistd.h>

static int	ft_wait_for_start(t_data *data)
{
	pthread_mutex_lock(&data->sim_mutex);
	while (data->is_ready == 0)
		pthread_cond_wait(&data->start_cond, &data->sim_mutex);
	if (data->is_ready == -1)
	{
		pthread_mutex_unlock(&data->sim_mutex);
		return (-1);
	}
	pthread_mutex_unlock(&data->sim_mutex);
	return (0);
}

static void	ft_coder_action(t_coder *coder, t_data *data)
{
	if (ft_take_dongles(coder) == 0)
	{
		pthread_mutex_lock(&data->sim_mutex);
		coder->last_compile_start = ft_get_time();
		pthread_mutex_unlock(&data->sim_mutex);
		ft_print_status(data, coder->id, "is compiling");
		if (ft_usleep(data->time_to_compile, data) == 1)
		{
			ft_drop_dongles(coder);
			return ;
		}
		pthread_mutex_lock(&data->sim_mutex);
		coder->nb_compiles++;
		pthread_mutex_unlock(&data->sim_mutex);
		ft_drop_dongles(coder);
		ft_print_status(data, coder->id, "is debugging");
		if (ft_usleep(data->time_to_debug, data) == 1)
			return ;
		ft_print_status(data, coder->id, "is refactoring");
		ft_usleep(data->time_to_refactor, data);
	}
}

void	*ft_coder_routine(void *thread)
{
	t_coder	*coder;
	t_data	*data;

	coder = (t_coder *)thread;
	data = coder->data;
	if (ft_wait_for_start(data) == -1)
		return (NULL);
	if (coder->id % 2 == 0)
		usleep(1000);
	while (ft_check_simulation_stop(data) == 0)
		ft_coder_action(coder, data);
	return (NULL);
}
