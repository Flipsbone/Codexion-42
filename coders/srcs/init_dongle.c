/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_dongle.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 13:55:07 by advacher          #+#    #+#             */
/*   Updated: 2026/04/29 14:19:23 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int	ft_init_each_dongle(t_data *data, int i)
{
	data->dongles[i].id = i + 1;
	data->dongles[i].is_available = true;
	data->dongles[i].available_at = 0;
	data->dongles[i].queue_size = 0;
	data->dongles[i].queue[0] = 0;
	data->dongles[i].queue[1] = 0;
	if (pthread_mutex_init(&data->dongles[i].mutex, NULL) != 0)
	{
		fprintf(stderr, "Error init mutex\n");
		return (-1);
	}
	return (0);
}

int	ft_init_dongles(t_data *data)
{
	int	i;

	data->dongles = malloc(sizeof(t_dongle) * data->number_of_coders);
	if (!data->dongles)
	{
		fprintf(stderr, "Error malloc dongles\n");
		return (-1);
	}
	i = 0;
	while (i < data->number_of_coders)
	{
		if (ft_init_each_dongle(data, i) != 0)
		{
			ft_release_dongle(i, data);
			return (-1);
		}
		i++;
	}
	return (0);
}
