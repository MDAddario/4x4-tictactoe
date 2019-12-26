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
	Node *game_tree_head = gameTreeProgress(board, game, transposition_table, 1, 1);
	time = clock() - time;
	double seconds = ((double)time) / CLOCKS_PER_SEC;

	// Print results
	printf("/-----------------------\\\n");
	printf("Dimension: \t%dx%d\n", DIMENSION, DIMENSION);
	printf("Run time: \t%.5f s\n", seconds);
	printf("\\-----------------------/\n");

	// Begin debugging chamber!
	Node* node = game_tree_head;
	U8 bit;

	for(;;){

		// Print information
		printBoard(board);
		printf("Value: %hhi\n", node->value);

		// Retrieve input
		bit = inputU8("Bit to play");

		// Check tree if route is possible
		if (node->children[bit] == NULL){
			printf("Bit %hhu not valid.\n", bit);
			continue;
		}

		// Progress game
		makeMove(board, game, bit);
		node = node->children[bit];
	}

	// We care about the environment
	freeTranspositionTable(&transposition_table, game);
	freeGame(&game);
	freeBoard(&board);

	return 0;
}

/*   
 *   TODO LIST: -----------------------------------------------------
 *
 *	- Add toggle for progress bar
 *	- Create function to show connectivity between trans_table and game_tree
 *	- Include optimal play pruning
 *	- Make function to make computer play perfectly
 *		- Random move selection?
 *		- Optimize winning chances
 *	- Include symmetry reductions
 *
 *   ----------------------------------------------------------------
 */
