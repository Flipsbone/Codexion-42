/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   clean.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:25:08 by advacher          #+#    #+#             */
/*   Updated: 2026/04/29 14:20:15 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include <stdlib.h>

void	ft_release_all(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&data->sim_mutex);
	pthread_cond_destroy(&data->start_cond);
	pthread_mutex_destroy(&data->print_mutex);
	free(data->dongles);
	free(data->coders);
}

void	ft_release_simulation(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&data->sim_mutex);
	pthread_cond_destroy(&data->start_cond);
	free(data->dongles);
	free(data->coders);
}

void	ft_release_simulation_mutex(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
	pthread_mutex_destroy(&data->sim_mutex);
	free(data->dongles);
	free(data->coders);
}

void	ft_release_coders(t_data *data)
{
	int	i;

	i = 0;
	while (i < data->number_of_coders)
	{
		pthread_mutex_destroy(&data->dongles[i].mutex);
		i++;
	}
	free(data->dongles);
	free(data->coders);
}

void	ft_release_dongle(int i, t_data *data)
{
	int	j;

	j = 0;
	while (j < i)
	{
		pthread_mutex_destroy(&data->dongles[j].mutex);
		j++;
	}
	free(data->dongles);
}
