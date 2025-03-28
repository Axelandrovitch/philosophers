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

void	timestamp_action(t_philosopher *p)
{
	long	now;

	now = set_timestamp();
	printf("%ld thread for p %d has been created\n", now - p->start_time, p->id);
}

void	create_threads(t_philo *philo)
{
	t_philosopher	*current;
	int				i;

	current = philo->philosophers;
	i = 0;
	while (i < philo->number_of_philosophers)
	{
		if (pthread_create(&current->thread, NULL, philosopher_routine, (void *)current))
			return ;
		current = current->left;
		i++;
	}
}

void	join_threads(t_philo *philo)
{
	t_philosopher	*current;
	int	i;

	current = philo->philosophers;
	i = 0;
	while (i < philo->number_of_philosophers)
	{
		if (pthread_join(current->thread, NULL))
			return ;
		current = current->left;
		i++;
	}
}

void	take_forks(t_philosopher *p)
{
	if (p->id % 2 == 0)
		usleep(100);
	pthread_mutex_lock(&p->program->mutex_stop);
	if (p->program->stop)
	{
		pthread_mutex_unlock(&p->program->mutex_stop);
		return ;
	}
	if (starved(p, true))
	{
		p->program->stop = true;
		pthread_mutex_unlock(&p->program->mutex_stop);
		return ;
	}
	pthread_mutex_unlock(&p->program->mutex_stop);
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

void    release_forks(t_philosopher *p)
{
	pthread_mutex_lock(&p->program->mutex_stop);
	if (p->program->stop)
	{
		pthread_mutex_unlock(&p->program->mutex_stop);
		return ;
	}
	if (starved(p, true))
	{
		p->program->stop = true;
		pthread_mutex_unlock(&p->program->mutex_stop);
		return ;
	}
	pthread_mutex_unlock(&p->program->mutex_stop);
    pthread_mutex_unlock(&p->fork);
    printf("timestamp: %ld ms philosopher %d released his fork\n", set_timestamp() - p->start_time, p->id);
    if (p!= p->right)
    {
    	pthread_mutex_unlock(&p->right->fork);
    	printf("timestamp: %ld ms philosopher %d released philosopher's %d fork\n", set_timestamp() - p->start_time, p->id, p->right->id);
    }
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*p;

	p = (t_philosopher *)arg;
	while (true)
	{
		pthread_mutex_lock(&p->program->mutex_stop);
		if (p->program->stop)
		{
			pthread_mutex_unlock(&p->program->mutex_stop);
			return (NULL);
		}
		if (starved(p, true))
		{
			p->program->stop = true;
			pthread_mutex_unlock(&p->program->mutex_stop);
			return (NULL);
		}
		pthread_mutex_unlock(&p->program->mutex_stop);
		take_forks(p);
		eating(p);
		release_forks(p);
		sleeping(p);
		thinking(p);
		usleep(100);
	}
	return (NULL);
}