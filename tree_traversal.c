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

// Convert from representation in base 2 to the same rep. in base 3
U32 convertBase2to3(U16 base2, U8 num_bits){

	U32 base3 = 0;

	// Read off each bit
	for (U8 bit = 0; bit < num_bits; bit++)

		if (getBitU16(base2, bit))
			base3 += intExp(3, bit);

	return base3;
}

// Injective hash function from x_board and o_board
U32 hashMap(ttt_game *game, U16 x_board, U16 o_board){

	return convertBase2to3(x_board, game->num_squares) 
		+ 2 * convertBase2to3(o_board, game->num_squares);
}

// Detemine the largest hash key that can be had
U32 largestHashKey(ttt_game *game){

	// Middle bit
	U8 threshhold_bit = (game->num_squares - 1) / 2 + 1;

	// Build x_board
	U16 x_board = 0x0000;
	for (U8 bit = 0; bit < threshhold_bit; bit++)
		setBitU16(&x_board, bit, 1);

	// Build o_board
	U16 o_board = 0x0000;
	for (U8 bit = threshhold_bit; bit < game->num_squares; bit++)
		setBitU16(&o_board, bit, 1);

	return hashMap(game, x_board, o_board);
}

// Plot hash map combinations
void printHashMap(ttt_game *game){

	// Open the file
	char* filename = "hashmap.txt";
	FILE* my_file = (FILE*)malloc(sizeof(FILE));
	my_file = fopen(filename, "w+");
	if (my_file == NULL){
		printf("File %s could not be opened.\n", filename);
		return;
	}

	// Occupy the file
	U32 hash;
	U32 max_board = intExp(2, game->num_squares);
	
	for (U16 o_board = 0; o_board < max_board; o_board++){
		for (U16 x_board = 0; x_board < max_board; x_board++){
			hash = hashMap(game, x_board, o_board);
			fprintf(my_file, "%u, %u, %u\n", x_board, o_board, hash);
		}
	}
	fclose(my_file);
	return;
}

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
void initTranspositionTable(Node ***transposition_table, ttt_game *game){

	// Maximal number of possible hash keys
	U32 max_key = largestHashKey(game);

	(*transposition_table) = (Node**)malloc((max_key + 1) * sizeof(Node*));

	if ((*transposition_table) == NULL){
		printf("Could not allocate memory for transposition table.\n");
		return;
	}

	// Set all defaults to NULL
	for (U32 key = 0; key <= max_key; key++)
		(*transposition_table)[key] = NULL;
	
	return;
}

// Save the environment
void freeTranspositionTable(Node ***transposition_table, ttt_game *game){

	// Maximal number of possible hash keys
	U32 max_key = largestHashKey(game);

	// Free all nodes from the table
	for (U32 key = 0; key <= max_key; key++)

		if ((*transposition_table)[key] != NULL){
			free((*transposition_table)[key]->children);
			free((*transposition_table)[key]);
		}

	// Free the table too
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

// User call to wire game tree and transposition table
Node* constructGameTree(ttt_board *board, ttt_game *game, Node **transposition_table,
						U8 is_x_optimal, U8 is_o_optimal, U8 is_print_progress){

	// Leaves
	U32 num_leaves = 0;
	U32 max_leaves;

	// Sorry lol
	if (DIMENSION == 4)
		max_leaves = 12870;		// (16 choose 8)
	else if (DIMENSION == 3)
		max_leaves = 126;		// (9 choose 4)
	else if (DIMENSION == 2)
		max_leaves = 6;			// (4 choose 2)

	// Construct tree
	return backgroundGameTree(board, game, transposition_table, 
							is_x_optimal, is_o_optimal, is_print_progress,
							CONTINUE, &num_leaves, max_leaves);
}

// Backend call to wire game tree and transposition table
Node* backgroundGameTree(ttt_board *board, ttt_game *game, Node **transposition_table, 
						U8 is_x_optimal, U8 is_o_optimal, U8 is_print_progress,
						S8 move_result, U32 *num_leaves, U32 max_leaves){

	// Isolate current node
	U32 hash_key = hashMap(game, board->x_board, board->o_board);
	Node** current_node = &(transposition_table[hash_key]);

	// Prune search if transposition
	if (*current_node != NULL)
		return *current_node;

	// Print progress
	if (is_print_progress)
		if (board->ply == game->max_ply){
			(*num_leaves)++;
			U8 progression = (*num_leaves) * 100 / max_leaves;
			printf("%3.hhu %%\n", progression);
		}

	// Generate node if new territory
	initNode(current_node, game);

	// Terminate if leaf node
	if (move_result != CONTINUE){
		(*current_node)->value = move_result;
		return *current_node;
	}

	// Default node value
	if (getBitU8(board->ply, 0))
		(*current_node)->value = O_WIN;
	else
		(*current_node)->value = X_WIN;

	// Span over all moves
	for (U8 bit = 0; bit < game->num_squares; bit++){

		// Perform move
		move_result = makeMoveValidate(board, game, bit);

		// Ensure it is legal
		if (move_result == ERROR)
			continue;

		// Get the children
		(*current_node)->children[bit] = backgroundGameTree(board, game, 
				transposition_table, is_x_optimal, is_o_optimal, is_print_progress,
				move_result, num_leaves, max_leaves);

		// Rollback
		undoMove(board, bit);

		// Find optimal score, prune if best path
		if (getBitU8(board->ply, 0)){

			if (is_x_optimal)
				if ((*current_node)->children[bit]->value == X_WIN){
					(*current_node)->value = X_WIN;
					break;
				}

			(*current_node)->value = max((*current_node)->value,
							(*current_node)->children[bit]->value);
		}
		else{

			if (is_o_optimal)
				if ((*current_node)->children[bit]->value == O_WIN){
					(*current_node)->value = O_WIN;
					break;
				}

			(*current_node)->value = min((*current_node)->value,
							(*current_node)->children[bit]->value);
		}
	}
	return *current_node;
}

// Enter the debugging chamber
void practiceGame(ttt_board *board, ttt_game *game, Node *node){

	// Quick check
	if (node == NULL){
		printf("Game tree head invalid.\n");
		return;
	}

	for(U8 bit;;){

		// Print information
		printBoard(board);
		printf("Value: %hhi\n", node->value);

		// Retrieve input
		bit = inputU8("Bit to play");

		// Check tree if move is legal
		if (node->children[bit] == NULL){
			printf("Bit %hhu not valid.\n", bit);
			continue;
		}

		// Progress game
		makeMove(board, game, bit);
		node = node->children[bit];
	}
	// Not that you could ever reach this
	return;
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
U32 intExp(U8 base, U8 exponent){

	U32 result = 1;

	for (U8 i = 0; i < exponent; i++)
		result *= base;

	return result;
}

// Retrieve number from char stdio
U8 inputU8(U8 *prompt){

	U8 input[3];
	U8 dummy;

	// Request input
	printf("%s: ", prompt);
	scanf("%s", input);

	// Clear buffer
	while ((dummy = getchar()) != '\n' && dummy != EOF){}
	return atoi(input);
}
