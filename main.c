#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "bit_ops.h"
#include "tictactoe.h"
#include "tree_traversal.h"

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
	gameTreeProgress(board, game, transposition_table);
	//Node *game_tree_node = constructGameTree(board, game, transposition_table, CONTINUE);
	time = clock() - time;
	double seconds = ((double)time) / CLOCKS_PER_SEC;

	printf("IT DOES NOT MAKE SENSE TO HAVE >100%% PROGRESS.\n");

	// Print results
	printf("Dimension: \t%dx%d\n", DIMENSION, DIMENSION);
	printf("Run time: \t%.5f s\n", seconds);

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
 *	- Make the constructGameTree function work with node values
 *	- Delete fullMinimax
 *	- Include symmetry reductions
 *
 *   ----------------------------------------------------------------
 */
