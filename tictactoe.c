#include <stdio.h>
#include <stdlib.h>
#include "bit_ops.h"
#include "tictactoe.h"

// Set up the ttt board
void initBoard(ttt_board **board){

	// Allocate
	(*board) = (ttt_board*)malloc(sizeof(ttt_board));

	// Populate
	(*board)->x_board     = 0x0000;
	(*board)->o_board     = 0x0000;
	(*board)->total_board = 0x0000;
	(*board)->ply         = 0x01;

	return;
}

// Visualize the ttt board
void printBoard(ttt_board *board){

	U8 row, col;

	printf("/");
	for (row = 0; row < DIMENSION; row++)
		printf("---");
	printf("\\\n");

	for (row = 0; row < DIMENSION; row++){

		printf("|");

		for (col = 0; col < DIMENSION; col++){

			if (getBitU16(board->x_board, row * DIMENSION + col))
				printf(" x ");
			else if (getBitU16(board->o_board, row * DIMENSION + col))
				printf(" o ");
			else
				printf(" . ");
		}
		printf("|\n");
	}

	printf("\\");
	for (row = 0; row < DIMENSION; row++)
		printf("---");
	printf("/\n");

	return;
}

// Save the environment
void freeBoard(ttt_board **board){

	free(*board);
	return;
}

// Set up the ttt game
void initGame(ttt_game **game){

	// Allocate
	(*game) = (ttt_game*)malloc(sizeof(ttt_game));

	// Populate
	(*game)->num_squares = DIMENSION * DIMENSION;
	constructVictoryMasks(
						&((*game)->victory_masks),
						&((*game)->num_masks),
						(*game)->num_squares);
	/*checkVictoryMasks(
					(*game)->victory_masks,
					(*game)->num_masks);*/

	// Bounds on when to check for a win
	(*game)->min_ply = 2 * DIMENSION;
	(*game)->max_ply = (*game)->num_squares + 1;

	return;
}

// Save the environment
void freeGame(ttt_game **game){

	freeVictoryMasks(
					&((*game)->victory_masks),
					&((*game)->num_masks),
					(*game)->num_squares);
	free(*game);
	return;
}

// Configure masks for when checking if tictactoe
void constructVictoryMasks(U16 ***victory_masks, U8 **num_masks, U8 num_squares){
	
	// Define our friends
	U8 outer_row, outer_col, outer_bit;
	U8 inner_row, inner_col, inner_bit;
	
	// Access masks as: (*victory_masks)[outer_bit][inner_bit]
	// Access nums as:  (*num_masks)[outer_bit]
	
	// Allocate memory for victory masks and num masks
	(*num_masks) = (U8*)calloc(num_squares, sizeof(U8));
	(*victory_masks) = (U16**)malloc(num_squares * sizeof(U16*));
	
	for (outer_bit = 0; outer_bit < num_squares; outer_bit++){
		(*victory_masks)[outer_bit] = (U16*)malloc(4 * sizeof(U16));
		
		// 1-fill all the masks
		for (inner_bit = 0; inner_bit < 4; inner_bit++)
			(*victory_masks)[outer_bit][inner_bit] = 0xffff;
	}
	
	// Run over all board squares
	for (outer_row = 0; outer_row < DIMENSION; outer_row++)
		for (outer_col = 0; outer_col < DIMENSION; outer_col++){
			
			outer_bit = outer_row * DIMENSION + outer_col;
			
			// Configure horizontal ttt checks
			for (inner_col = 0; inner_col < DIMENSION; inner_col++){
				
				inner_bit = outer_row * DIMENSION + inner_col;
				setBitU16((*victory_masks)[outer_bit] + (*num_masks)[outer_bit], inner_bit, 0);
			}
			(*num_masks)[outer_bit]++;
			
			// Configure vertical ttt checks
			for (inner_row = 0; inner_row < DIMENSION; inner_row++){
				
				inner_bit = inner_row * DIMENSION + outer_col;
				setBitU16((*victory_masks)[outer_bit] + (*num_masks)[outer_bit], inner_bit, 0);
			}
			(*num_masks)[outer_bit]++;
			
			// Configure diagonal ttt checks
			if (outer_row == outer_col){
				
				for (inner_row = 0; inner_row < DIMENSION; inner_row++){
				
					inner_bit = inner_row * (DIMENSION + 1);
					setBitU16((*victory_masks)[outer_bit] + (*num_masks)[outer_bit], inner_bit, 0);
				}
				(*num_masks)[outer_bit]++;
			}
			
			// Configure anti-diagonal ttt checks
			if (outer_row + outer_col == DIMENSION - 1){
				
				for (inner_row = 0; inner_row < DIMENSION; inner_row++){
				
					inner_bit = (inner_row + 1) * (DIMENSION - 1);
					setBitU16((*victory_masks)[outer_bit] + (*num_masks)[outer_bit], inner_bit, 0);
				}
				(*num_masks)[outer_bit]++;
			}
		}
	return;
}

