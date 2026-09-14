/* ************************************************************************** */
/*                                                                            */
/*                                                    *        /\       *     */
/*   clean.c                                         \        /##\        /   */
/*                                                    \      /####\      /    */
/*   By: odschreu <odschreu@student.codam.nl>      ===##====#{####}====##==   */
/*                                                        |X||##||X|          */
/*   Created: 2026/09/10 17:31:13 by odschreu             |X||##||X|          */
/*   Updated: 2026/09/10 17:31:43 by odschreu            ..+::##::+..         */
/*                                                                            */
/* ************************************************************************** */



/*
 * When cleaning up:
 * 	- destroy the mutexes
 * 	- destroy the cond (dongle)
 * 	- free the malloced arrays (dongles, coders)
 *
*/
