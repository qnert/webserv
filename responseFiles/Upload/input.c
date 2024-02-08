/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: flfische <flfische@student.42heilbronn.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/27 10:14:13 by flfische          #+#    #+#             */
/*   Updated: 2024/01/28 19:52:45 by flfische         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "board.h"

/**
 * @brief Gets the size of the board.
 *
 * @param str The input string.
 * @return Returns the size of the board.
 */
int	get_size(char *str)
{
	int	i;
	int	size;

	i = 0;
	size = 0;
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
			size++;
		i++;
	}
	return (size / 4);
}

/**
 * @brief Checks if the input is valid.
 *
 * @param argc The number of arguments.
 * @param argv The arguments.
 * @param size The size of the board.
 * @return Returns 1 if the input is invalid, 0 otherwise.
 */
int	check_input(int argc, char **argv, int size)
{
	int		i;
	char	*str;

	if (argc != 2 || size < 1 || size > 9 || ft_count_numbers(argv[1],
			size) != 1)
	{
		write(1, "Error\n", 6);
		return (1);
	}
	i = 0;
	str = argv[1];
	while (str[i])
	{
		if ((i % 2 == 1 && str[i] != ' ') || (i % 2 == 0 && !(str[i] >= '1'
					&& str[i] <= size + '0')))
		{
			write(1, "Error\n", 6);
			return (1);
		}
		i++;
	}
	return (0);
}

/**
 * @brief Counts the numbers in the input.
 *
 * also checks if the input is the correct length
 * and if the numbers are in the correct range.
 *
 * @param str The input string.
 * @param size The size of the board.
 * @return Returns 1 if the input is valid, 0 otherwise.
 */
int	ft_count_numbers(char *str, int size)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (str[i])
	{
		if (str[i] >= '1' && str[i] <= size + '0')
			count++;
		i++;
	}
	if (i == size * 8 - 1 && count == size * 4)
		return (1);
	return (0);
}

/**
 * @brief Parses the input into the board structure.
 *
 * @param str The input string.
 * @param board The board structure.
 * @param size The size of the board.
 */
void	parse_input(char *str, t_board *board, int size)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i] != '\0')
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			if (j < size)
				board->up[j] = str[i] - '0';
			else if (j < size * 2)
				board->down[j - size] = str[i] - '0';
			else if (j < size * 3)
				board->left[j - size * 2] = str[i] - '0';
			else if (j < size * 4)
				board->right[j - size * 3] = str[i] - '0';
			j++;
		}
		i++;
	}
}

