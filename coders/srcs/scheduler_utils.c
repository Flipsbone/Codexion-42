/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   scheduler_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/28 10:38:49 by advacher          #+#    #+#             */
/*   Updated: 2026/05/04 10:16:14 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"
#include "../include/struct.h"

long	ft_get_last_compile(t_coder *coder)
{
	long	time;

	pthread_mutex_lock(&coder->data->sim_mutex);
	time = coder->last_compile_start;
	pthread_mutex_unlock(&coder->data->sim_mutex);
	return (time);
}

long	ft_get_nb_compiles(t_coder *coder)
{
	long	nb;

	pthread_mutex_lock(&coder->data->sim_mutex);
	nb = coder->nb_compiles;
	pthread_mutex_unlock(&coder->data->sim_mutex);
	return (nb);
}
