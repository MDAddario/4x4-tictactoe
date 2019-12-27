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
	Node *game_tree_head = constructGameTree(board, game, transposition_table, FALSE, FALSE, TRUE);
	time = clock() - time;
	double seconds = ((double)time) / CLOCKS_PER_SEC;

	// Print results
	printf("/-----------------------\\\n");
	printf("Dimension: \t%dx%d\n", DIMENSION, DIMENSION);
	printf("Run time: \t%.5f s\n", seconds);
	printf("\\-----------------------/\n");


	// Play against a perfect X_player
	Node *node = game_tree_head;
	for(U8 bit;;){

		// Print board information
		printBoard(board);
		printf("Value: %hhi\n", node->value);

		// Player's turn
		if (getBitU8(board->ply, 0) == 0){

			// Retrieve input
			bit = inputU8("Bit to play");

			// Check tree if move is legal
			if (node->children[bit] == NULL){
				printf("Bit %hhu not valid.\n", bit);
				continue;
			}
		}
		// Computers turn
		else{

			S8 best_bit = -1;
			U8 best_points = 0;
			U8 points, computer_bit, human_bit;

			for (computer_bit = 0; computer_bit < game->num_squares; computer_bit++){

				if (node->children[computer_bit] != NULL){

					// Accept if computer wins
					if (node->children[computer_bit]->value == X_WIN){
						best_bit = computer_bit;
						break;
					}

					// Ignore if computer loses
					if (node->children[computer_bit]->value == O_WIN)
						continue;

					// Count the number of ways the human player can lose on next move
					points = 0;
					for (human_bit = 0; human_bit < game->num_squares; human_bit++){
						if (node->children[computer_bit]->children[human_bit] != NULL)
							if (node->children[computer_bit]->children[human_bit]->value == X_WIN)
								points++;
					}
				}

				// Find the best computer move
				if (points >= best_points){
					best_bit = computer_bit;
					best_points = points;
				}
			}

			bit = best_bit;

			// Avoid segfaults
			if (bit == -1)
				break;
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
 *	- Create function to show connectivity between trans_table and game_tree
 *	- Make function to make computer play perfectly
 *		- Random move selection?
 *		- Optimize winning chances
 *	- Include symmetry reductions
 *
 *   ----------------------------------------------------------------
 */
