#include "../include/philosophers.h"

void	thinking(t_philosopher *p)
{
	if (stop_banquet(p))
		return ;
	long	current_time;

	current_time = set_timestamp();
	printf("timestamp: %ld ms philosopher %d is thinking\n", current_time - p->start_time, p->id);
}

void	sleeping(t_philosopher *p)
{
	if (stop_banquet(p))
		return ;
	long	current_time;

	current_time = set_timestamp();
	printf("timestamp: %ld ms philosopher %d is sleeping\n", current_time - p->start_time, p->id);
	usleep(p->time_to_sleep * 1000);
}

void	eating(t_philosopher *p)
{
	if (stop_banquet(p))
		return ;
	long	current_time;
	current_time = set_timestamp();
	printf("timestamp: %ld ms philosopher %d is eating\n", current_time - p->start_time, p->id);
	p->last_meal = current_time;
	usleep(p->time_to_eat * 1000);
}

bool	starved(t_philosopher *p, bool print_message)
{
	long	current_time;

	current_time = set_timestamp();
	if (current_time - p->last_meal >= p->time_to_die)
	{
		if (print_message)
			printf("timestamp: %ld ms philosopher %d died\n", current_time - p->start_time, p->id);
		return (true);
	}
	return (false);
}