// Ensure that the victory masks are properly constructed
void checkVictoryMasks(U16 **victory_masks, U8 *num_masks){
	
	// Our big helpers
	U8 outer_row, outer_col, outer_bit, counter;
	
	// Run over all board squares
	for (outer_row = 0; outer_row < DIMENSION; outer_row++)
		for (outer_col = 0; outer_col < DIMENSION; outer_col++){
			
			outer_bit = outer_row * DIMENSION + outer_col;
			
			// Print number of masks
			printf("(row, col) = (%hhu, %hhu) has %hhu masks.\n", outer_row, outer_col, num_masks[outer_bit]);
			
			// Print all the masks
			for (counter = 0; counter < num_masks[outer_bit]; counter++)
				printU16(victory_masks[outer_bit][counter]);
		}
	return;
}

// Save the environment!
void freeVictoryMasks(U16 ***victory_masks, U8 **num_masks, U8 num_squares){
	
	for (U8 outer_bit = 0; outer_bit < num_squares; outer_bit++)
		free((*victory_masks)[outer_bit]);
	
	free(*victory_masks);
	free(*num_masks);
	
	return;
}

/* Return codes:
 * -2: Victory for the O's
 *  0: Game continues
 *  1: Game tied and over
 *  2: Victory for the X's
 *  3: Error in decision
 */

// Perform an action on the board
S8 makeMove(ttt_board *board, ttt_game *game, U8 bit){

	// Determine whose move to play, then make move
	if (getBitU8(board->ply, 0))
		setBitU16(&(board->x_board), bit, 1);
	else
		setBitU16(&(board->o_board), bit, 1);
	setBitU16(&(board->total_board), bit, 1);

	// Increment turn
	(board->ply)++;

	// Check for end of game
	return checkVictory(board, game, bit);
}

// Perform an action on the board after validating move
S8 makeMoveValidate(ttt_board *board, ttt_game *game, U8 bit){

	// Check for empty space
	if (getBitU16(board->total_board, bit))
		return 3;

	// Make move
	return makeMove(board, game, bit);
}

// Retract an action on the board
S8 undoMove(ttt_board *board, U8 bit){

	// Determine whose move to undo, then undo move
	if (getBitU8(board->ply, 0))
		setBitU16(&(board->o_board), bit, 0);
	else
		setBitU16(&(board->x_board), bit, 0);
	setBitU16(&(board->total_board), bit, 0);

	// Decrement ply
	(board->ply)--;

	return CONTINUE;
}

// Retract an action on the board after validating action
S8 undoMoveValidate(ttt_board *board, U8 bit){

	// Ensure space is occupied
	if (!getBitU16(board->total_board, bit))
		return ERROR;

	// Undo move
	return undoMove(board, bit);
}

// Check if game has terminated
S8 checkVictory(ttt_board *board, ttt_game *game, U8 bit){

	// Terminate immediately if not far enough in game
	if (board->ply < game->min_ply)
		return CONTINUE;

	// Check for tie
	if (board->ply == game->max_ply)
		return TIE;

	// Determine who just played
	U16 active_board;
	S8 victory_code;

	if (getBitU8(board->ply, 0)){
		active_board = board->o_board;
		victory_code = O_WIN;
	}
	else{
		active_board = board->x_board;
		victory_code = X_WIN;
	}

	// Check for a tictactoe
	for (U8 counter = 0; counter < (game->num_masks[bit]); counter++)

		if ((active_board | game->victory_masks[bit][counter]) == 0xffff)
			return victory_code;

	// Game continues
	return CONTINUE;
}

// Provide a status update on board
void statusUpdate(ttt_board *board, S8 outcome){

	if (outcome == O_WIN)
		printf("The O's have won!\n");

	else if (outcome == CONTINUE)
		printf("The game continues.\n");

	else if (outcome == TIE)
		printf("The game has ended in a tie.\n");

	else if (outcome == X_WIN)
		printf("The X's have won!\n");

	else if (outcome == ERROR)
		printf("There was an error with your move.\n");

	else
		printf("We don't know what happened here. Contact the dev.\n");

	printBoard(board);
	return;
}
