#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bit_ops.h"
#include "tictactoe.h"
#include "tree_traversal.h"

#define TRUE  1
#define FALSE 0

// The main attraction
int main(){

	// Setup ttt board
	ttt_board *board;
	initBoard(&board);

	// Setup ttt game
	ttt_game *game;
	initGame(&game);

	// Setup trans_table
	Node **transposition_table;
	initTranspositionTable(&transposition_table, game);

	// Get that tree (clock it)
	S64 time = clock();
	Node *game_tree_head = constructGameTree(board, game, transposition_table, TRUE, TRUE, TRUE);
	time = clock() - time;
	double seconds = ((double)time) / CLOCKS_PER_SEC;

	// Print results
	printf("/-----------------------\\\n");
	printf("Dimension: \t%dx%d\n", DIMENSION, DIMENSION);
	printf("Run time: \t%.5f s\n", seconds);
	printf("\\-----------------------/\n");

	// Debug!
	practiceGame(board, game, game_tree_head);

	// We care about the environment
	freeTranspositionTable(&transposition_table, game);
	freeGame(&game);
	freeBoard(&board);

	return 0;
}

/*   
 *   TODO LIST: -----------------------------------------------------
 *
 *	- Create function to show connectivity between trans_table and game_tree
 *	- Make function to make computer play perfectly
 *		- Random move selection?
 *		- Optimize winning chances
 *	- Include symmetry reductions
 *
 *   ----------------------------------------------------------------
 */
