class Game
{
public:
	char board[5][5];
	Chess movableChs[2];
	int chsNumA, chsNumB;
	Chess playerA[6], playerB[6];
	Chess* currentPlayer;
	bool turn;

	//Includes initializing the positions
	Game();
	~Game();

	bool isLegalMove(int x, int y);
	//Synchronize the chess and the board
	void setBoard(char chs);
	//Get the symbol of the chess originally on the board
	char getChessOnBoard(Chess chs);
	//Player move a chess
	char moveChess(Chess chessToGo);
	
	//Roll the dice, and return how many chess the player can move
	int nextMove();
	//Recieve the eaten chess and update the existence, and return 0(game over), 1(A wins), 2(B wins)
	int updatePlayer(char c);
	//Rotate the board and change the pointer of the player
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
