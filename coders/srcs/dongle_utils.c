/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 10:37:43 by advacher          #+#    #+#             */
/*   Updated: 2026/05/04 09:54:38 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"
#include <unistd.h>

void	ft_lock_both_dongles(t_coder *coder, t_dongle *left, t_dongle *right)
{
	if (coder->id % 2 != 0)
	{
		pthread_mutex_lock(&left->mutex);
		pthread_mutex_lock(&right->mutex);
	}
	else
	{
		pthread_mutex_lock(&right->mutex);
		pthread_mutex_lock(&left->mutex);
	}
}

void	ft_unlock_both_dongles(t_dongle *left, t_dongle *right)
{
	pthread_mutex_unlock(&left->mutex);
	pthread_mutex_unlock(&right->mutex);
}

int	ft_can_take_dongle(t_dongle *dongle, t_coder *coder)
{
	if (dongle->is_available == false)
		return (0);
	if (ft_get_time() < dongle->available_at)
		return (0);
	if (ft_is_my_turn(dongle, coder) == 0)
		return (0);
	return (1);
}

void	ft_add_to_queue(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue_size < 2)
	{
		dongle->queue[dongle->queue_size] = coder->id;
		dongle->queue_size++;
	}
}

void	ft_remove_from_queue(t_dongle *dongle, t_coder *coder)
{
	if (dongle->queue_size == 0)
		return ;
	if (dongle->queue[0] == coder->id)
	{
		dongle->queue[0] = dongle->queue[1];
		dongle->queue[1] = 0;
		dongle->queue_size--;
	}
	else if (dongle->queue_size == 2 && dongle->queue[1] == coder->id)
	{
		dongle->queue[1] = 0;
		dongle->queue_size--;
	}
}
