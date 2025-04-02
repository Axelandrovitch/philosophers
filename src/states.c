#include "../include/philosophers.h"

void	thinking(t_philosopher *p)
{
	pthread_mutex_lock(&p->program->mutex_stop);
    if (p->program->stop)
    {
        pthread_mutex_unlock(&p->program->mutex_stop);
        return;
    }
    pthread_mutex_unlock(&p->program->mutex_stop);

	long	current_time;

	current_time = set_timestamp();
	printf("%ld %d is thinking\n", current_time - p->start_time, p->id);
}

void	sleeping(t_philosopher *p)
{
	pthread_mutex_lock(&p->program->mutex_stop);
    if (p->program->stop)
    {
        pthread_mutex_unlock(&p->program->mutex_stop);
        return;
    }
    pthread_mutex_unlock(&p->program->mutex_stop);

	long	current_time;

	current_time = set_timestamp();
	printf("%ld %d is sleeping\n", current_time - p->start_time, p->id);
	usleep(p->time_to_sleep * 1000);
}

void	eating(t_philosopher *p)
{
	long	current_time;
	pthread_mutex_lock(&p->program->mutex_stop);
    if (p->program->stop)
    {
        pthread_mutex_unlock(&p->program->mutex_stop);
        return;
    }
    pthread_mutex_unlock(&p->program->mutex_stop);

	current_time = set_timestamp();
	printf("%ld %d is eating\n", current_time - p->start_time, p->id);
	pthread_mutex_lock(&p->program->mutex_stop);
	p->last_meal = current_time;
	pthread_mutex_unlock(&p->program->mutex_stop);
	usleep(p->time_to_eat * 1000);
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
