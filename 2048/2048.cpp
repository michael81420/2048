// 2048.cpp : Defines the entry point for the console application.
//


#include "stdafx.h"
#include <stdio.h>
#include <conio.h>
#include <fstream>
#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <conio.h>

#define MOVE_UP 0
#define MOVE_DOWN 1
#define MOVE_RIGHT 2
#define MOVE_LEFT 3
#define LEARNING_RATE 0.0025

using namespace std;

int board[4][4];
double inner[17][17][17][17];
double outer[17][17][17][17];
bool game_over = false;
int score = 0;
int pre_score = 0;
int AI_move;
bool available_up = true, available_down = true, available_right = true, available_left = true;

void print_board();
void InitalBoard();
void rand_num();
void InitalValueTable();
bool move_up();
bool move_down();
bool move_right();
bool move_left();
bool judge_game_over();
int judge_AI_move();
double cala_value(int board[][4]);
double cala_value_exp(int board[][4]);
bool board_full();
void updateValueTable(int round);
void outputValueTable();
FILE *fptr = NULL;


int _tmain(int argc, _TCHAR* argv[])
{
	int count = 0;
	int key;
	double total = 0;
	bool update;

	srand(time(NULL));
	InitalValueTable();

	ofstream OutputExcel;
	OutputExcel.precision(6);
	OutputExcel.setf(ios::fixed, ios::floatfield);
	OutputExcel.open("2048.csv"); //糶excel

	while (count < 100000)
	{
		int round = 1;
		InitalBoard();
		//print_board();
		
		while (!game_over)
		{
			AI_move = judge_AI_move();
			
			switch (AI_move)
			{
			case MOVE_UP: // UP
				//cout << "up";
				update = move_up();
				if (!update)
				{
					available_up = false;
				}
				break;
			case MOVE_DOWN: // DOWN
				//cout << "down";
				update = move_down();
				if (!update)
				{
					available_down = false;
				}
				break;
			case MOVE_RIGHT: // Right
				//cout << "right";
				update = move_right();
				if (!update)
				{
					available_right = false;
				}
				break;
			case MOVE_LEFT: // Left
				//cout << "left";
				update = move_left();
				if (!update)
				{
					available_left = false;
				}
				break;
			}
			if (update && !board_full())
			{
				round++;
				switch (AI_move)
				{
				case MOVE_UP:
					fprintf(fptr, "%d\tUP\n", score - pre_score);
					break;
				case MOVE_DOWN:
					fprintf(fptr, "%d\tDOWN\n", score - pre_score);
					break;
				case MOVE_RIGHT:
					fprintf(fptr, "%d\tRIGHT\n", score - pre_score);
					break;
				case MOVE_LEFT:
					fprintf(fptr, "%d\tLEFT\n", score - pre_score);
					break;
				}
				pre_score = score;
				available_up = true;
				available_down = true;
				available_right = true;
				available_left = true;
				rand_num();
				fprintf(fptr, "%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", round,(int)(log(board[0][0] + 1) / log(2)), (int)(log(board[0][1] + 1) / log(2))
					, (int)(log(board[0][2] + 1) / log(2)), (int)(log(board[0][3] + 1) / log(2)), (int)(log(board[1][0] + 1) / log(2)), (int)(log(board[1][1] + 1) / log(2))
					, (int)(log(board[1][2] + 1) / log(2)), (int)(log(board[1][3] + 1) / log(2)), (int)(log(board[2][0] + 1) / log(2)), (int)(log(board[2][1] + 1) / log(2))
					, (int)(log(board[2][2] + 1) / log(2)), (int)(log(board[2][3] + 1) / log(2)), (int)(log(board[3][0] + 1) / log(2)), (int)(log(board[3][1] + 1) / log(2))
					, (int)(log(board[3][2] + 1) / log(2)), (int)(log(board[3][3] + 1) / log(2)));

				//system("cls");
				//print_board();
			}
			game_over = judge_game_over();
			
		}
		fprintf(fptr, "%d\tOVER\n", score - pre_score);
		//cout << "Game Over";
		total += score;
		fclose(fptr);
		updateValueTable(round);
		score = 0;
		pre_score = 0;
		game_over = false;
		count++;
		if (count % 1000 == 0)
		{
			total = total / 1000;
			OutputExcel << total << endl;
			printf("%f\n", total);
			total = 0;
		}
	}
	OutputExcel.close();
	outputValueTable();
	system("pause");
	return 0;
}

