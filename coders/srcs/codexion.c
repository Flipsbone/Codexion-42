/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/16 10:12:32 by advacher          #+#    #+#             */
/*   Updated: 2026/04/23 15:18:39 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/prototype.h"

int	main(int ac, char **av)
{
	t_data	data;

	if (ft_parse_arguments(ac, av, &data) == -1)
		return (-1);
	if (ft_init_variables(&data) == -1)
		return (-1);
	if (ft_start_simulation(&data) == -1)
	{
		ft_release_all(&data);
		return (-1);
	}
	ft_release_all(&data);
	return (0);
}
