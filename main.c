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
	Node ***transposition_table;
	initTranspositionTable(&transposition_table, game);

	// Get that tree (clock it)
	S64 time = clock();
	Node* game_tree_head = constructGameTree(board, game, transposition_table, CONTINUE);
	time = clock() - time; 
	double seconds = ((double)time) / CLOCKS_PER_SEC;

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

/*
 *	Manuscript sections: --------------------------------------------
 *
 *	- Games of perfect information
 *	- ``Winning Strategies'' and associated theorems
 *
 *	- Bit boards
 *	- Faster system for checking tic tac toes (more than just the inherent speedup)
 *	- Approx 10x speedup
 *
 *	- Transposition tables & game tree
 *	- Bit boards make for SUPER fast table keys
 *	- Transposition results in approx 10x speedup
 *
 *	- Symmetry reductions
 *
 *	-----------------------------------------------------------------
 */
