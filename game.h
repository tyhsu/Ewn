class game
{
public:
 	char board[5][5];
 	chess playerA[6]; 
 	chess playerB[6];
 	chess* currentPlayer;

 	// move
 	// turn
 	// coor(get,set)
	game();
	~game();
};

struct chess
{	
	bool exist;
	int x, y;
};