void InitalBoard(){	//﹍
	
	fptr = fopen("record.txt", "w");
	fprintf(fptr, "Round\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\tScore\tMove\n");

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			board[i][j] = 0;
		}
	}

	for (int i = 0; i < 2;)
	{
		int new_I = rand() % 4;
		int new_J = rand() % 4;
		if (board[new_I][new_J] == 0){
			board[new_I][new_J] = 2;
			i++;
		}
	}

	fprintf(fptr, "1\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", (int)(log(board[0][0] + 1) / log(2)), (int)(log(board[0][1] + 1) / log(2))
		, (int)(log(board[0][2] + 1) / log(2)), (int)(log(board[0][3] + 1) / log(2)), (int)(log(board[1][0] + 1) / log(2)), (int)(log(board[1][1] + 1) / log(2))
		, (int)(log(board[1][2] + 1) / log(2)), (int)(log(board[1][3] + 1) / log(2)), (int)(log(board[2][0] + 1) / log(2)), (int)(log(board[2][1] + 1) / log(2))
		, (int)(log(board[2][2] + 1) / log(2)), (int)(log(board[2][3] + 1) / log(2)), (int)(log(board[3][0] + 1) / log(2)), (int)(log(board[3][1] + 1) / log(2))
		, (int)(log(board[3][2] + 1) / log(2)), (int)(log(board[3][3] + 1) / log(2)));
}

void InitalValueTable(){

	for (int i = 0; i <= 16; i++)
	{
		for (int j = 0; j <= 16; j++)
		{
			for (int k = 0; k <= 16; k++)
			{
				for (int m = 0; m <= 16; m++)
				{
					inner[i][j][k][m] = 0;
					outer[i][j][k][m] = 0;
				}
			}
		}
	}

}

void print_board(){	//块table

	cout << "score : " << score << endl << "----------------" << endl;
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] == 0)
			{
				cout << "  | ";
			}
			else
			{
				cout << board[i][j] << " | ";
			}
		}
		cout << endl << "----------------" << endl;
	}
}

bool judge_game_over(){
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] != 0)
			{
				if (i - 1 >= 0)
				{
					if (board[i][j] == board[i - 1][j])
					{
						return false;
					}
				}

				if (i + 1 < 4)
				{
					if (board[i][j] == board[i + 1][j])
					{
						return false;
					}
				}

				if (j - 1 >= 0)
				{
					if (board[i][j] == board[i][j - 1])
					{
						return false;
					}
				}

				if (j + 1 < 4)
				{
					if (board[i][j] == board[i][j + 1])
					{
						return false;
					}
				}
			}
			else
			{
				return false;
			}
		}
	}
	return true;
}

void rand_num(){	//繦诀玻ネ2 or 4
	
	int new_I;
	int new_J;
	
	do{
		new_I = rand() % 4;
		new_J = rand() % 4;
	} while (board[new_I][new_J] != 0);

	if (rand() % 4 < 3){
		board[new_I][new_J] = 2;
	}
	else
	{
		board[new_I][new_J] = 4;
	}
}

bool board_full(){
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			if (board[i][j] == 0)
			{
				return false;
			}
		}
	}
	return true;
}

