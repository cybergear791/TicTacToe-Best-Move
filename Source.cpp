// Project 4 AI
// Created by nick Flouty & Joey Robinson
// Solves a 4x4 Tic-Tac-Toe board but with new rules: for a player to win, they must have a move in a unique row and column.

#include <iostream>	//use of cout
#include <istream>	//use of >> from file
#include <fstream>	//to open read a file
#include <vector>	//use of vector
#include <string>	//use of string
#include <string.h>	//for c_str
#include <limits>
#include <algorithm>
#include <time.h>	//for time

using namespace std;

/************************* Shared FUNCTIONS *************************/
vector<vector<int>> Successors(vector<int> board, int playersturn)
{
	vector<vector<int>> successors;
	for (int i = 0; i < board.size(); i++)
	{
		vector<int> temp(board);
		if (temp[i] == 0)
		{
			temp[i] = playersturn;
			successors.push_back(temp);
		}
	}
	return successors;
}


/************************* MinMax() FUNCTIONS *************************/
bool compare_winning(vector<int> board, int &winningPlayer)
{
	vector<vector<int>> winning_combos{
	{0,6,11,13},
	{0,5,10,15},
	{0,5,11,14},
	{0,6,9,15},
	{0,7,9,14},
	{0,7,10,13},
	{1,4,10,15},
	{1,4,11,14},
	{1,6,8,15},
	{1,6,11,12},
	{1,7,8,14},
	{1,7,10,12},
	{2,4,9,15},
	{2,4,11,13},
	{2,5,8,15},
	{2,5,11,12},
	{2,7,8,13},
	{2,7,9,12},
	{3,4,9,14},
	{3,4,10,13},
	{3,5,10,12},
	{3,5,8,14},
	{3,6,8,13},
	{3,6,9,12},
	};
	int tieCounter = 0;
	for (int i = 0; i < board.size(); i++)
	{
		if (board[i] == 0)
			tieCounter++;

	}
	if (tieCounter == 0)
	{
		winningPlayer = -1;
		return true;
	}

	for (int i = 0; i < winning_combos.size(); i++)
	{
		int j = 0;
		if (board[winning_combos[i][j]] == 1 && board[winning_combos[i][j + 1]] == 1 && board[winning_combos[i][j + 2]] == 1 && board[winning_combos[i][j + 3]] == 1)
		{
			winningPlayer = 1;
			return true;
		}
		if (board[winning_combos[i][j]] == 2 && board[winning_combos[i][j + 1]] == 2 && board[winning_combos[i][j + 2]] == 2 && board[winning_combos[i][j + 3]] == 2)
		{
			winningPlayer = 2;
			return true;
		}
	}
	return false;
}

double minMax(vector<int> board, int playersturn, int &expandedNodes, vector<int> &bestSuccessor)
{
	int winningPlayer = 0;
	if (compare_winning(board, winningPlayer))
	{
		return winningPlayer;
	}

	int finalResult;
	int nextPlayer;
	vector<vector<int>> successors = Successors(board, playersturn);
	if (playersturn == 1)
	{
		finalResult = std::numeric_limits<int>::min();
		nextPlayer = 2;
	}
	else
	{
		finalResult = std::numeric_limits<int>::max();
		nextPlayer = 1;
	}


	for (int i = 0; i < successors.size(); i++)
	{
		expandedNodes++;
		if (playersturn == 1)
		{
			int result = minMax(successors[i], nextPlayer, expandedNodes, bestSuccessor);
			finalResult = max(finalResult, result);
			if (finalResult == result)
				bestSuccessor = successors[i];
		}
		else
		{
			int result = minMax(successors[i], nextPlayer, expandedNodes, bestSuccessor);
			finalResult = min(finalResult, result);
			if (finalResult == result)
				bestSuccessor = successors[i];
		}
	}
	return finalResult;
}


/************************* AlphaBetaPruning() FUNCTIONS *************************/
char user(int player)	// converts player 1 from int board to player X for string board
{
	if (player == 1)
		return('X');
	else
		return('0');
}

vector<string> int_to_str_vector(vector<int> board)	//converts vector<int> to a vector<string> 
{
	vector<string> newboard(4);
	int index = 0;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[index] == 2)
				newboard[i].push_back('0');
			else if (board[index] == 1)
				newboard[i].push_back('X');
			else if (board[index] == 0)
				newboard[i].push_back('-');
			index++;
		}
	}
	return newboard;
}

