/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prototype.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: advacher <advacher@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/04/22 11:36:01 by advacher          #+#    #+#             */
/*   Updated: 2026/05/04 10:09:13 by advacher         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PROTOTYPE_H
# define PROTOTYPE_H

# include "struct.h"

int		ft_parse_arguments(int ac, char **av, t_data *data);
int		ft_init_variables(t_data *data);
int		ft_init_dongles(t_data *data);

int		ft_start_simulation(t_data *data);
int		ft_finish_simulation(t_data *data);
void	*ft_coder_routine(void *thread);

void	*ft_monitor_routine(void *param);
void	ft_wake_up_all(t_data *data);
int		ft_check_burnout(t_data *data, int i, long now);
int		ft_evaluate_coders(t_data *data, long now);

int		ft_take_dongles(t_coder *coder);
void	ft_drop_dongles(t_coder *coder);
void	ft_lock_both_dongles(t_coder *coder, t_dongle *left, t_dongle *right);
void	ft_unlock_both_dongles(t_dongle *left, t_dongle *right);
int		ft_can_take_dongle(t_dongle *dongle, t_coder *coder);
void	ft_wait_for_dongle(t_dongle *target);

int		ft_is_my_turn(t_dongle *dongle, t_coder *coder);
void	ft_add_to_queue(t_dongle *dongle, t_coder *coder);
void	ft_remove_from_queue(t_dongle *dongle, t_coder *coder);
long	ft_get_last_compile(t_coder *coder);
long	ft_get_nb_compiles(t_coder *coder);

int		ft_usleep(long time_to_sleep_in_ms, t_data *data);
void	ft_print_status(t_data *data, int id, char *status);
long	ft_get_time(void);
int		ft_check_simulation_stop(t_data *data);

void	ft_release_coders(t_data *data);
void	ft_release_dongle(int i, t_data *data);
void	ft_release_simulation_mutex(t_data *data);
void	ft_release_simulation(t_data *data);
void	ft_release_all(t_data *data);

#endif