double cala_value(int board[][4]){

	return outer[(int)(log(board[0][3] + 1) / log(2))][(int)(log(board[1][3] + 1) / log(2))][(int)(log(board[2][3] + 1) / log(2))][(int)(log(board[3][3] + 1) / log(2))]
		+ outer[(int)(log(board[3][3] + 1) / log(2))][(int)(log(board[3][2] + 1) / log(2))][(int)(log(board[3][1] + 1) / log(2))][(int)(log(board[3][0] + 1) / log(2))]
		+ outer[(int)(log(board[3][0] + 1) / log(2))][(int)(log(board[2][0] + 1) / log(2))][(int)(log(board[1][0] + 1) / log(2))][(int)(log(board[0][0] + 1) / log(2))]
		+ outer[(int)(log(board[0][0] + 1) / log(2))][(int)(log(board[0][1] + 1) / log(2))][(int)(log(board[0][2] + 1) / log(2))][(int)(log(board[0][3] + 1) / log(2))]
		+ outer[(int)(log(board[3][3] + 1) / log(2))][(int)(log(board[2][3] + 1) / log(2))][(int)(log(board[1][3] + 1) / log(2))][(int)(log(board[0][3] + 1) / log(2))]
		+ outer[(int)(log(board[3][0] + 1) / log(2))][(int)(log(board[3][1] + 1) / log(2))][(int)(log(board[3][2] + 1) / log(2))][(int)(log(board[3][3] + 1) / log(2))]
		+ outer[(int)(log(board[0][0] + 1) / log(2))][(int)(log(board[1][0] + 1) / log(2))][(int)(log(board[2][0] + 1) / log(2))][(int)(log(board[3][0] + 1) / log(2))]
		+ outer[(int)(log(board[0][3] + 1) / log(2))][(int)(log(board[0][2] + 1) / log(2))][(int)(log(board[0][1] + 1) / log(2))][(int)(log(board[0][0] + 1) / log(2))]
		+ inner[(int)(log(board[0][2] + 1) / log(2))][(int)(log(board[1][2] + 1) / log(2))][(int)(log(board[2][2] + 1) / log(2))][(int)(log(board[3][2] + 1) / log(2))]
		+ inner[(int)(log(board[2][3] + 1) / log(2))][(int)(log(board[2][2] + 1) / log(2))][(int)(log(board[2][1] + 1) / log(2))][(int)(log(board[2][0] + 1) / log(2))]
		+ inner[(int)(log(board[3][1] + 1) / log(2))][(int)(log(board[2][1] + 1) / log(2))][(int)(log(board[1][1] + 1) / log(2))][(int)(log(board[0][1] + 1) / log(2))]
		+ inner[(int)(log(board[1][0] + 1) / log(2))][(int)(log(board[1][1] + 1) / log(2))][(int)(log(board[1][2] + 1) / log(2))][(int)(log(board[1][3] + 1) / log(2))]
		+ inner[(int)(log(board[3][2] + 1) / log(2))][(int)(log(board[2][2] + 1) / log(2))][(int)(log(board[1][2] + 1) / log(2))][(int)(log(board[0][2] + 1) / log(2))]
		+ inner[(int)(log(board[2][0] + 1) / log(2))][(int)(log(board[2][1] + 1) / log(2))][(int)(log(board[2][2] + 1) / log(2))][(int)(log(board[2][3] + 1) / log(2))]
		+ inner[(int)(log(board[0][1] + 1) / log(2))][(int)(log(board[1][1] + 1) / log(2))][(int)(log(board[2][1] + 1) / log(2))][(int)(log(board[3][1] + 1) / log(2))]
		+ inner[(int)(log(board[1][3] + 1) / log(2))][(int)(log(board[1][2] + 1) / log(2))][(int)(log(board[1][1] + 1) / log(2))][(int)(log(board[1][0] + 1) / log(2))];
}

double cala_value_exp(int board[][4]){

	return outer[board[0][3]][board[1][3]][board[2][3]][board[3][3]]
		+ outer[board[3][3]][board[3][2]][board[3][1]][board[3][0]]
		+ outer[board[3][0]][board[2][0]][board[1][0]][board[0][0]]
		+ outer[board[0][0]][board[0][1]][board[0][2]][board[0][3]]
		+ outer[board[3][3]][board[2][3]][board[1][3]][board[0][3]]
		+ outer[board[3][0]][board[3][1]][board[3][2]][board[3][3]]
		+ outer[board[0][0]][board[1][0]][board[2][0]][board[3][0]]
		+ outer[board[0][3]][board[0][2]][board[0][1]][board[0][0]]
		+ inner[board[0][2]][board[1][2]][board[2][2]][board[3][2]]
		+ inner[board[2][3]][board[2][2]][board[2][1]][board[2][0]]
		+ inner[board[3][1]][board[2][1]][board[1][1]][board[0][1]]
		+ inner[board[1][0]][board[1][1]][board[1][2]][board[1][3]]
		+ inner[board[3][2]][board[2][2]][board[1][2]][board[0][2]]
		+ inner[board[2][0]][board[2][1]][board[2][2]][board[2][3]]
		+ inner[board[0][1]][board[1][1]][board[2][1]][board[3][1]]
		+ inner[board[1][3]][board[1][2]][board[1][1]][board[1][0]];
}


