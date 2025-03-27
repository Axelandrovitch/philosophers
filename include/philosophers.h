/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahetru <marvin@42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/07 11:45:55 by ahetru            #+#    #+#             */
/*   Updated: 2025/03/07 11:46:50 by ahetru           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdbool.h>
# include <stdlib.h>

typedef struct s_program_status
{
	bool			stop;
	pthread_mutex_t	mutex_stop;
} t_program_status;

typedef struct s_philosopher
{
	pthread_t	thread;
	int		id;
	int		time_to_eat;
	int		time_to_sleep;
	int		time_to_die;
	struct 	s_philosopher *left;
	struct 	s_philosopher *right;
	pthread_mutex_t       fork;
	bool	is_eating;
	bool	is_sleeping;
	bool	is_thinking;
	bool	is_dead;
	long	start_time;
	long	last_meal;
	t_program_status	*program;
} t_philosopher;

typedef struct s_philo
{
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_sleep;
	int				time_to_eat;
	long			start_timestamp;
	t_philosopher 	*philosophers;
	t_program_status	*program;
} t_philo;

bool	parse_arguments(t_philo *philo, char **argv, t_program_status *program);

void	print_philosophers(t_philo *philo,t_philosopher *head);

void	*free_philosophers(t_philosopher *head);

void	*philosopher_routine(void *arg);

void	create_threads(t_philo *philo);

void	join_threads(t_philo *philo);

long	set_timestamp(void);

void	timestamp_action(t_philosopher *philosopher);

// State functions
void	thinking(t_philosopher *philosopher);

void	sleeping(t_philosopher *philosopher);

void	eating(t_philosopher *philosopher);

bool	starved(t_philosopher *philosopher, bool print_message);

bool	someone_starved(t_philosopher *philosopher);

#endif
