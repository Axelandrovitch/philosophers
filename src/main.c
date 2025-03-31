#include "../include/philosophers.h"


void	banquet(t_philosopher *philosophers)
{
	create_threads(philosophers);
	join_threads(philosophers);
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
	print_philosophers(philosophers);
	free_philosophers(philosophers);
	return (0);
}