int judge_AI_move(){

	int newBoard[4][4];
	double moveUp = 0.0, moveDown = 0.0, moveRight = 0.0, moveLeft = 0.0;
	
	//moveUP

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			newBoard[i][j] = board[i][j];
		}
	}

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int temp1 = i;
			int temp2;
			while (newBoard[temp1][j] == 0 && temp1 < 4){
				temp1++;
			}

			if (temp1 == 4){	//禬筁board
				newBoard[i][j] = 0;
			}
			else{
				temp2 = temp1 + 1;
				while (newBoard[temp2][j] == 0 && temp2 < 4){
					temp2++;
				}
				if (temp2 == 4 && temp1 != i){
					newBoard[i][j] = newBoard[temp1][j];
					newBoard[temp1][j] = 0;
				}
				else if (temp2 != 4 && temp1 != i){
					if (newBoard[temp1][j] == newBoard[temp2][j]){
						newBoard[i][j] = newBoard[temp1][j] * 2;
						newBoard[temp1][j] = 0;
						newBoard[temp2][j] = 0;
					}
					else{
						newBoard[i][j] = newBoard[temp1][j];
						newBoard[temp1][j] = 0;
					}
				}
				else if (temp2 != 4 && temp1 == i){
					if (newBoard[temp1][j] == newBoard[temp2][j]){
						newBoard[i][j] = newBoard[temp1][j] * 2;
						newBoard[temp2][j] = 0;
					}
				}
			}
		}
	}

	moveUp = cala_value(newBoard);

	//moveDown

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			newBoard[i][j] = board[i][j];
		}
	}

	for (int i = 3; i >= 0; i--)
	{
		for (int j = 0; j < 4; j++)
		{
			int temp1 = i;
			int temp2;

			while (newBoard[temp1][j] == 0 && temp1 >= 0){
				temp1--;
			}

			if (temp1 == -1){	//禬筁newBoard
				newBoard[i][j] = 0;
			}
			else{
				temp2 = temp1 - 1;
				while (newBoard[temp2][j] == 0 && temp2 >= 0){
					temp2--;
				}
				if (temp2 == -1 && temp1 != i){
					newBoard[i][j] = newBoard[temp1][j];
					newBoard[temp1][j] = 0;
				}
				else if (temp2 != -1 && temp1 != i){
					if (newBoard[temp1][j] == newBoard[temp2][j]){
						newBoard[i][j] = newBoard[temp1][j] * 2;
						newBoard[temp1][j] = 0;
						newBoard[temp2][j] = 0;
					}
					else{
						newBoard[i][j] = newBoard[temp1][j];
						newBoard[temp1][j] = 0;
					}
				}
				else if (temp2 != -1 && temp1 == i){
					if (newBoard[temp1][j] == newBoard[temp2][j]){
						newBoard[i][j] = newBoard[temp1][j] * 2;
						newBoard[temp2][j] = 0;
					}
				}
			}
		}
	}

	moveDown = cala_value(newBoard);
	//moveRight

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			newBoard[i][j] = board[i][j];
		}
	}

	for (int j = 3; j >= 0; j--)
	{
		for (int i = 0; i < 4; i++)
		{
			int temp1 = j;
			int temp2;

			while (newBoard[i][temp1] == 0 && temp1 >= 0){
				temp1--;
			}

			if (temp1 == -1){	//禬筁newBoard
				newBoard[i][j] = 0;
			}
			else{
				temp2 = temp1 - 1;
				while (newBoard[i][temp2] == 0 && temp2 >= 0){
					temp2--;
				}
				if (temp2 == -1 && temp1 != j){
					newBoard[i][j] = newBoard[i][temp1];
					newBoard[i][temp1] = 0;
				}
				else if (temp2 != -1 && temp1 != j){
					if (newBoard[i][temp1] == newBoard[i][temp2]){
						newBoard[i][j] = newBoard[i][temp1] * 2;
						newBoard[i][temp1] = 0;
						newBoard[i][temp2] = 0;
					}
					else{
						newBoard[i][j] = newBoard[i][temp1];
						newBoard[i][temp1] = 0;
					}
				}
				else if (temp2 != -1 && temp1 == j){
					if (newBoard[i][temp1] == newBoard[i][temp2]){
						newBoard[i][j] = newBoard[i][temp1] * 2;
						newBoard[i][temp2] = 0;
					}
				}
			}
		}
	}

	moveRight = cala_value(newBoard);

	//moveLeft

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			newBoard[i][j] = board[i][j];
		}
	}

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			int temp1 = j;
			int temp2;

			while (newBoard[i][temp1] == 0 && temp1 < 4){
				temp1++;
			}

			if (temp1 == 4){	//禬筁newBoard
				newBoard[i][j] = 0;
			}
			else{
				temp2 = temp1 + 1;
				while (newBoard[i][temp2] == 0 && temp2 < 4){
					temp2++;
				}
				if (temp2 == 4 && temp1 != j){
					newBoard[i][j] = newBoard[i][temp1];
					newBoard[i][temp1] = 0;
				}
				else if (temp2 != 4 && temp1 != j){
					if (newBoard[i][temp1] == newBoard[i][temp2]){
						newBoard[i][j] = newBoard[i][temp1] * 2;
						newBoard[i][temp1] = 0;
						newBoard[i][temp2] = 0;
					}
					else{
						newBoard[i][j] = newBoard[i][temp1];
						newBoard[i][temp1] = 0;
					}
				}
				else if (temp2 != 4 && temp1 == j){
					if (newBoard[i][temp1] == newBoard[i][temp2]){
						newBoard[i][j] = newBoard[i][temp1] * 2;
						newBoard[i][temp2] = 0;
					}
				}
			}
		}
	}

	moveLeft = cala_value(newBoard);

	if (available_up)
	{
		if (available_down)
		{
			if (available_right)
			{
				if (available_left)
				{
					if (moveUp >= moveDown && moveUp >= moveRight && moveUp >= moveLeft)	//簿程
					{
						return MOVE_UP;
					}
					else
					{
						if (moveDown >= moveRight && moveDown >= moveLeft)	//簿程
						{
							return MOVE_DOWN;
						}
						else
						{
							if (moveRight >= moveLeft)	//簿程
							{
								return MOVE_RIGHT;
							}
							else
							{
								return MOVE_LEFT;
							}
						}
					}
				}
				else
				{
					if (moveUp >= moveDown && moveUp >= moveRight)	//簿程
					{
						return MOVE_UP;
					}
					else
					{
						if (moveDown >= moveRight)	//簿程
						{
							return MOVE_DOWN;
						}
						else
						{
							return MOVE_RIGHT;
						}
					}
				}
			}
			else
			{
				if (available_left)
				{
					if (moveUp >= moveDown && moveUp >= moveLeft)	//簿程
					{
						return MOVE_UP;
					}
					else
					{
						if (moveDown >= moveLeft)	//簿程
						{
							return MOVE_DOWN;
						}
						else
						{
							return MOVE_LEFT;
						}
					}
				}
				else
				{
					if (moveUp >= moveDown)	//簿程
					{
						return MOVE_UP;
					}
					else
					{
						return MOVE_DOWN;
					}
				}
			}
		}
		else
		{
			if (available_right)
			{
				if (available_left)
				{
					if (moveUp >= moveRight && moveUp >= moveLeft)	//簿程
					{
						return MOVE_UP;
					}
					else
					{
						if (moveRight >= moveLeft)	//簿程
						{
							return MOVE_RIGHT;
						}
						else
						{
							return MOVE_LEFT;
						}
					}
				}
				else
				{
					if (moveUp >= moveRight)	//簿程
					{
						return MOVE_UP;
					}
					else
					{
						return MOVE_RIGHT;
					}
				}
			}
			else
			{
				if (available_left)
				{
					if (moveUp >= moveLeft)	//簿程
					{
						return MOVE_UP;
					}
					else
					{
						return MOVE_LEFT;
					}
				}
				else
				{
					return MOVE_UP;
				}
			}
		}
	}
	else
	{
		if (available_down)
		{
			if (available_right)
			{
				if (available_left)
				{
					if (moveDown >= moveRight && moveDown >= moveLeft)	//簿程
					{
						return MOVE_DOWN;
					}
					else
					{
						if (moveRight >= moveLeft)	//簿程
						{
							return MOVE_RIGHT;
						}
						else
						{
							return MOVE_LEFT;
						}
					}
				}
				else
				{
					if (moveDown >= moveRight)	//簿程
					{
						return MOVE_DOWN;
					}
					else
					{
						return MOVE_RIGHT;
					}
				}
			}
			else
			{
				if (available_left)
				{
					if (moveDown >= moveLeft)	//簿程
					{
						return MOVE_DOWN;
					}
					else
					{
						return MOVE_LEFT;
					}
				}
				else
				{
					return MOVE_DOWN;
				}
			}
		}
		else
		{
			if (available_right)
			{
				if (available_left)
				{
					if (moveRight >= moveLeft)	//簿程
					{
						return MOVE_RIGHT;
					}
					else
					{
						return MOVE_LEFT;
					}
				}
				else
				{
					return MOVE_RIGHT;
				}
			}
			else
			{
				return MOVE_LEFT;
			}
		}
	}



	
}

