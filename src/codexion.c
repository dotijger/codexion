/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   codexion.c                                      \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/10 17:57:43 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/10 18:17:31 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */


/*
 * To start the codexion:
 *   1) monitor thread needs to be created (allseeing mother) with respective scheduler strategy (fifo or efi)
 *   2) all threads need to be created
 *   	2a) threads can only begin with their respective "coding" function when all threads have been created
 *   2) all threads need to be joined
 *   3) if all threads are joined, end program, clean up (only happens with a specific number of compiles, otherwise should run indefinitely)
 *   4) if anything happens, safe exit (with cleanup)
 *
*/ 

static void	coding_routine(t_data *data_table)
{
	while (!data_table->start)
		;
	// start routine:
	// try to compile
	// debug for debug time (ms)
	// refactor for refactor time (ms)
}

static void	monitor_routine(t_data *data_table)
{
	// checks whether coders burn out / die
	// assigns the dongles fairly (i am assuming)
}

void	codexion_start(t_data *data_table)
{
	pthread_t	monitor;
	int	i;
	create_monitor(&monitor, &data_table->scheduler); //TODO
	
	i = -1;
	while (++i < data_table->number_of_coders)
		safe_thread_handle(&data_table->coders[i].thread_id, coding_routine, data_table, CREATE);

	// should probably use a cond instead of a spinlock
	data_table->start = true;
	
	i = -1;
	while (++i < data_table->number_of_coders)
		safe_thread_handle(&data_table->coders[i].thread_id, NULL, NULL, JOIN);

	safe_thread_handle(&monitor, NULL, NULL, JOIN);
	// this means all coders have successfully reached their numbers of compiles and the codexion is done
} 