bool contains(vector<int> unique, int col)
{
	for (int s = 0; s < unique.size(); s++)
		if (col == unique[s])
			return true;
	return false;
}
bool contains2(vector<vector<int>> unique, vector<int> col)
{
	for (int s = 0; s < unique.size(); s++)
		if (col == unique[s])
			return true;
	return false;
}

void is_a_win(vector<string> board, char player, vector<int> &unique, int i)
{
	while (i < board.size())
	{
		int j = 0;

		while (j < board[i].size())
		{
			if (!contains(unique, j))
			{
				if (player == board[i][j])
				{
					unique[i] = j;
					is_a_win(board, player, unique, i + 1);
				}
			}
			j++;
		}
		i++;
	}
}
bool if_is_a_win(vector<string>board, char player)
{
	vector<int> unique;
	for (int s = 0; s < board.size(); s++)
		unique.push_back(-1);


	int i = 0;
	while (i < board.size())
	{
		int j = 0;

		while (j < board[i].size())
		{
			if (!contains(unique,j))
			{
				if (player == board[i][j])		
				{
					unique[i] = j;
					is_a_win(board, player, unique, i + 1);
				}
			}
			j++;
		}
		i++;		
	}

	return !contains(unique, -1);		//If the code manages to finish without adding a unique value to each row, then player has not won.
}

void is_a_win_count(vector<string> board, char player, vector<int> &unique, int row, int j, int &ways, vector<vector<int>> &uniques)
{
	while (row < board.size())
	{
		while (j < board[row].size())
		{
			if (!contains(unique, j))
			{
				if (player == board[row][j] || board[row][j] == '-')
				{
					unique[row] = j;
					if (!contains(unique, -1) && !contains2(uniques, unique))
					{
						ways++;
						uniques.push_back(unique);
					}
					else
						is_a_win_count(board, player, unique, row + 1, 0, ways, uniques);  // i played with changing j to 0,j, or j+1.
					unique[row] = -1;
				}
			}
			j++;
		}
		row++;
	}
}
int number_of_ways(vector<string> board, char player)	// # of ways for player to win.
{
	int ways = 0;

	vector<int> unique;
	for (int s = 0; s < board.size(); s++)
		unique.push_back(-1);

	vector<vector<int>> unique_uniques;

	int row = 0;
	while (row < board.size())
	{
		int j = 0;
		while (j < board[row].size())
		{
			if (!contains(unique, j))
			{
				if (player == board[row][j] || board[row][j] == '-')
				{
					unique[row] = j;
					is_a_win_count(board, player, unique, row + 1, 0, ways, unique_uniques);
				}
			}
			j++;
		}
		row++;
	}

	return ways;
}

int static_eval(vector<string> board, int player)
{
	if (if_is_a_win(board, 'X'))
		return 100;
	else if (if_is_a_win(board, '0'))
		return -100;
	else
	{
		if (player == 1)	//for player X point of view
			return number_of_ways(board, 'X') - number_of_ways(board, '0');
		else
			return number_of_ways(board, '0') - number_of_ways(board, 'X');
	}
}

int Alpha_Beta(vector<int> board, int alpha, int beta, int depth, int pos, int &expandedNodes, vector<int> &bestSuccessor)
{
	char player = user(pos);

	if (if_is_a_win(int_to_str_vector(board), player) || depth == 6)		//if (leaf(u))
		return static_eval(int_to_str_vector(board), player);	

	vector<vector<int>> successors = Successors(board, pos);
	int result_value;
	int value;
	int node = 0;

	if (pos == 1)
	{
		node = std::numeric_limits<int>::min();
		pos = 2;
	}
	else
	{
		node = std::numeric_limits<int>::max();
		pos = 1;
	}

	if (node == std::numeric_limits<int>::max())								// if MAX node. 
	{
		result_value = alpha;											// initialize result value
		for (int i = 0; i < successors.size(); i++)
		{
			expandedNodes++;
			value = Alpha_Beta(successors[i], result_value, beta, depth + 1, pos, expandedNodes, bestSuccessor);
			result_value = max(result_value, value);
			if (result_value == value)
				bestSuccessor = successors[i];
			if (result_value >= beta)
				return result_value;
		}
	}
	else
	{
		result_value = beta;
		for (int i = 0; i < successors.size(); i++)
		{
			expandedNodes++;
			value = Alpha_Beta(successors[i], alpha, result_value, depth + 1, pos, expandedNodes, bestSuccessor);
			result_value = min(result_value, value);
			if (result_value == value)
				bestSuccessor = successors[i];
			if (result_value <= alpha)
				return result_value;
		}
	}
	return result_value;
}


