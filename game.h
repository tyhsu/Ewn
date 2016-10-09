class Game
{
public:
	char board[5][5];
	Chess movableChs[2];
	int chsNumA, chsNumB;
	Chess playerA[6], playerB[6];
	Chess* currentPlayer;
	//false(0): A, true(1): B
	bool turn;

	//Including initializing the positions
	Game();

	bool isLegalMove(int x, int y);
	//Synchronize the chessman and the board
	void setBoard(char chs);
	//Get the symbol of the chessman originally on the board
	char getChessOnBoard(Chess chs);
	//Player move a chessman
	char moveChess(Chess chessToGo);
	//Get command for moveing a chessman
	char getCommand();
	//Play the game OuO
	void play();
	//Roll the dice, and return how many chessmen the player can move
	int nextMove();
	//Recieve the eaten chessman and update the existence, and return 0(game over), 1(A wins), 2(B wins)
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
	void moveX();
	void moveY();
};
