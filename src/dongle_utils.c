/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   dongle_utils.c                                  \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/14 12:52:10 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/14 12:57:36 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	take_left_dongle(t_mtx *mtx, t_data *data_table, int i)
{
	safe_mutex_handle(mtx, LOCK);
	data_table->dongles[left(i)].taken = true;
	safe_mutex_handle(mtx, UNLOCK);
	log_event("%d has taken a dongle\n", i);
}

void	take_right_dongle(t_mtx *mtx, t_data *data_table, int i)
{
	safe_mutex_handle(mtx, LOCK);
	data_table->dongles[right(i)].taken = true;
	safe_mutex_handle(mtx, UNLOCK);
	log_event("%d has taken a dongle\n", i);
}

void	put_left_dongle_back(t_mtx *mtx, t_data *data_table, int i)
{
	safe_mutex_handle(mtx, LOCK);
	data_table->dongles[left(i)].taken = false;
	safe_mutex_handle(mtx, UNLOCK);
}

void	put_right_dongle_back(t_mtx *mtx, t_data *data_table, int i)
{
	safe_mutex_handle(mtx, LOCK);
	data_table->dongles[right(i)].taken = false;
	safe_mutex_handle(mtx, UNLOCK);
}

