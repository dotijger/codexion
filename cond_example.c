#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// global variables

int fuel = 0;
pthread_mutex_t	fuel_mtx;
pthread_cond_t	fuel_cond;


void	*fuel_fill(void *arg)
{
	int	i;

	i = -1;
	while (++i < 5)
	{
		pthread_mutex_lock(&fuel_mtx);
		fuel += 15;
		printf("Adding fuel, fuel level: %d\n", fuel);
		pthread_mutex_unlock(&fuel_mtx);
		pthread_cond_signal(&fuel_cond);
		sleep(1);
	}

	return NULL;
}

void	*car(void *arg)
{
	pthread_mutex_lock(&fuel_mtx);
	while (fuel < 40)
	{
		printf("No fuel, waiting...\n");
		pthread_cond_wait(&fuel_cond, &fuel_mtx);
		/* equivalent to:
		 * pthread_mutex_unlock(&fuel_mtx);
		 * wait for signal from cond
		 * pthread_mutex_lock(&fuel_mtx);
		*/
	}
	fuel -= 40;
	printf("Got fuel, fuel level: %d\n", fuel);
	pthread_mutex_unlock(&fuel_mtx);

	return NULL;

}

int	main(void)
{
	pthread_t threads[2];
	int	i;

	if (pthread_mutex_init(&fuel_mtx, NULL) != 0)
	{
		printf("Mutex init problem.");
		exit(EXIT_FAILURE);
	}

	if (pthread_cond_init(&fuel_cond, NULL) != 0)
	{
		printf("Cond init problem.");
		exit(EXIT_FAILURE);
	}
	
	i = -1;
	while (++i < 2)
	{
		if (i == 1)
			if (pthread_create(&threads[i], NULL, &fuel_fill, NULL) != 0)
				return (-1);
		if (i == 0)
			if (pthread_create(&threads[i], NULL, &car, NULL) != 0)
				return (-1);
	}


	i = -1;
	while (++i < 2)
		if (pthread_join(threads[i], NULL) != 0)
			return (-1);

	if (pthread_mutex_destroy(&fuel_mtx) != 0)
	{
		printf("Mutex destroy problem.");
		exit(EXIT_FAILURE);
	}
	if (pthread_cond_destroy(&fuel_cond) != 0)
	{
		printf("Cond destroy problem.");
		exit(EXIT_FAILURE);
	}
	return (0);
}
