#include "../include/philosophers.h"
#include <pthread.h>

static int	ft_isdigit(int c)
{
	if (c >= 48 && c <= 57)
		return (1);
	return (0);
}

static bool	is_num(const char *str)
{
	size_t	i;

	i = 0;
	if (!str || str[0] == '\0')
		return (false);
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			return (false);
		}
		i++;
	}
	return (true);
}

bool	is_valid_str(const char *str)
{
	if (!(str[0] == '+' || str[0] == '-' || ft_isdigit(str[0])))
		return (false);
	if ((str[0] == '+' || str[0] == '-') && !ft_isdigit(str[1]))
		return (false);
	if (str[1] && !is_num(str + 1))
		return (false);
	return (true);
}

size_t	ft_strlen(const char *str)
{
	size_t	len;

	len = 0;
	while (str[len])
		len++;
	return (len);
}

long	ft_atol(const char *nptr)
{
	int		i;
	int		sign;
	long		n;

	i = 0;
	sign = 1;
	n = 0;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	if (nptr[i] == '+' || nptr[i] == '-')
	{
		if (nptr[i] == '-')
			sign = -sign;
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		n = n * 10 + nptr[i] - '0';
		i++;
	}
	return (n * sign);
}

t_philosopher	*init_philosopher(int id, t_philo *philo)
{
	t_philosopher *philosopher;

	philosopher = malloc(sizeof(t_philosopher));
	if (!philosopher)
		return NULL;
	philosopher->id = id;
	philosopher->is_eating = false;
	philosopher->is_dead = false;
	philosopher->is_sleeping = false;
	philosopher->is_thinking = false;
	philosopher->time_to_sleep = philo->time_to_sleep;
	philosopher->time_to_die = philo->time_to_die;
	philosopher->time_to_eat = philo->time_to_eat;
	philosopher->last_meal = 0;
	philosopher->left = NULL;
	philosopher->right = NULL;
	pthread_mutex_init(&philosopher->fork, NULL);
	return (philosopher);
}

void	*free_philosophers(t_philosopher *head)
{
	t_philosopher	*tmp;
	t_philosopher	*current;

	current = head;
	while (current->left != head)
	{
		tmp = current;
		current = current->left;
		pthread_mutex_destroy(&current->fork);
		free(tmp);
	}
	free(current);
	return (NULL);
}

void	add_node_to_list(t_philosopher **head, t_philosopher **tail, t_philosopher *new_node)
{
		if (!*head)
		{
			*head = new_node;
			new_node->left= new_node;
			new_node->right = new_node;
			*tail = new_node;
		}
		else
		{
			(*tail)->left = new_node;
			new_node->right = *tail;
			new_node->left = *head;
			(*head)->right = new_node;
			*tail = new_node;
		}
}

t_philosopher	*init_philosophers(t_philo *philo, t_program_status *program)
{
	t_philosopher *head;
	t_philosopher *tail;
	t_philosopher *new_node;
	int	i;
	long	start_time;

	head = NULL;
	tail = NULL;
	i = 1;
	start_time = set_timestamp();
	while (i <= philo->number_of_philosophers)
	{
		new_node = init_philosopher(i, philo);
		if (!new_node)
			return (free_philosophers(head));
		new_node->start_time = start_time;
		new_node->last_meal= start_time;
		new_node->program = program;
		add_node_to_list(&head, &tail, new_node);
		i++;
	}
	return (head);
}

void	print_philosophers(t_philo *philo,t_philosopher *head)
{
	t_philosopher *current;
	current = head;
	int	i;
	i = 0;
	while (i < philo->number_of_philosophers)
	{
		printf("I am philosopher ID: %d\n", current->id);
		printf("On my left is philosopher ID: %d\n", current->left->id);
		printf("On my right is philosopher ID: %d\n", current->right->id);
		current = current->left;
		i++;
	}
}

bool	parse_arguments(t_philo *philo, char **argv, t_program_status *program)
{
	t_philosopher 	*philosophers;

	if (!is_valid_str(argv[1]) || !is_valid_str(argv[2]) || !is_valid_str(argv[3]))
		return (false);
	philo->number_of_philosophers = ft_atol(argv[1]);
	philo->time_to_die = ft_atol(argv[2]);
	philo->time_to_eat= ft_atol(argv[3]);
	philo->time_to_sleep = ft_atol(argv[4]);
	if (philo->number_of_philosophers < 1 || philo->time_to_die < 0 || philo->time_to_sleep < 0)
		return (false);
	philosophers = init_philosophers(philo, program);
	if (!philosophers)
		return (false);
	philo->philosophers = philosophers;
	return (true);
}
