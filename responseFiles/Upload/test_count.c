/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_count.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flfische <flfische@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 15:23:45 by jjerosch          #+#    #+#             */
/*   Updated: 2024/01/28 11:06:19 by flfische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "board.h"

/**
 * @brief Checks if given row includes every number once.
 * 
 * 0 is ignored as it is a placeholder for an empty field.
 * 
 * @param board The game board.
 * @param pos The position to be tested.
 * @return Returns 1 if the row is valid, 0 otherwise.
 */
int	test_count_row(t_board *board, int pos)
{
	int	i;
	int	j;
	int	row;

	row = pos / board->size;
	i = 0;
	while (i < board->size)
	{
		j = i + 1;
		while (j < board->size)
		{
			if (board->board[row][i] == board->board[row][j]
				&& board->board[row][i] != 0)
			{
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

/**
 * @brief Checks if given column includes every number once.
 * 
 * 0 is ignored as it is a placeholder for an empty field.
 * 
 * @param board The game board.
 * @param pos The position to be tested.
 * @return Returns 1 if the column is valid, 0 otherwise.
 */
int	test_count_col(t_board *board, int pos)
{
	int	i;
	int	j;
	int	col;

	col = pos % board->size;
	i = 0;
	while (i < board->size)
	{
		j = i + 1;
		while (j < board->size)
		{
			if (board->board[i][col] == board->board[j][col]
				&& board->board[i][col] != 0)
			{
				return (0);
			}
			j++;
		}
		i++;
	}
	return (1);
}

