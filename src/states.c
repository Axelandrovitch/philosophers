#include "../include/philosophers.h"

void	thinking(t_philosopher *p)
{
	if (stop_banquet(p))
		return ;
	long	current_time;

	current_time = set_timestamp();
	printf("timestamp: %ld ms philosopher %d is thinking\n", current_time - p->start_time, p->id);
}

void smart_sleep(long duration_ms, t_philosopher *p)
{
	long start = set_timestamp();
	while (!stop_banquet(p))
	{
		if (set_timestamp() - start >= duration_ms)
			break;
		usleep(100);
    }
}

void	sleeping(t_philosopher *p)
{
	if (stop_banquet(p))
		return ;
	long	current_time;

	current_time = set_timestamp();
	smart_sleep(p->time_to_eat, p);
	printf("timestamp: %ld ms philosopher %d is sleeping\n", current_time - p->start_time, p->id);
}

void	eating(t_philosopher *p)
{
	if (stop_banquet(p))
		return ;
	long	current_time;
	current_time = set_timestamp();
	printf("timestamp: %ld ms philosopher %d is eating\n", current_time - p->start_time, p->id);
	p->last_meal = current_time;
	// usleep(p->time_to_eat * 1000);
	smart_sleep(p->time_to_eat, p);
	p->meals_count++;
	if (p->meals_count == p->must_eat_n && p->must_eat_n > 0)
	{
		pthread_mutex_lock(&p->program->mutex_stop);
		p->program->fed_up_philosophers++;
		pthread_mutex_unlock(&p->program->mutex_stop);
	}
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
