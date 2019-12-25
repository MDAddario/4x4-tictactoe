// Game tree units
typedef struct struct_node{

	S8 value;
	struct struct_node **children;

} Node;

// Hash functinos
U32  convertBase2to3(U16,       U8);
U32  hashMap        (ttt_game*, U16, U16);
U32  largestHashKey (ttt_game*);
void printHashMap   (ttt_game*);

// Node methods
void initNode(Node**, ttt_game*);

// Transposition table methods
void initTranspositionTable(Node***, ttt_game*);
void freeTranspositionTable(Node***, ttt_game*);

// Tree traversal funcitons
void  countLeafNodes       (ttt_board*, ttt_game*, U64*);
Node* constructGameTree    (ttt_board*, ttt_game*, Node**, S8);
void  gameTreeProgress     (ttt_board*, ttt_game*, Node**);
Node* constructGameTreeProg(ttt_board*, ttt_game*, Node**, S8, U32*, U32);
S8    fullMinimax          (ttt_board*, ttt_game*);

// Helper functions
S8  max   (S8, S8);
S8  min   (S8, S8);
U32 intExp(U8, U8);
