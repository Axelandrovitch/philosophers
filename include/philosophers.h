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

typedef struct s_input_data {
	int				number_of_philosophers;
	int				time_to_die;
	int				time_to_sleep;
	int				time_to_eat;
	int				must_eat_n;
	bool			count_each;
	long			start_timestamp;
} t_input_data;

typedef struct s_program_status
{
	bool			stop;
	int				fed_up_philosophers;
	pthread_mutex_t	mutex_stop;
} t_program_status;

typedef struct s_philosopher
{
	pthread_t	thread;
	int		id;
	int		time_to_eat;
	int		time_to_sleep;
	int		time_to_die;
	int		meals_count;
	int				must_eat_n;
	int				number_of_philosophers;
	struct 	s_philosopher *left;
	struct 	s_philosopher *right;
	pthread_mutex_t       fork;
	long	start_time;
	long	last_meal;
	t_program_status	*program;
} t_philosopher;

bool	parse_arguments(t_philosopher **p,char **argv, int argc, t_program_status *program);

void	print_philosophers(t_philosopher *head);

void	*free_philosophers(t_philosopher *head);

void	*philosopher_routine(void *arg);

void	create_threads(t_philosopher *philosophers);

void	join_threads(t_philosopher *philosophers);

long	set_timestamp(void);

void	timestamp_action(t_philosopher *philosopher);

// State functions
void	thinking(t_philosopher *philosopher);

void	sleeping(t_philosopher *philosopher);

void	eating(t_philosopher *philosopher);

bool	starved(t_philosopher *philosopher, bool print_message);

bool	starved_two(t_philosopher *philosopher, bool print_message);

bool	stop_banquet(t_philosopher *p);

#endif
