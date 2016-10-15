Minimax


function minimax(board, height, maximizingPlayer)
	if height==0	//node is a terminal node
		return the feature value of node	//heuristic value

	curBoard := board	//copy
	if maximizingPlayer
		bestValue := -∞
		// according to maximizingPlayer, know which player to play
		for each possible move chs, dirct	//each child of node
			nxtBoard := curBoard	//copy
			nxtBoard.move(chs, dirct)
			val := minimax(nxtBoard, height-1, FALSE)
			bestValue := max(bestValue, val);
		return bestValue
	else
		bestValue := +∞
		for each possible move chs, dirct
			nxtBoard := curBoard	//copy
			nxtBoard.move(chs, dirct)
			val := minimax(nxtBoard, height - 1, TRUE)
			bestValue := min(bestValue, val);
		return bestValue

(* Initial call for maximizing player *)
minimax(origin, height, TRUE)