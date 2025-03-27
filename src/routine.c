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

// void    release_forks(t_p *p)
// {
//     pthread_mutex_unlock(&p->fork);
//     printf("timestamp: %ld ms p %d released his fork\n", set_timestamp() - p->start_time, p->id);
//     if (p != p->right)
//     {
//     	pthread_mutex_unlock(&p->right->fork);
//     	printf("timestamp: %ld ms p %d released p's %d fork\n", set_timestamp() - p->start_time, p->id, p->right->id);
//     }
// }

void	release_left_fork(t_philosopher *p)
{
	pthread_mutex_lock(&p->program->mutex_stop);
	if (p->program->stop)
	{
		pthread_mutex_lock(&p->program->mutex_stop);
		return ;
	}
	pthread_mutex_unlock(&p->left->fork);
    printf("timestamp: %ld ms p %d released his fork\n", set_timestamp() - p->start_time, p->id);
	pthread_mutex_unlock(&p->program->mutex_stop);
}
void	release_right_fork(t_philosopher *p)
{
	pthread_mutex_lock(&p->program->mutex_stop);
	if (p->program->stop)
	{
		pthread_mutex_lock(&p->program->mutex_stop);
		return ;
	}
	pthread_mutex_unlock(&p->right->fork);
    printf("timestamp: %ld ms p %d released p's %d fork\n", set_timestamp() - p->start_time, p->id, p->right->id);
	pthread_mutex_unlock(&p->program->mutex_stop);
}

void	take_left_fork(t_philosopher *p)
{
	if (p->id % 2 == 0)
		usleep(100);
	pthread_mutex_lock(&p->program->mutex_stop);
	if (p->program->stop)
		{
			pthread_mutex_unlock(&p->program->mutex_stop);
			return ;
		}
	pthread_mutex_lock(&p->fork);
	printf("timestamp: %ld ms p %d took his fork\n", set_timestamp() - p->start_time, p->id);
	pthread_mutex_unlock(&p->program->mutex_stop);
}

void	take_right_fork(t_philosopher *p)
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
	pthread_mutex_lock(&p->right->fork);
 	printf("timestamp: %ld ms p %d took p's %d fork\n", set_timestamp() - p->start_time, p->id, p->right->id);
	pthread_mutex_unlock(&p->program->mutex_stop);
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
	// pthread_mutex_lock(&p->fork);
	// printf("timestamp: %ld ms philosopher %d took his fork\n", set_timestamp() - p->start_time, p->id);
	// pthread_mutex_lock(&p->right->fork);
 	// printf("timestamp: %ld ms philosopher %d took philosopher's %d fork\n", set_timestamp() - p->start_time, p->id, p->right->id);
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
// void	simple_take_forks(t_p *p)
// {
// 	if (p->id % 2 == 0)
// 		usleep(100);
// 	pthread_mutex_lock(&p->fork);
// 	if (p->program->stop) {
// 		pthread_mutex_unlock(&p->fork);
// 		return ;
// 	}
// 	if (p == p->right)
// 	{
// 		release_forks(p);	
// 		while (!starved_two(p, false))
// 			;
// 			return ;
// 	}
// 	printf("timestamp: %ld ms p %d took his fork\n", set_timestamp() - p->start_time, p->id);
// 	pthread_mutex_lock(&p->right->fork);
// 	if (someone_starved(p))
// 	{
// 		pthread_mutex_unlock(&p->fork);
// 		pthread_mutex_unlock(&p->right->fork);
// 		return ;
// 	}
// 	printf("timestamp: %ld ms p %d took p's %d fork\n", set_timestamp() - p->start_time, p->id, p->right->id);
// }

// void    take_forks(t_p *p)
// {
// 	if (p->id % 2 == 0)
// 	{
// 		pthread_mutex_lock(&p->fork);
// 		if (someone_starved(p))
// 		{
// 			pthread_mutex_unlock(&p->fork);
// 			return ;
// 		}
// 		printf("timestamp: %ld ms p %d took his fork\n", set_timestamp() - p->start_time, p->id);
// 		if (p != p->right)
// 			{
// 				pthread_mutex_lock(&p->right->fork);
// 				if (someone_starved(p))
// 				{
// 					pthread_mutex_unlock(&p->fork);
// 					pthread_mutex_unlock(&p->right->fork);
// 					return ;
// 				}
// 				printf("timestamp: %ld ms p %d took p's %d fork\n", set_timestamp() - p->start_time, p->id, p->right->id);
// 			}
// 	}
// 	else
// 	{
// 		if (p != p->right)
// 			{
// 				pthread_mutex_lock(&p->right->fork);
// 				if (someone_starved(p))
// 				{
// 					pthread_mutex_unlock(&p->right->fork);
// 					return ;
// 				}
// 				printf("timestamp: %ld ms p %d took p's %d fork\n", set_timestamp() - p->start_time, p->id, p->right->id);
// 			}
// 		pthread_mutex_lock(&p->fork);
// 		if (someone_starved(p))
// 		{
// 			pthread_mutex_unlock(&p->fork);
// 			if (p != p->right)
// 				pthread_mutex_unlock(&p->right->fork);
// 			return ;
// 		}
// 		printf("timestamp: %ld ms p %d took his fork\n", set_timestamp() - p->start_time, p->id);
// 	}
// 	if (p == p->right)
// 	{
// 		release_forks(p);
// 		while (!starved_two(p, false))
// 			;
// 	}
// }

// bool	someone_starved(t_p *p)
// {
// 	pthread_mutex_lock(&p->program->mutex_stop);
// 	if (p->program->stop)
// 	{
// 		pthread_mutex_unlock(&p->program->mutex_stop);
// 		return (true);
// 	}
// 	pthread_mutex_unlock(&p->program->mutex_stop);
// 	return (false);
// }


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
// void	*p_routine(void *arg)
// {
// 	t_p	*p;

// 	p = (t_p *)arg;
// 	while (true)
// 	{
// 		if (starved_two(p, true))
// 			return (NULL);
// 		simple_take_forks(p);
// 		if (starved_two(p, true))
// 			return (NULL);
// 		eating(p);
// 		if (starved_two(p, true))
// 			return (NULL);
// 		release_forks(p);
// 		if (starved_two(p, true))
// 			return (NULL);
// 		sleeping(p);
// 		if (starved_two(p, true))
// 			return (NULL);
// 		thinking(p);
// 		if (starved_two(p, true))
// 			return (NULL);
// 		usleep(100);
// 	}
// 	return (NULL);
// }
	// eat: On regènère la barre de vie au début de l'état eating
	// sleep: on dort pendant une durée determinée.
	// Problème: Il faut pouvoir dire quand un philosophe meurt dans son sommeil
	// think: ne prend pas de durée définie
