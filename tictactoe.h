#define ERROR    3
#define X_WIN    2
#define TIE      1
#define CONTINUE 0
#define O_WIN   -2

// Contains information specific to present game
typedef struct ttt_board_struct{

	U16 x_board;
	U16 o_board;
	U16 total_board;
	U8  ply;

} ttt_board;

// Contains general tictactoe information
typedef struct ttt_game_struct{

	U16 **victory_masks;
	U8   *num_masks;
	U8    num_squares;
	U8    min_ply;
	U8    max_ply;

} ttt_game;

/*
 *    Bit layout of ttt boards for DIMENSION = 4
 *
 *    0  1  2  3
 *    4  5  6  7
 *    8  9  10 11
 *    12 13 14 15
 *
 *    Bit corresponds to row * DIMENSION + col
 *    We keep it as a bit to avoid the multiplication
 *
 *    For DIMENSION > 4, the data type of ttt boards must be increased
 */

// Board methods
void initBoard (ttt_board**);
void printBoard(ttt_board*);
void freeBoard (ttt_board**);

// Game methods
void initGame(ttt_game**);
void freeGame(ttt_game**);

// Mask functions
void constructVictoryMasks(U16***, U8**, U8);
void checkVictoryMasks    (U16**,  U8*);
void freeVictoryMasks     (U16***, U8**, U8);

// Gameplay functions
S8   makeMove        (ttt_board*, ttt_game*, U8);
S8   makeMoveValidate(ttt_board*, ttt_game*, U8);
S8   undoMove        (ttt_board*,            U8);
S8   undoMoveValidate(ttt_board*,            U8);
S8   checkVictory    (ttt_board*, ttt_game*, U8);
void statusUpdate    (ttt_board*,            S8);
