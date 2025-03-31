#include "../include/philosophers.h"

long	set_timestamp(void)
{
	struct timeval	current_time;
	gettimeofday(&current_time, NULL);
	long	seconds = current_time.tv_sec;
	long	microseconds = current_time.tv_usec;
	long	miliseconds = (long)seconds * 1000 + (long)microseconds / 1000;
	return (miliseconds);
}

void	create_threads(t_philosopher *philosophers)
{
	t_philosopher	*current;
	int				i;

	current = philosophers;
	i = 0;
	while (i < current->number_of_philosophers)
	{
		if (pthread_create(&current->thread, NULL, philosopher_routine, (void *)current))
			return ;
		current = current->left;
		i++;
	}
}

void	join_threads(t_philosopher *philosophers)
{
	t_philosopher	*current;
	int	i;

	current = philosophers;
	i = 0;
	while (i < current->number_of_philosophers)
	{
		if (pthread_join(current->thread, NULL))
			return ;
		current = current->left;
		i++;
	}
}

bool	stop_banquet(t_philosopher *p)
{
	pthread_mutex_lock(&p->program->mutex_stop);
	if (p->program->stop)
	{
		pthread_mutex_unlock(&p->program->mutex_stop);
		return (true);
	}
	if (p->program->fed_up_philosophers == p->number_of_philosophers)
	{
		p->program->stop = true;
		pthread_mutex_unlock(&p->program->mutex_stop);
		return (true);
	}
	if (starved(p, true))
	{
		p->program->stop = true;
		pthread_mutex_unlock(&p->program->mutex_stop);
		return (true);
	}
	pthread_mutex_unlock(&p->program->mutex_stop);
	return (false);
}

void	take_forks(t_philosopher *p)
{
	if (stop_banquet(p))
		return ;
	if (p->id % 2 == 0)
	{
		pthread_mutex_lock(&p->right->fork);
		printf("timestamp: %ld ms philosopher %d took philosopher's %d fork\n",
			   set_timestamp() - p->start_time, p->id, p->right->id);

		pthread_mutex_lock(&p->fork);
		printf("timestamp: %ld ms philosopher %d took his fork\n",
			   set_timestamp() - p->start_time, p->id);
	}
	else
	{
		pthread_mutex_lock(&p->fork);
		printf("timestamp: %ld ms philosopher %d took his fork\n",
			   set_timestamp() - p->start_time, p->id);
		pthread_mutex_lock(&p->right->fork);
		printf("timestamp: %ld ms philosopher %d took philosopher's %d fork\n",
			   set_timestamp() - p->start_time, p->id, p->right->id);
	}
}

void    release_forks(t_philosopher *p, bool print_message)
{
	// if (stop_banquet(p))
	// 	return ;
    pthread_mutex_unlock(&p->fork);
	if (print_message)
    	printf("timestamp: %ld ms philosopher %d released his fork\n", set_timestamp() - p->start_time, p->id);
    if (p!= p->right)
    {
    	pthread_mutex_unlock(&p->right->fork);
		if (print_message)
    		printf("timestamp: %ld ms philosopher %d released philosopher's %d fork\n", set_timestamp() - p->start_time, p->id, p->right->id);
    }
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*p;

	p = (t_philosopher *)arg;
	while (true)
	{
		if (stop_banquet(p))
			return (NULL);
		take_forks(p);
		if (stop_banquet(p))
		{
			release_forks(p, false);
			return (NULL);
		}
		eating(p);
		if (stop_banquet(p))
		{
			release_forks(p, false);
			return (NULL);
		} else {
			release_forks(p, true);
		}
		if (stop_banquet(p))
			return (NULL);
		sleeping(p);
		if (stop_banquet(p))
			return (NULL);
		thinking(p);
	}
	return (NULL);
}
