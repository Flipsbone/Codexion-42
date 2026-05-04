/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongle.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 10:37:48 by advacher          #+#    #+#             */
/*   Updated: 2026/04/29 16:22:15 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"
#include <unistd.h>

static int	ft_check_and_take(t_coder *coder, t_dongle *left, t_dongle *right)
{
	if (ft_can_take_dongle(left, coder) && ft_can_take_dongle(right, coder))
	{
		left->is_available = false;
		right->is_available = false;
		ft_remove_from_queue(left, coder);
		ft_remove_from_queue(right, coder);
		ft_unlock_both_dongles(left, right);
		ft_print_status(coder->data, coder->id, "has taken a dongle");
		ft_print_status(coder->data, coder->id, "has taken a dongle");
		return (1);
	}
	return (0);
}

static int	ft_wait_for_dongles(t_coder *coder, t_dongle *left, t_dongle *right)
{
	while (!ft_check_simulation_stop(coder->data))
	{
		ft_lock_both_dongles(coder, left, right);
		if (ft_check_and_take(coder, left, right))
			return (0);
		ft_unlock_both_dongles(left, right);
		usleep(500);
	}
	return (1);
}

int	ft_take_dongles(t_coder *coder)
{
	t_dongle	*left;
	t_dongle	*right;
	int			status;

	left = coder->left_dongle;
	right = coder->right_dongle;
	if (left == right)
	{
		ft_print_status(coder->data, coder->id, "has taken a dongle");
		while (!ft_check_simulation_stop(coder->data))
			usleep(1000);
		return (1);
	}
	ft_lock_both_dongles(coder, left, right);
	ft_add_to_queue(left, coder);
	ft_add_to_queue(right, coder);
	ft_unlock_both_dongles(left, right);
	status = ft_wait_for_dongles(coder, left, right);
	return (status);
}

static void	ft_drop_one_dongle(t_coder *coder, t_dongle *dongle)
{
	pthread_mutex_lock(&dongle->mutex);
	dongle->is_available = true;
	dongle->available_at = ft_get_time() + coder->data->dongle_cooldown;
	pthread_mutex_unlock(&dongle->mutex);
}

void	ft_drop_dongles(t_coder *coder)
{
	ft_drop_one_dongle(coder, coder->left_dongle);
	ft_drop_one_dongle(coder, coder->right_dongle);
}
