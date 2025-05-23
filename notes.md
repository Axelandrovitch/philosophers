• Each philosopher should be a thread.

Understand what is a thread!

• There is one fork between each pair of philosophers. Therefore, if there are several
philosophers, each philosopher has a fork on their left side and a fork on their right
side. If there is only one philosopher, there should be only one fork on the table.

• To prevent philosophers from duplicating forks, you should protect the forks state
with a mutex for each of them.

Undertand mutex!

## Global rules
You have to write a program for the mandatory part and another one for the bonus part
(if you decide to do the bonus part). They both have to comply with the following rules:

• Global variables are forbidden!

• Your(s) program(s) should take the following arguments:
number_of_philosophers time_to_die time_to_eat time_to_sleep
[number_of_times_each_philosopher_must_eat]
	◦ number_of_philosophers: The number of philosophers and also the number
	of forks.
	◦ time_to_die (in milliseconds): If a philosopher didn’t start eating time_to_die
	milliseconds since the beginning of their last meal or the beginning of the sim-
	ulation, they die.
	◦ time_to_eat (in milliseconds): The time it takes for a philosopher to eat.
	During that time, they will need to hold two forks.
	◦ time_to_sleep (in milliseconds): The time a philosopher will spend sleeping.
	◦ number_of_times_each_philosopher_must_eat (optional argument): If all
	philosophers have eaten at least number_of_times_each_philosopher_must_eat
	times, the simulation stops. If not specified, the simulation stops when a
	philosopher dies.

• Each philosopher has a number ranging from 1 to number_of_philosophers.

• Philosopher number 1 sits next to philosopher number number_of_philosophers.
Any other philosopher number N sits between philosopher number N - 1 and philoso-
pher number N + 1.

About the logs of your program:
• Any state change of a philosopher must be formatted as follows:
	◦ timestamp_in_ms X has taken a fork
	◦ timestamp_in_ms X is eating
	◦ timestamp_in_ms X is sleeping
	◦ timestamp_in_ms X is thinking
	◦ timestamp_in_ms X died
Replace timestamp_in_ms with the current timestamp in milliseconds
and X with the philosopher number.

• A displayed state message should not be mixed up with another message.

• A message announcing a philosopher died should be displayed no more than 10 ms
after the actual death of the philosopher.

• Again, philosophers should avoid dying!

Your program must not have any data races.

./philo 4 310 200 100
-------------------------------------------------------------------------
timestamp: 0 ms philosopher 1 took philosopher's 4 fork
timestamp: 0 ms philosopher 1 took his fork
timestamp: 0 ms philosopher 1 is eating
timestamp: 0 ms philosopher 2 took his fork
timestamp: 200 ms philosopher 1 released his fork
timestamp: 200 ms philosopher 1 released philosopher's 4 fork
timestamp: 200 ms philosopher 1 is sleeping
timestamp: 200 ms philosopher 2 took philosopher's 1 fork
timestamp: 200 ms philosopher 2 is eating
timestamp: 200 ms philosopher 4 took his fork
timestamp: 200 ms philosopher 4 took philosopher's 3 fork
timestamp: 200 ms philosopher 4 is eating
timestamp: 300 ms philosopher 1 is thinking
timestamp: 400 ms philosopher 2 released his fork
timestamp: 400 ms philosopher 2 released philosopher's 1 fork
timestamp: 400 ms philosopher 2 is sleeping
timestamp: 400 ms philosopher 3 took philosopher's 2 fork
timestamp: 400 ms philosopher 4 released his fork
timestamp: 400 ms philosopher 4 released philosopher's 3 fork
timestamp: 400 ms philosopher 4 is sleeping
timestamp: 400 ms philosopher 1 took philosopher's 4 fork
timestamp: 400 ms philosopher 1 took his fork
timestamp: 400 ms philosopher 1 died
timestamp: 400 ms philosopher 3 died

ceci est problèmatique, un seul philosophe devrait mourir.
Pourquoi ?
Faudrait-il gérer différemment le check de l'état de vie des philosophes ?
