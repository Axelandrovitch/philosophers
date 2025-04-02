#include "../include/philosophers.h"

long	set_timestamp(void)
{
	struct timeval	time;
	gettimeofday(&time, NULL);
	// long	seconds = current_time.tv_sec;
	// long	microseconds = current_time.tv_usec;
	// long	miliseconds = (long)seconds * 1000 + (long)microseconds / 1000;
	return (time.tv_sec * 1000 + time.tv_usec / 1000);
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

void	lonely_philosopher(t_philosopher *p)
{
	pthread_mutex_lock(&p->fork);
	printf("timestamp: %ld ms philosopher %d took his fork\n",
			set_timestamp() - p->start_time, p->id);
	while (!starved(p, false))
		;
	pthread_mutex_unlock(&p->fork);
}

void	take_forks(t_philosopher *p)
{
	pthread_mutex_lock(&p->fork);
	pthread_mutex_lock(&p->program->mutex_stop);
	if (p->program->stop)
         {
             pthread_mutex_unlock(&p->program->mutex_stop);
             pthread_mutex_unlock(&p->fork);
             return ;
         }
	pthread_mutex_unlock(&p->program->mutex_stop);
	printf("%ld %d has taken his fork\n",
		   set_timestamp() - p->start_time, p->id);
	pthread_mutex_lock(&p->right->fork);
	pthread_mutex_lock(&p->program->mutex_stop);
	if (p->program->stop)
	{
		pthread_mutex_unlock(&p->program->mutex_stop);
		pthread_mutex_unlock(&p->fork);
		pthread_mutex_unlock(&p->right->fork);
		return ;
	}
	printf("%ld %d has taken right fork\n",
		   set_timestamp() - p->start_time, p->id);
	pthread_mutex_unlock(&p->program->mutex_stop);
}

// void take_forks(t_philosopher *p)
// {
//     // For even-numbered philosophers: take right fork first.
//     if (p->id % 2 == 0)
//     {
//         // Acquire right fork.
//         pthread_mutex_lock(&p->right->fork);
//         // Immediately check stop flag.
//         pthread_mutex_lock(&p->program->mutex_stop);
//         if (p->program->stop)
//         {
//             pthread_mutex_unlock(&p->program->mutex_stop);
//             pthread_mutex_unlock(&p->right->fork);
//             return;
//         }
//         pthread_mutex_unlock(&p->program->mutex_stop);
//         printf("%ld %d has taken right fork\n", set_timestamp() - p->start_time, p->id);
//
//         // Acquire left fork.
//         pthread_mutex_lock(&p->fork);
//         pthread_mutex_lock(&p->program->mutex_stop);
//         if (p->program->stop)
//         {
//             pthread_mutex_unlock(&p->program->mutex_stop);
//             // Release already acquired forks.
//             pthread_mutex_unlock(&p->fork);
//             pthread_mutex_unlock(&p->right->fork);
//             return;
//         }
//         pthread_mutex_unlock(&p->program->mutex_stop);
//         printf("%ld %d has taken his fork\n", set_timestamp() - p->start_time, p->id);
//     }
//     else
//     {
//         // For odd-numbered philosophers: take left fork first.
//         pthread_mutex_lock(&p->fork);
//         pthread_mutex_lock(&p->program->mutex_stop);
//         if (p->program->stop)
//         {
//             pthread_mutex_unlock(&p->program->mutex_stop);
//             pthread_mutex_unlock(&p->fork);
//             return;
//         }
//         pthread_mutex_unlock(&p->program->mutex_stop);
//         printf("%ld %d has taken his fork\n", set_timestamp() - p->start_time, p->id);
//
//         pthread_mutex_lock(&p->right->fork);
//         pthread_mutex_lock(&p->program->mutex_stop);
//         if (p->program->stop)
//         {
//             pthread_mutex_unlock(&p->program->mutex_stop);
//             // Release already acquired forks.
//             pthread_mutex_unlock(&p->right->fork);
//             pthread_mutex_unlock(&p->fork);
//             return;
//         }
//         pthread_mutex_unlock(&p->program->mutex_stop);
//         printf("%ld %d has taken right fork\n", set_timestamp() - p->start_time, p->id);
//     }
// }


void    release_forks(t_philosopher *p, bool print_message)
{
    pthread_mutex_unlock(&p->fork);
	if (print_message)
    	printf("%ld %d released his fork\n", set_timestamp() - p->start_time, p->id);
    if (p!= p->right)
    {
    	pthread_mutex_unlock(&p->right->fork);
		if (print_message)
    		printf("%ld %d released fork\n", set_timestamp() - p->start_time, p->id);
    }
}

// bool	stop_banquet(t_philosopher *p, bool release)
// {
// 	pthread_mutex_lock(&p->program->mutex_stop);
// 	if (p->program->stop)
// 	{
// 		pthread_mutex_unlock(&p->program->mutex_stop);
// 		if (release)
// 			release_forks(p,false);
// 		return (true);
// 	}
// 	if (p->program->fed_up_philosophers == p->number_of_philosophers)
// 	{
// 		p->program->stop = true;
// 		pthread_mutex_unlock(&p->program->mutex_stop);
// 		if (release)
// 			release_forks(p,false);
// 		return (true);
// 	}
// 	if (starved(p, true))
// 	{
// 		p->program->stop = true;
// 		pthread_mutex_unlock(&p->program->mutex_stop);
// 		if (release)
// 			release_forks(p,false);
// 		return (true);
// 	}
// 	pthread_mutex_unlock(&p->program->mutex_stop);
// 	return (false);
// }

bool	check_banquet_state(t_philosopher *p, bool release)
{
		pthread_mutex_lock(&p->program->mutex_stop);
        if (p->program->stop)
        {
			if (release)
				release_forks(p, false);
            pthread_mutex_unlock(&p->program->mutex_stop);
            return (false);
        }
        pthread_mutex_unlock(&p->program->mutex_stop);
		return (true);
}

void	*philosopher_routine(void *arg)
{
	t_philosopher	*p;

	p = (t_philosopher *)arg;
	if (p->id % 2 == 0)
		usleep(1000);
	while (true)
	{
  		if (!check_banquet_state(p, false))
			return (NULL);
		if (p->number_of_philosophers == 1)
		{
			lonely_philosopher(p);
			return (NULL);
		}
		take_forks(p);
  		if (!check_banquet_state(p, true))
			return (NULL);
		eating(p);
  		if (!check_banquet_state(p, true))
			return (NULL);
		release_forks(p, true);
  		if (!check_banquet_state(p, false))
			return (NULL);
		sleeping(p);
  		if (!check_banquet_state(p, false))
			return (NULL);
		thinking(p);
	}
	return (NULL);
}
