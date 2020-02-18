#include <iostream>
#include <iomanip>
using namespace std;

const int NUMROW = 8, NUMCOL = 8;
const int MAXLEN = 80;

// array to store the row change along 8 directions
const int row_change[8] = {-1, -1, 0, 1, 1, 1, 0, -1};
const int col_change[8] = {0, 1, 1, 1, 0, -1, -1, -1};

// function prototypes:
int countFlippableChess(char [NUMROW][NUMCOL], int, int, bool, int, int, int, int);
bool opponentCanFlipBack(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int input_row, int input_col, int x_dir, int y_dir);


char getChess(bool p1_turn){
	return (p1_turn) ? 'X' : 'O';
}

void initBoard(char board[NUMROW][NUMCOL], int row, int col){
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			board[i][j] = ' ';
		}
	}
	board[3][4] = board[4][3] = 'X';
	board[3][3] = board[4][4] = 'O';
}

void printLetterTitles(char board[NUMROW][NUMCOL], int row, int col){
	cout << setw(1) << " ";
	for (int j = 0; j < col; j++){
		cout << setw(4) << (char)(j + 'a');
	}
	cout << endl;
}

void printBoundary(char board[NUMROW][NUMCOL], int row, int col){
	cout << setw(2) << " ";
	for (int j = 0; j < col; j++){
		cout << setw(4) << "---";
	}
	cout << endl;
}

// print the i-th row in the chess board
void printChessRow(char board[NUMROW][NUMCOL], int row, int col, int i){
	cout << i << " |";
	for (int j = 0; j < col; j++){
		cout << " " << board[i][j] << " |";
	}
	cout << " " << i;
}

// count the number of chesses / empty space in the whole chess board
int countChess(char board[NUMROW][NUMCOL], int row, int col, int option){
	int p1_count = 0;
	int p2_count = 0;
	int empty_count = 0;
	
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (board[i][j] == 'X') p1_count++;
			if (board[i][j] == 'O') p2_count++;
			if (board[i][j] == ' ') empty_count++;
		}
	}
	if (option == 0) return p1_count;
	if (option == 1) return p2_count;
	if (option == 2) return empty_count;
	return -1;
}	

// print the chess board
//     A   B
//    --- ---
// 0 | . |   |
//    --- ---
void printBoard(char board[NUMROW][NUMCOL], int row, int col){

	// print the header (letters)
	printLetterTitles(board, row, col);
	
	for (int i = 0; i < row; i++){
		// print row upper boundary
		printBoundary(board, row, col);
		printChessRow(board, row, col, i);
		switch (i){
			case 2: cout << setw(20) << "Player 1 (X): " << countChess(board, row, col, 0) << endl; break;
			case 3: cout << setw(20) << "Player 2 (O): " << countChess(board, row, col, 1) << endl; break;
			case 4: cout << setw(20) << "Moves left  : " << countChess(board, row, col, 2) << endl; break;
			default: cout << endl;
		}
	}
	printBoundary(board, row, col);
	printLetterTitles(board, row, col);
}

void showMenu(bool p1_turn){
	cout << "This is player " << ( (p1_turn) ? "1 (X)" : "2 (O)" ) << "'s turn. ";
	cout << "Please enter the position (e.g., a0) " << endl;
	cout << "(s to switch turn, or 0 to end the game): ";
}

bool insideBoard(int row, int col, int r, int c){
	if (r < 0 || r >= row) return false;
	if (c < 0 || c >= col) return false;
	return true;
}

bool validMove(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int r, int c){
	
	if (board[r][c] != ' ') return false;
	
	int valid[8] = {0};
	for (int i = 0; i < 8; i++){
		valid[i] = countFlippableChess(board, row, col, my_turn, r, c, row_change[i], col_change[i]);
		if (valid[i]) return true;
	}
	return false;
}

bool checkInput(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int input_row, int input_col){
	if (input_row == -1 && input_col == -1) return true;
	if (input_row == -2 && input_col == -2) return true;
	if (!insideBoard(row, col, input_row, input_col)) return false;
	if (!validMove(board, row, col, my_turn, input_row, input_col)) return false;	
	return true;
}

void processInput(char input[MAXLEN], int & input_row, int & input_col){
	if (input[0] == '0') {
		input_row = input_col = -1;
		return;
	}
	if (input[0] == 's') {
		input_row = input_col = -2;
		return;
	}
	input_col = input[0] - 'a';
	input_row = input[1] - '0';
	return;
}

void getUserMove(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int & input_row, int & input_col){
	char input[MAXLEN] = "";
	showMenu(my_turn);
	cin >> input;
	processInput(input, input_row, input_col);
	while (!checkInput(board, row, col, my_turn, input_row, input_col)){
		cout << "Invalid input, please try again." << endl;
		showMenu(my_turn);
		cin >> input;
		processInput(input, input_row, input_col);
	}
	// testing
/*	bool oppNextToOwn[8] = {0};

	for (int i = 0; i < 8; i++){
		oppNextToOwn[i] = opponentCanFlipBack(board, row, col, my_turn, input_row, input_col, row_change[i], col_change[i]);
		cout << "Direction " << i << ": " << oppNextToOwn[i] << endl;
	}
*/	// testing
	
	return;
}

