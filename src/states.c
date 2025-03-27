#include "../include/philosophers.h"

void	thinking(t_philosopher *philosopher)
{
	long	current_time;

	philosopher->is_thinking = true;
	current_time = set_timestamp();
	printf("timestamp: %ld ms philosopher %d is thinking\n", current_time - philosopher->start_time, philosopher->id);
}

void	sleeping(t_philosopher *philosopher)
{
	long	current_time;

	philosopher->is_sleeping = true;
	current_time = set_timestamp();
	printf("timestamp: %ld ms philosopher %d is sleeping\n", current_time - philosopher->start_time, philosopher->id);
	usleep(philosopher->time_to_sleep * 1000);
}

void	eating(t_philosopher *philosopher)
{
	long	current_time;

	philosopher->is_eating = true;
	current_time = set_timestamp();
	printf("timestamp: %ld ms philosopher %d is eating\n", current_time - philosopher->start_time, philosopher->id);
	philosopher->last_meal = current_time;
	usleep(philosopher->time_to_eat * 1000);
	philosopher->is_eating = false;
}

bool	starved(t_philosopher *philosopher, bool print_message)
{
	long	current_time;

	current_time = set_timestamp();
	if (current_time - philosopher->last_meal >= philosopher->time_to_die)
	{
		if (print_message)
			printf("timestamp: %ld ms philosopher %d died\n", current_time - philosopher->start_time, philosopher->id);
		philosopher->is_dead = true;
		return (true);
	}
	return (false);
}