bool move_up(){
	bool update = false;

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			int temp1 = i;
			int temp2;
			while (board[temp1][j] == 0 && temp1 < 4){
				temp1++;
			}

			if (temp1 == 4){	//禬筁board
				board[i][j] = 0;
			}
			else{
				temp2 = temp1 + 1;
				while (board[temp2][j] == 0 && temp2 < 4){
					temp2++;
				}
				if (temp2 == 4 && temp1 != i){
					board[i][j] = board[temp1][j];
					board[temp1][j] = 0;
					update = true;
				}
				else if (temp2 != 4 && temp1 != i){
					if (board[temp1][j] == board[temp2][j]){
						board[i][j] = board[temp1][j] * 2;
						board[temp1][j] = 0;
						board[temp2][j] = 0;
						score += board[i][j];
					}
					else{
						board[i][j] = board[temp1][j];
						board[temp1][j] = 0;
					}
					update = true;
				}
				else if (temp2 != 4 && temp1 == i){
					if (board[temp1][j] == board[temp2][j]){
						board[i][j] = board[temp1][j] * 2;
						board[temp2][j] = 0;
						score += board[i][j];
						update = true;
					}
				}
			}
		}
	}

	return update;
}

bool move_down(){
	bool update = false;

	for (int i = 3; i >= 0; i--)
	{
		for (int j = 0; j < 4; j++)
		{
			int temp1 = i;
			int temp2;

			while (board[temp1][j] == 0 && temp1 >= 0){
				temp1--;
			}

			if (temp1 == -1){	//禬筁board
				board[i][j] = 0;
			}
			else{
				temp2 = temp1 - 1;
				while (board[temp2][j] == 0 && temp2 >= 0){
					temp2--;
				}
				if (temp2 == -1 && temp1 != i){
					board[i][j] = board[temp1][j];
					board[temp1][j] = 0;
					update = true;
				}
				else if (temp2 != -1 && temp1 != i){
					if (board[temp1][j] == board[temp2][j]){
						board[i][j] = board[temp1][j] * 2;
						board[temp1][j] = 0;
						board[temp2][j] = 0;
						score += board[i][j];
					}
					else{
						board[i][j] = board[temp1][j];
						board[temp1][j] = 0;
					}
					update = true;
				}
				else if (temp2 != -1 && temp1 == i){
					if (board[temp1][j] == board[temp2][j]){
						board[i][j] = board[temp1][j] * 2;
						board[temp2][j] = 0;
						score += board[i][j];
						update = true;
					}
				}
			}
		}
	}

	return update;
}

