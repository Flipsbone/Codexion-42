/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/24 10:24:45 by advacher          #+#    #+#             */
/*   Updated: 2026/05/04 10:16:00 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"
#include <stdio.h>
#include <string.h>

static int	ft_check_fifo(t_dongle *dongle, t_coder *coder)
{
	return (dongle->queue[0] == coder->id);
}

static int	ft_check_edf(t_dongle *dongle, t_coder *coder)
{
	long	coder_deadline;
	long	other_deadline;
	int		other_id;
	t_coder	*other;

	other_id = dongle->queue[0];
	if (other_id == coder->id)
		other_id = dongle->queue[1];
	other = &coder->data->coders[other_id - 1];
	coder_deadline = ft_get_last_compile(coder) + coder->data->time_to_burnout;
	other_deadline = ft_get_last_compile(other) + coder->data->time_to_burnout;
	if (other_deadline < coder_deadline)
		return (0);
	if (other_deadline == coder_deadline)
	{
		if (ft_get_nb_compiles(other) < ft_get_nb_compiles(coder))
			return (0);
		if (ft_get_nb_compiles(other) == ft_get_nb_compiles(coder)
			&& other_id < coder->id)
			return (0);
	}
	return (1);
}

int	ft_is_my_turn(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue_size <= 1)
		return (1);
	if (strcmp(coder->data->scheduler, "fifo") == 0)
		return (ft_check_fifo(dongle, coder));
	return (ft_check_edf(dongle, coder));
}
