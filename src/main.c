#include "../include/philosophers.h"

void *monitor_routine(void *arg)
{
    t_philosopher *philosophers;
    t_philosopher *current;
    int i;
    long current_time;
   
	philosophers = 	(t_philosopher *)arg;
    while (1)
    {
        current = philosophers;
        i = 0;
        pthread_mutex_lock(&current->program->mutex_stop);
        if (current->program->fed_up_philosophers == current->number_of_philosophers)
        {
            current->program->stop = true;
            pthread_mutex_unlock(&current->program->mutex_stop);
            return (NULL);
        }
        pthread_mutex_unlock(&current->program->mutex_stop);
        while (i < current->number_of_philosophers)
        {
            current_time = set_timestamp();
            pthread_mutex_lock(&current->program->mutex_stop);
            if (current_time - current->last_meal >= current->time_to_die)
            {
                printf("%ld %d died\n", current_time - current->start_time, current->id);
                current->program->stop = true;
                pthread_mutex_unlock(&current->program->mutex_stop);
                return (NULL);
            }
            pthread_mutex_unlock(&current->program->mutex_stop);
            current = current->left;
            i++;
        }
        usleep(500);
    }
    return NULL;
}

void	banquet(t_philosopher *philosophers)
{
	pthread_t	monitor;

	pthread_create(&monitor, NULL, monitor_routine, philosophers);
	create_threads(philosophers);
	join_threads(philosophers);
	pthread_join(monitor, NULL);
	pthread_mutex_destroy(&philosophers->program->mutex_stop);
}

int	main(int argc, char **argv)
{
	t_philosopher *philosophers;
	t_program_status program_status;

	if (argc != 5 && argc != 6)
	{
		printf("Usage: <%s> <number of philosophers> <time to die> <time to eat> <time to sleep>\n", argv[0]);
		return (0);
	}
	pthread_mutex_init(&program_status.mutex_stop, NULL);
	program_status.stop = false;
	program_status.fed_up_philosophers = 0;
	if (!parse_arguments(&philosophers, argv, argc, &program_status))
	{
		printf("Could not parse arguments\n");
		return (1);
	}
	printf("-------------------------------------------------------------------------\n");
	banquet(philosophers);
	printf("-------------------------------------------------------------------------\n");
	if (argc == 6)
		print_philosophers(philosophers);
	free_philosophers(philosophers);
	return (0);
}