bool move_right(){
	bool update = false;

	for (int j = 3; j >= 0; j--)
	{
		for (int i = 0; i < 4; i++)
		{
			int temp1 = j;
			int temp2;

			while (board[i][temp1] == 0 && temp1 >= 0){
				temp1--;
			}

			if (temp1 == -1){	//禬筁board
				board[i][j] = 0;
			}
			else{
				temp2 = temp1 - 1;
				while (board[i][temp2] == 0 && temp2 >= 0){
					temp2--;
				}
				if (temp2 == -1 && temp1 != j){
					board[i][j] = board[i][temp1];
					board[i][temp1] = 0;
					update = true;
				}
				else if (temp2 != -1 && temp1 != j){
					if (board[i][temp1] == board[i][temp2]){
						board[i][j] = board[i][temp1] * 2;
						board[i][temp1] = 0;
						board[i][temp2] = 0;
						score += board[i][j];
					}
					else{
						board[i][j] = board[i][temp1];
						board[i][temp1] = 0;
					}
					update = true;
				}
				else if (temp2 != -1 && temp1 == j){
					if (board[i][temp1] == board[i][temp2]){
						board[i][j] = board[i][temp1] * 2;
						board[i][temp2] = 0;
						score += board[i][j];
						update = true;
					}
				}
			}
		}
	}

	return update;
}