/************************* Main() Functions *************************/
int whose_turn(vector<int> board)
{
	int x = 0;
	int o = 0;
	int i = 0;
	while (i < board.size())
	{
		if (board[i] == 1)
			x++;
		if (board[i] == 2)
			o++;
		i++;
	}
	if (o < x)
		return 2;	//0 turn
	if (o == x)
		return 1;	// X turn
	else
		return -1;	// BAD BOARD
}

vector<int> str_to_int_vector(vector<string> board)	//converts vector<str> to a vector<int>
{
	vector<int> newboard;

	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[i].size(); j++)
		{
			if (board[i][j] == '0')
				newboard.push_back(2);
			else if (board[i][j] == 'X')
				newboard.push_back(1);
			else if (board[i][j] == '-')
				newboard.push_back(0);
		}
	}
	return newboard;
}

void Print_BestMove(vector<string>board, vector<string> nextBoard)
{
	char move[3];
	for (int i = 0; i < board.size(); i++)
	{
		for (int j = 0; j < board[i].size(); j++)
		{
			if (board[i][j] != nextBoard[i][j])
			{	
				cout << "Best next move : (" << i << ',' << j << ')' << endl;
				break;
			}
		}
	}
}

int main()
{
	int user;
	cout << "Which problem do you want to solve?" << endl
		<< "1: minmax" << endl
		<< "2: alpha-beta pruning" << endl
		<< "3: quit" << endl << ">>";
	cin >> user;

	while (user != 3 && (user == 1 || user == 2) )
	{
		string input;
		cout << "Enter the name of a 4x4 Tic-Tac-Toe.txt : ";
		cin >> input;

		ifstream file;
		file.open(input);

		// vector has 4 strings. One for each line.
		vector<string> board(4);
		// reads the file into a string vector.
		while (!file.eof())
		{
			for (int i = 0; i < 4; i++)
			{
				file >> board[i];
			}
		}
	
		if (user == 1)
		{
			int expandedNodes = 1;
			vector<int> bestSuccessor;
			int player = whose_turn(str_to_int_vector(board));

			float duration = clock();
			int value = minMax(str_to_int_vector(board), player, expandedNodes, bestSuccessor);
			duration = clock() - duration; 

			string winner;
			if (value == 1)
				winner = "Player 1 Wins";
			else if (value == 2)
				winner = "Player 2 Wins";
			else if (value == -1)
				winner = "Tie";

			cout << "Root Node Value: " << winner << endl;
			Print_BestMove(board, int_to_str_vector(bestSuccessor));
			cout << "Expanded Nodes: " << expandedNodes << endl;
			cout << "CPU time: " << duration / CLOCKS_PER_SEC << " seconds." << endl;
		}

		else if (user == 2)
		{ 
			int expandedNodes = 0;
			vector<int> bestSuccessor2;
			int player2 = whose_turn(str_to_int_vector(board));
			int alpha = std::numeric_limits<int>::min();
			int beta = std::numeric_limits<int>::max();

			float duration2 = clock();
			int value = Alpha_Beta(str_to_int_vector(board), alpha, beta, 0, player2, expandedNodes, bestSuccessor2);
			duration2 = clock() - duration2; 

			string winner;
			if (value == 100)
				winner = "100 (Player 1 Wins)";
			else if (value == -100)
				winner = "-100 (Player 2 Wins)";
			else if (value == std::numeric_limits<int>::max())
				winner = "No Clear Decision, Root Value: 1";
			else if (value == std::numeric_limits<int>::min())
				winner = "No Clear Decision, Root Value: 0";

			cout << "Root Node Value: " << winner << endl;
			Print_BestMove(board, int_to_str_vector(bestSuccessor2));
			cout << "Number of nodes expanded: " << expandedNodes << endl;
			cout << "CPU time: " << duration2 / CLOCKS_PER_SEC << " seconds." << endl;
		}

		file.close();

		cout << endl << "Which problem do you want to solve?" << endl
			<< "1: minmax" << endl
			<< "2: alpha-beta pruning" << endl
			<< "3: quit" << endl << ">>";
		cin >> user;
	}

	system("PAUSE");
	return 0;
}
