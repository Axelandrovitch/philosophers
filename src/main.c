#include "../include/philosophers.h"

void	*monitor_philosophers(void *arg)
{
	t_philo	*philo;
	t_philosopher *current;
	int	i;

	philo = (t_philo *)arg;
	while (true)
	{
		i = 0;
		current = philo->philosophers;
		while (i < philo->number_of_philosophers)
		{
			if (current->is_dead)
				{
					pthread_mutex_lock(&current->program->mutex_stop);
					current->program->stop = true;
					pthread_mutex_unlock(&current->program->mutex_stop);
					return (NULL);
				}
			current = current->left;
			i++;
		}
	usleep(10);
	}
	return (NULL);
}

void	destroy_mutexes(t_philo *philo) {
	t_philosopher	*current_philosopher;
	int	i;

	current_philosopher = philo->philosophers;
	i = 0;
	while (i < philo->number_of_philosophers)
	{
		pthread_mutex_destroy(&current_philosopher->fork);
		i++;	
		current_philosopher = current_philosopher->left;
	}
}

void	init_banquet(t_philo *philo)
{
	pthread_t	monitor;

	pthread_create(&monitor, NULL, &monitor_philosophers, philo);
	create_threads(philo);
	pthread_join(monitor, NULL);
	join_threads(philo);
	destroy_mutexes(philo);
	pthread_mutex_destroy(&philo->program->mutex_stop);
}

int	main(int argc, char **argv)
{
	t_philo philo;
	t_program_status program;

	if (argc != 5)
	{
		printf("Usage: <%s> <number of philosophers> <time to die> <time to eat> <time to sleep>\n", argv[0]);
		return (0);
	}
	pthread_mutex_init(&program.mutex_stop, NULL);
	program.stop = false;
	philo.program = &program;
	parse_arguments(&philo, argv, &program);
	printf("-------------------------------------------------------------------------\n");
	init_banquet(&philo);
	free_philosophers(philo.philosophers);
	return (0);
}