bool move_left(){
	bool update = false;

	for (int j = 0; j < 4; j++)
	{
		for (int i = 0; i < 4; i++)
		{
			int temp1 = j;
			int temp2;

			while (board[i][temp1] == 0 && temp1 < 4){
				temp1++;
			}

			if (temp1 == 4){	//禬筁board
				board[i][j] = 0;
			}
			else{
				temp2 = temp1 + 1;
				while (board[i][temp2] == 0 && temp2 < 4){
					temp2++;
				}
				if (temp2 == 4 && temp1 != j){
					board[i][j] = board[i][temp1];
					board[i][temp1] = 0;
					update = true;
				}
				else if (temp2 != 4 && temp1 != j){
					if (board[i][temp1] == board[i][temp2]){
						board[i][j] = board[i][temp1] * 2;
						board[i][temp1] = 0;
						board[i][temp2] = 0;
						score += board[i][j];
					}
					else{
						board[i][j] = board[i][temp1];
						board[i][temp1] = 0;
					}
					update = true;
				}
				else if (temp2 != 4 && temp1 == j){
					if (board[i][temp1] == board[i][temp2]){
						board[i][j] = board[i][temp1] * 2;
						board[i][temp2] = 0;
						score += board[i][j];
						update = true;
					}
				}
			}
		}
	}

	return update;
}