// test along one direction
// count the number of 'flippable' chess
int countFlippableChess(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int input_row, int input_col, int x_dir, int y_dir){
	int count = 0;
	
	char my_chess = getChess(my_turn);
	char opp_chess = getChess(!my_turn);

	int r = input_row + x_dir;
	int c = input_col + y_dir;
	
	if ( insideBoard(row, col, r, c) && board[r][c] == opp_chess ) {
		// continue to check when opp_chess is beside input_row and input_col
		r = r + x_dir;
		c = c + y_dir;
		count++;
		while ( insideBoard(row, col, r, c) ){
			// find an empty location - return 0
			// find an opponent chess - update count, continue to next iteration
			// find my chess - return count
			if (board[r][c] == ' ') return 0;
			if (board[r][c] == my_chess) return count;
			// move to next location along the direction
			r = r + x_dir;
			c = c + y_dir;
			count++;
		}
		// outside board, cannot find my chess - return 0
		return 0;
	}
	else {	// outside the board, or surrounding is not opp_chess - return 0;
		return 0;
	}
}

int flipOneDirectionChess(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int input_row, int input_col, int x_dir, int y_dir){
	int flipped = 0;
	char my_chess = getChess(my_turn);
	char opp_chess = getChess(!my_turn);
	
	int r = input_row + x_dir;
	int c = input_col + y_dir;
	
	// flip until it is outside board (won't happen), or it reaches my chess
	while ( insideBoard(row, col, r, c) && board[r][c] == opp_chess ) {
		// flip the chess along that direction
		board[r][c] = my_chess;
		flipped++;
		
		r = r + x_dir;
		c = c + y_dir;
	}
	return flipped;
}

void flipAllDirectionChess(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int input_row, int input_col){
	for (int i = 0; i < 8; i++){
		if (countFlippableChess(board, row, col, my_turn, input_row, input_col, row_change[i], col_change[i]) > 0){
			flipOneDirectionChess(board, row, col, my_turn, input_row, input_col, row_change[i], col_change[i]);
		}
	}
}

void updateBoard(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int input_row, int input_col){
	char my_chess = getChess(my_turn);
	board[input_row][input_col] = my_chess;
	flipAllDirectionChess(board, row, col, my_turn, input_row, input_col);
	return;
}

bool checkEnd(char board[NUMROW][NUMCOL], int row, int col){
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (validMove(board, row, col, 0, i, j)) return false;
			if (validMove(board, row, col, 1, i, j)) return false;
		}
	}
	return true;
}

bool hasValidMove(char board[NUMROW][NUMCOL], int row, int col, bool my_turn){
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (validMove(board, row, col, my_turn, i, j)){
//				cout << "Valid move is: " << i << ", " << (char)(j+'a') << endl;
				return true;
			}	
		}		
	}
	return false;
}

void showResult(char board[NUMROW][NUMCOL], int row, int col){
	int p1_count = countChess(board, row, col, 0);
	int p2_count = countChess(board, row, col, 1);

	cout << setfill('*') << setw(17) << " " << endl;
	cout << "  P1: " << p1_count << endl;
	cout << "  P2: " << p2_count << endl;
	if (p1_count == p2_count) cout << "  Draw game! " << endl;
	else cout << ( (p1_count > p2_count) ? "  Player 1 wins!" : "  Player 2 wins!" ) << endl;
	cout << setfill('*') << setw(17) << " " << endl;
	cout << setfill(' ');
	return;
}

bool getGameMode(){
	char input[MAXLEN];
	cout << "Welcome to Othello!" << endl;
	cout << "Please select the game mode, (1) Player vs Player, (2) Player vs Computer: ";
	cin >> input;
	while (input[0] != '1' && input[0] != '2'){
		cout << "Invalid mode selection, please try again." << endl;
		cout << "Please select the game mode, (1) Player vs Player, (2) Player vs Computer: ";
		cin >> input;
	}
	if (input[0] == '1') return false;
	else return true;	
}

// check if, along a direction, opponent will flip back after the chess is put
// i.e., check if, along a direction, there is an opponent chess next to my next own chess &&
// in the opposite direction, one position next is empty
bool opponentCanFlipBack(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int input_row, int input_col, int x_dir, int y_dir){
	char my_chess = getChess(my_turn);
	char opp_chess = getChess(!my_turn);
	
	int flippable = countFlippableChess(board, row, col, my_turn, input_row, input_col, x_dir, y_dir);
	if (flippable > 0){
		int row_to_check_opp = input_row + x_dir * (flippable + 2);
		int col_to_check_opp = input_col + y_dir * (flippable + 2);
		int row_to_check_empty = input_row - x_dir;
		int col_to_check_empty = input_col - y_dir;
		/*		
		cout << "coordinate to check opp = (" << row_to_check_opp << ", " << (char)(col_to_check_opp+'a') << ")" << endl;
		cout << "coordinate to check empty = (" << row_to_check_empty << ", " << (char)(col_to_check_empty+'a') << ")" << endl;
		*/	
		if ( insideBoard(row, col, row_to_check_opp, col_to_check_opp) && board[row_to_check_opp][col_to_check_opp] == opp_chess &&
			 insideBoard(row, col, row_to_check_empty, col_to_check_empty) && board[row_to_check_empty][col_to_check_empty] == ' ')
			return true;
		else
			return false;
	}
	else return false;
}

