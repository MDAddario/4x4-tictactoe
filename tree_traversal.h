// Game tree units
typedef struct struct_node{

	S8 value;
	struct struct_node **children;

} Node;

// Node methods
void initNode(Node**, ttt_game*);

// Transposition table methods
void initTranspositionTable(Node****, ttt_game*);
void freeTranspositionTable(Node****, ttt_game*);

// Tree traversal funcitons
void  countLeafNodes   (ttt_board*, ttt_game*, U64*);
Node* constructGameTree(ttt_board*, ttt_game*, Node***, S8);
S8    fullMinimax      (ttt_board*, ttt_game*);

// Helper functions
S8  max   (S8, S8);
S8  min   (S8, S8);
U64 intExp(U8, U8);
