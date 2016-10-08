class Game
{
public:
	char board[5][5];
	int chsNumA, chsNumB;
	Chess playerA[6], playerB[6];
	Chess* currentPlayer;
	bool turn;

 	// coor(get,set)
	Game();	//includes initializing the positions
	~Game();

	bool isLegalMove(int x, int y);
	void setBoard(char chs);
	char getChessOnBoard(Chess chs);
	char moveChess(Chess chessToGo);
	
	int rollDice();
	int updatePlayer(char c);
	void switchPlayer();

	void printBoard();	
};

struct Chess
{	
	char symbol;
	bool exist;
	int x, y;
	int getX();
	int getY();
	void moveX();
	void moveY();
};