bool notEatBackMove(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int r, int c){
	if (validMove(board, row, col, my_turn, r, c)){
		for (int dir = 0; dir < 8; dir++){
			if (!opponentCanFlipBack(board, row, col, my_turn, r, c, row_change[dir], col_change[dir])) return true;
		}
	}
	else return false;
}

bool enoughChessMove(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int r, int c, int limit){
	if (validMove(board, row, col, my_turn, r, c)){
		for (int dir = 0; dir < 8; dir++){
			if (countFlippableChess(board, row, col, my_turn, r, c, row_change[dir], col_change[dir]) >= limit) return true;
		}
	}
	return false;
}

void getComputerMove(char board[NUMROW][NUMCOL], int row, int col, bool my_turn, int & input_row, int & input_col){
	// put the chess at four corners if valid
	if (validMove(board, row, col, my_turn, 0, 0)){ input_row = 0; input_col = 0; return; }
	if (validMove(board, row, col, my_turn, row-1, 0)){ input_row = row - 1; input_col = 0; return; }
	if (validMove(board, row, col, my_turn, 0, col-1)){ input_row = 0; input_col = col - 1; return; }
	if (validMove(board, row, col, my_turn, row-1, col-1)){ input_row = row - 1; input_col = col - 1; return; }
	
	// put the chess at boundary positions if valid (only if it won't let opponent to eat back)
	for (int i = 0; i < row; i++){
		if (i == 0 || i == row - 1){	// row 0 and last row, do for all columns except first and last one
			for (int j = 1; j < col-1; j++) {
				if (notEatBackMove(board, row, col, my_turn, i, j)){
					input_row = i;
					input_col = j;
					return;
				}
			}
		}
		else {		// row 1 to row-2, do for first and last column
			for (int j = 0; j < col; j = j + col - 1){
				if (notEatBackMove(board, row, col, my_turn, i, j)){
					input_row = i;
					input_col = j;
					return;
				}
			}
		}
	}
	
	// consider moves not at boundary (accept can flip 5 or more chesses, 3 or more chesses, and 1 chess)
	for (int limit = 5; limit >= 1; limit = limit - 2){
		for (int i = 1; i < row - 1; i++){
			for (int j = 1; j < col - 1; j++){
				if (enoughChessMove(board, row, col, my_turn, i, j, limit)){
					input_row = i;
					input_col = j;
					return;
				}
			}
		}
	}
	
	// remaining moves (at boundary, will let opponent eat back)
	for (int i = 0; i < row; i++){
		for (int j = 0; j < col; j++){
			if (validMove(board, row, col, my_turn, i, j)){
				input_row = i;
				input_col = j;
				return;
			}
		}
	}
}

int main(){
	char board[NUMROW][NUMCOL];
	int row = 0, col = 0, size = 0;
		
	bool game_end = false;
	bool p1_turn = true;
	
	bool vsComputer = false;
	
	vsComputer = getGameMode();
	
	row = NUMROW; col = NUMCOL;
	
	initBoard(board, row, col);
	printBoard(board, row, col);
	
	while (!game_end){
		int input_row = -1, input_col = -1;
		if (vsComputer) {
			if (p1_turn) getUserMove(board, row, col, p1_turn, input_row, input_col);
			else{
				getComputerMove(board, row, col, p1_turn, input_row, input_col);
				showMenu(p1_turn);
				cout << (char)(input_col+'a') << input_row << endl;
			}
		}
		else
			getUserMove(board, row, col, p1_turn, input_row, input_col);

		if (input_row == -1 && input_col == -1){
			game_end = true;
			cout << "Game is ended by user." << endl;
			continue;
		}
		updateBoard(board, row, col, p1_turn, input_row, input_col);
		printBoard(board, row, col);
		
		game_end = checkEnd(board, row, col);
		if (game_end){
			cout << "Game ends, no more possible moves for both players." << endl;
			continue;
		}		
		p1_turn = !p1_turn;
		
		if (!hasValidMove(board, row, col, p1_turn)){
			cout << "No possible moves for player " << ( (p1_turn) ? "1." : "2." ) << endl;
			cout << "Switch to player " << ( (p1_turn) ? "2's turn." : "1's turn." ) << endl;
			p1_turn = !p1_turn;
		}	
	}
	showResult(board, row, col);
	cout << "*** Bye bye! ***" << endl;
	
	return 0;
}
