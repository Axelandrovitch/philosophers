#include "../include/philosophers.h"
#include <unistd.h>

long	set_timestamp(void)
{
	struct timeval	current_time;
	gettimeofday(&current_time, NULL);
	long	seconds = current_time.tv_sec;
	long	microseconds = current_time.tv_usec;
	long	miliseconds = (long)seconds * 1000 + (long)microseconds / 1000;
	return (miliseconds);
}

void	timestamp_action(t_philosopher *philosopher)
{
	long	now;

	now = set_timestamp();
	printf("%ld thread for philosopher %d has been created\n", now - philosopher->start_time, philosopher->id);
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

void    release_forks(t_philosopher *philosopher)
{
    pthread_mutex_unlock(&philosopher->fork);
    printf("timestamp: %ld ms philosopher %d released his fork\n", set_timestamp() - philosopher->start_time, philosopher->id);
    if (philosopher != philosopher->right)
    {
    	pthread_mutex_unlock(&philosopher->right->fork);
    	printf("timestamp: %ld ms philosopher %d released philosopher's %d fork\n", set_timestamp() - philosopher->start_time, philosopher->id, philosopher->right->id);
    }
}

void    take_forks(t_philosopher *philosopher)
{
	if (philosopher->id % 2 == 0)
	{
		pthread_mutex_lock(&philosopher->fork);
		if (someone_starved(philosopher))
		{
			pthread_mutex_unlock(&philosopher->fork);
			return ;
		}
		printf("timestamp: %ld ms philosopher %d took his fork\n", set_timestamp() - philosopher->start_time, philosopher->id);
		if (philosopher != philosopher->right)
			{
				pthread_mutex_lock(&philosopher->right->fork);
				if (someone_starved(philosopher))
				{
					pthread_mutex_unlock(&philosopher->fork);
					pthread_mutex_unlock(&philosopher->right->fork);
					return ;
				}
				printf("timestamp: %ld ms philosopher %d took philosopher's %d fork\n", set_timestamp() - philosopher->start_time, philosopher->id, philosopher->right->id);
			}
	}
	else
	{
		if (philosopher != philosopher->right)
			{
				pthread_mutex_lock(&philosopher->right->fork);
				if (someone_starved(philosopher))
				{
					pthread_mutex_unlock(&philosopher->right->fork);
					return ;
				}
				printf("timestamp: %ld ms philosopher %d took philosopher's %d fork\n", set_timestamp() - philosopher->start_time, philosopher->id, philosopher->right->id);
			}
		pthread_mutex_lock(&philosopher->fork);
		if (someone_starved(philosopher))
		{
			pthread_mutex_unlock(&philosopher->fork);
			if (philosopher != philosopher->right)
				pthread_mutex_unlock(&philosopher->right->fork);
			return ;
		}
		printf("timestamp: %ld ms philosopher %d took his fork\n", set_timestamp() - philosopher->start_time, philosopher->id);
	}
	if (philosopher == philosopher->right)
	{
		release_forks(philosopher);
		while (!starved(philosopher, false))
			;
	}
}


bool	someone_starved(t_philosopher *philosopher)
{
	pthread_mutex_lock(&philosopher->program->mutex_stop);
	if (philosopher->program->stop)
	{
		pthread_mutex_unlock(&philosopher->program->mutex_stop);
		return (true);
	}
	pthread_mutex_unlock(&philosopher->program->mutex_stop);
	return (false);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*philosopher;

	philosopher = (t_philosopher *)arg;
	while (true)
	{
		if (starved(philosopher, true) || someone_starved(philosopher))
			return (NULL);
		take_forks(philosopher);
		if (starved(philosopher, true) || someone_starved(philosopher))
			return (NULL);
		eating(philosopher);
		if (starved(philosopher, true) || someone_starved(philosopher))
			return (NULL);
		release_forks(philosopher);
		if (starved(philosopher, true) || someone_starved(philosopher))
			return (NULL);
		sleeping(philosopher);
		if (starved(philosopher, true) || someone_starved(philosopher))
			return (NULL);
		thinking(philosopher);
		if (starved(philosopher, true) || someone_starved(philosopher))
			return (NULL);
		usleep(100);
	}
	return (NULL);
}
	// eat: On regènère la barre de vie au début de l'état eating
	// sleep: on dort pendant une durée determinée.
	// Problème: Il faut pouvoir dire quand un philosophe meurt dans son sommeil
	// think: ne prend pas de durée définie
