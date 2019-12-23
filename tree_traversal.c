#include <stdio.h>
#include <stdlib.h>
#include "bit_ops.h"
#include "tictactoe.h"
#include "tree_traversal.h"

/*
 *   Fun fact:
 *
 *   In tic tac toe, making a move is never inherently disadvantageous
 *   for a player. That is, there is no move that X can play to 
 *   directly advantage O. Therefore, if we assumed that O had a winning
 *   strategy, X could just use the strategy instead, proving that there
 *   does not exist a winning strategy for O.
 */

// Set up default attributes for node
void initNode(Node **node, ttt_game *game){

	// Allocate
	(*node) = (Node*)malloc(sizeof(Node));

	if ((*node) == NULL){
		printf("Unable to allocate memory for node.\n");
		return;
	}

	// Populate
	(*node)->value = CONTINUE;
	(*node)->children = (Node**)malloc(game->num_squares * sizeof(Node*));

	if ((*node)->children == NULL){
		printf("Unable to allocate memory for node children.\n");
		return;
	}

	for (U8 bit = 0; bit < game->num_squares; bit++)
		(*node)->children[bit] = NULL;
	return;
}

// Allocate memory for transposition table
void initTranspositionTable(Node ****transposition_table, ttt_game *game){

	// Number of unique x_board configurations: 2^{N^2}
	U64 num_states = intExp(2, game->num_squares);

	(*transposition_table) = (Node***)malloc(num_states * sizeof(Node**));

	if ((*transposition_table) == NULL){
		printf("Could not allocate memory for transposition outer-table.\n");
		return;
	}

	for (U64 i = 0; i < num_states; i++){
		(*transposition_table)[i] = (Node**)malloc(num_states * sizeof(Node*));

		if ((*transposition_table)[i] == NULL){
			printf("Could not allocate memory for transposition inner-table.\n");
			return;
		}

		for (U64 j = 0; j < num_states; j++)
			(*transposition_table)[i][j] = NULL;
	}
	return;
}

// Save the environment
void freeTranspositionTable(Node ****transposition_table, ttt_game *game){

	// Number of unique x_board configurations: 2^{N^2}
	U64 num_states = intExp(2, game->num_squares);

	for (U64 i = 0; i < num_states; i++){

		// Free all nodes from the table
		for (U64 j = 0; j < num_states; j++){

			if ((*transposition_table)[i][j] != NULL){

				free((*transposition_table)[i][j]->children);
				free((*transposition_table)[i][j]);
			}
		}

		// Free the table too
		free((*transposition_table)[i]);
	}
	free(*transposition_table);
	return;
}

// Determine the number of distinct games of ttt
void countLeafNodes(ttt_board *board, ttt_game *game, U64 *num_leaves){

	S8 move_result;	

	// Span over all moves
	for (U8 bit = 0; bit < game->num_squares; bit++){

		// Perform move
		move_result = makeMoveValidate(board, game, bit);

		// Ensure it is legal
		if (move_result == ERROR)
			continue;

		// Determine if leaf node or not
		if (move_result != CONTINUE)
			(*num_leaves)++;
		else
			countLeafNodes(board, game, num_leaves);
		
		// Rollback
		undoMove(board, bit);
	}
	return;
}

// Wire the game tree and transposition table
Node* constructGameTree(ttt_board *board, ttt_game *game, Node ***transposition_table, S8 move_result){

	// Isolate current node
	Node** current_node = &(transposition_table[board->x_board][board->o_board]);

	// Prune search if transposition
	if (*current_node != NULL)
		return *current_node;

	// Generate node if new territory
	initNode(current_node, game);

	// Terminate if leaf node
	if (move_result != 0){
		(*current_node)->value = move_result;
		return *current_node;
	}

	// Span over all moves
	for (U8 bit = 0; bit < game->num_squares; bit++){

		// Perform move
		move_result = makeMoveValidate(board, game, bit);

		// Ensure it is legal
		if (move_result == ERROR)
			continue;

		// Get the children
		(*current_node)->children[bit] = constructGameTree(board, game, transposition_table, move_result);

		// Rollback
		undoMove(board, bit);
	}
	return *current_node;
}

// Brute force search the entire game space
S8 fullMinimax(ttt_board *board, ttt_game *game){

	// Our friends
	U8 bit;
	S8 value, outcome;

	// X's to move
	if (getBitU8(board->ply, 0)){

		// O victory code
		value = O_WIN;

		// Span over all moves
		for (bit = 0; bit < game->num_squares; bit++){

			// Perform move
			outcome = makeMoveValidate(board, game, bit);

			// Ensure it is legal
			if (outcome == ERROR)
				continue;

			// Print board status
			statusUpdate(board, outcome);

			// Determine if leaf node or not
			if (outcome != CONTINUE)
				value = outcome;
			else
				value = max(value, fullMinimax(board, game));
			
			// Rollback
			undoMove(board, bit);

			// Prune search if victory or tie
			if (value >= TIE)
				return value;
		}
	}
	// O's to move
	else{

		// X victory code
		value = X_WIN;

		// Span over all moves
		for (bit = 0; bit < game->num_squares; bit++){

			// Perform move
			outcome = makeMoveValidate(board, game, bit);

			// Ensure it is legal
			if (outcome == ERROR)
				continue;

			// Print board status
			statusUpdate(board, outcome);

			// Determine if leaf node or not
			if (outcome != CONTINUE)
				value = outcome;
			else
				value = min(value, fullMinimax(board, game));
			
			// Rollback
			undoMove(board, bit);

			// Prune search if victory or tie
			if (value <= TIE)
				return value;
		}
	}
	return value;
}

// Determine maximum
S8 max(S8 a, S8 b){

	if (a > b)
		return a;
	return b;
}

// Determine minimum
S8 min(S8 a, S8 b){

	if (a < b)
		return a;
	return b;
}

// Integer exponentiation
U64 intExp(U8 base, U8 exponent){

	U64 result = (U64)base;

	for (U8 i = 1; i < exponent; i++)
		result *= base;

	return result;
}