void updateValueTable(int round){

	FILE *fptr;
	char s[1000];
	int d;
	int board1[4][4], board2[4][4]; //パ パオ
	int get_score;
	double error;

	fptr = fopen("record.txt", "r");

	

	for (int i = 0; i < round; i++)	//程Ы
	{
		fgets(s, 1000, fptr);
	}
	fscanf(fptr, "%d");
	fscanf(fptr, "\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", &board1[0][0], &board1[0][1], &board1[0][2], &board1[0][3], &board1[1][0],
		&board1[1][1], &board1[1][2], &board1[1][3], &board1[2][0], &board1[2][1], &board1[2][2], &board1[2][3], &board1[3][0], &board1[3][1], &board1[3][2]
		, &board1[3][3], &get_score);
	error = -cala_value_exp(board1);
	outer[board1[0][3]][board1[1][3]][board1[2][3]][board1[3][3]] += (error / 16) * LEARNING_RATE;
	outer[board1[3][3]][board1[3][2]][board1[3][1]][board1[3][0]] += (error / 16) * LEARNING_RATE;
	outer[board1[3][0]][board1[2][0]][board1[1][0]][board1[0][0]] += (error / 16) * LEARNING_RATE;
	outer[board1[0][0]][board1[0][1]][board1[0][2]][board1[0][3]] += (error / 16) * LEARNING_RATE;
	outer[board1[3][3]][board1[2][3]][board1[1][3]][board1[0][3]] += (error / 16) * LEARNING_RATE;
	outer[board1[3][0]][board1[3][1]][board1[3][2]][board1[3][3]] += (error / 16) * LEARNING_RATE;
	outer[board1[0][0]][board1[1][0]][board1[2][0]][board1[3][0]] += (error / 16) * LEARNING_RATE;
	outer[board1[0][3]][board1[0][2]][board1[0][1]][board1[0][0]] += (error / 16) * LEARNING_RATE;
	inner[board1[0][2]][board1[1][2]][board1[2][2]][board1[3][2]] += (error / 16) * LEARNING_RATE;
	inner[board1[2][3]][board1[2][2]][board1[2][1]][board1[2][0]] += (error / 16) * LEARNING_RATE;
	inner[board1[3][1]][board1[2][1]][board1[1][1]][board1[0][1]] += (error / 16) * LEARNING_RATE;
	inner[board1[1][0]][board1[1][1]][board1[1][2]][board1[1][3]] += (error / 16) * LEARNING_RATE;
	inner[board1[3][2]][board1[2][2]][board1[1][2]][board1[0][2]] += (error / 16) * LEARNING_RATE;
	inner[board1[2][0]][board1[2][1]][board1[2][2]][board1[2][3]] += (error / 16) * LEARNING_RATE;
	inner[board1[0][1]][board1[1][1]][board1[2][1]][board1[3][1]] += (error / 16) * LEARNING_RATE;
	inner[board1[1][3]][board1[1][2]][board1[1][1]][board1[1][0]] += (error / 16) * LEARNING_RATE;
	rewind(fptr);

	for (int i = round; i >= 1 ; i--)	//程玡Ы
	{
		int count = 0;
		while (count != i - 1)
		{
			fgets(s, 1000, fptr);
			count++;
		}
		fscanf(fptr, "%d");
		fscanf(fptr, "\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", &board1[0][0], &board1[0][1], &board1[0][2], &board1[0][3], &board1[1][0],
			&board1[1][1], &board1[1][2], &board1[1][3], &board1[2][0], &board1[2][1], &board1[2][2], &board1[2][3], &board1[3][0], &board1[3][1], &board1[3][2]
			, &board1[3][3], &get_score);
		fscanf(fptr, "%s\n");
		fscanf(fptr, "%d");
		fscanf(fptr, "\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t", &board2[0][0], &board2[0][1], &board2[0][2], &board2[0][3], &board2[1][0],
			&board2[1][1], &board2[1][2], &board2[1][3], &board2[2][0], &board2[2][1], &board2[2][2], &board2[2][3], &board2[3][0], &board2[3][1], &board2[3][2]
			, &board2[3][3]);
		error = cala_value_exp(board2) + get_score - cala_value_exp(board1);
		outer[board1[0][3]][board1[1][3]][board1[2][3]][board1[3][3]] += (error / 16) * LEARNING_RATE;
		outer[board1[3][3]][board1[3][2]][board1[3][1]][board1[3][0]] += (error / 16) * LEARNING_RATE;
		outer[board1[3][0]][board1[2][0]][board1[1][0]][board1[0][0]] += (error / 16) * LEARNING_RATE;
		outer[board1[0][0]][board1[0][1]][board1[0][2]][board1[0][3]] += (error / 16) * LEARNING_RATE;
		outer[board1[3][3]][board1[2][3]][board1[1][3]][board1[0][3]] += (error / 16) * LEARNING_RATE;
		outer[board1[3][0]][board1[3][1]][board1[3][2]][board1[3][3]] += (error / 16) * LEARNING_RATE;
		outer[board1[0][0]][board1[1][0]][board1[2][0]][board1[3][0]] += (error / 16) * LEARNING_RATE;
		outer[board1[0][3]][board1[0][2]][board1[0][1]][board1[0][0]] += (error / 16) * LEARNING_RATE;
		inner[board1[0][2]][board1[1][2]][board1[2][2]][board1[3][2]] += (error / 16) * LEARNING_RATE;
		inner[board1[2][3]][board1[2][2]][board1[2][1]][board1[2][0]] += (error / 16) * LEARNING_RATE;
		inner[board1[3][1]][board1[2][1]][board1[1][1]][board1[0][1]] += (error / 16) * LEARNING_RATE;
		inner[board1[1][0]][board1[1][1]][board1[1][2]][board1[1][3]] += (error / 16) * LEARNING_RATE;
		inner[board1[3][2]][board1[2][2]][board1[1][2]][board1[0][2]] += (error / 16) * LEARNING_RATE;
		inner[board1[2][0]][board1[2][1]][board1[2][2]][board1[2][3]] += (error / 16) * LEARNING_RATE;
		inner[board1[0][1]][board1[1][1]][board1[2][1]][board1[3][1]] += (error / 16) * LEARNING_RATE;
		inner[board1[1][3]][board1[1][2]][board1[1][1]][board1[1][0]] += (error / 16) * LEARNING_RATE;
		rewind(fptr);
	}
	fclose(fptr);
}

void outputValueTable(){
	FILE *fptr1, *fptr2;
	
	fptr1 = fopen("value_table_inner.txt", "w");
	fptr2 = fopen("value_table_outter.txt", "w");

	for (int i = 0; i < 17; i++)
	{
		for (int j = 0; j < 17; j++)
		{
			for (int k = 0; k < 17; k++)
			{
				for (int m = 0; m < 17; m++)
				{
					fprintf(fptr1, "%d %d %d %d %f\n", i, j, k, m, inner[i][j][k][m]);
					fprintf(fptr2, "%d %d %d %d %f\n", i, j, k, m, outer[i][j][k][m]);
				}
			}
		}
	}

	fclose(fptr1);
	fclose(fptr2);
}