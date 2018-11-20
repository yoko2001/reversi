#include<iostream>
#include<cstdlib>
#include<ctime>
#include <cassert>


using namespace std;
struct direction {
	int x;
	int y;
};
struct diredist {
	int i;
	int distance;
}; 
const int Black = 1, White = 2;
static int laststeps[100] = { 0 };
static int stepnum = 0;

direction dire[8] = {
	{-1 ,1}, {0, 1}, {1, 1}, {1, 0},           //WD， D， SD ， S  Arrow Keys
	{1, -1}, {0, -1}, {-1, -1}, {-1, 0}			// SA， A， WA， W
};

int mapDefault(int map[][8]) {
	map[3][3] = 1;
	map[4][4] = 1;
	map[3][4] = 2;
	map[4][3] = 2;
	return 0;
}

int printCurrent(int chessboard[][8]) {
	int i, j;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) cout << chessboard[i][j] << ' ';
		cout << endl;
	}
	cout << endl;
	return 0;
}

int CopyChessboard(int formerchessboard[][8] , int targetchessoard[][8]) {
	int i, j;
	for (i = 0; i < 8; i++) 
		for(j=0; j<8; j++)
			targetchessoard[i][j] = formerchessboard[i][j];
	return 0;
}

bool isolated(int x, int y, int map[][8]) {
	int i;
	int x0, y0;

	for (i = 0; i < 8; i++) {
		if ((-1 < (x + dire[i].x)) && ((x + dire[i].x) < 8) && (-1 < (y + dire[i].y)) && ((y + dire[i].y) < 8)) {
			x0 = x + dire[i].x;  y0 = y + dire[i].y;
			if (map[x0][y0] != 0) return false;
		}
	}
	return true;
}

int thisDireLegal(int x, int y, int i, int Color, int map[][8]) {
	bool flag = false;
	int xx = x, yy = y, howManyIn = 0;

	if (map[x+dire[i].x][y+dire[i].y] == 0) return 0;
	if (map[x + dire[i].x][y + dire[i].y] == Color) return 0;
	int OppoColor;
	if (Color == 1) OppoColor = 2; else OppoColor = 1;

	

	while ((-1 < (xx + dire[i].x)) && ((xx + dire[i].x) < 8) && (-1 < (yy + dire[i].y)) && ((yy + dire[i].y) < 8)) {
		xx += dire[i].x;
		yy += dire[i].y;
		if (map[xx][yy] == OppoColor) {
			howManyIn++;
			
		}
		if (map[xx][yy] == 0) {
			return 0;
		}
		if (map[xx][yy] == Color) {
			flag = true;
			break;
		}
	}
	if (flag) return howManyIn;
	else return 0;
}

bool ifLegal(int x0, int y0, int chessboard[][8], int Color) {
	if (chessboard[x0][y0] != 0) return false;

	int copiedChessboard[8][8];

	bool flag = false;
	int i, ifLegalhowmany;

	CopyChessboard(chessboard, copiedChessboard);

	if (isolated(x0, y0, copiedChessboard)) return false;

	for (i = 0; i < 8; i++) {
		ifLegalhowmany = thisDireLegal(x0, y0, i, Color, copiedChessboard);
		if (ifLegalhowmany > 0) {
			return true;
			//cout << x0 << ' ' << y0 << ' ' <<i<<' '<< thisDireLegal(x0, y0, i, Color, pointercC);
		}
	}
	return false;
}

int Reverse(int x0, int y0, int chessboard[][8], int Color) {
	int OppoColor, i, step, j, xx = x0, yy = y0;
	if (Color == 1) OppoColor = 2; else OppoColor = 1;

	for (i = 0; i < 8; i++) {
		step = thisDireLegal(x0, y0, i, Color,chessboard);
		if (step > 0) {
			xx = x0; yy = y0;
			//assert(xx <= 7 && yy <= 7 && xx >= 0 && yy >= 0);
			chessboard[xx][yy] = Color;
			for (j = 0; j < step; j++) {
				xx += dire[i].x;
				yy += dire[i].y;
				//assert(xx <= 7 && yy <= 7 && xx >= 0 && yy >= 0);
				chessboard[xx][yy] = Color;
			}

		}
	}
	return 0;
}

bool ifThereExistsLegal(int chessboard[][8], int color) {
	bool f;
	int j, k;
	f = false;
	for (j = 0; j < 8; j++) {
		for (k = 0; k < 8; k++) {
			if (ifLegal(j, k, chessboard, color)) {
				f = true; break;
			}
		}
		if (f) break;
	}

	return f;
}

int Referee(int chessboard[][8]) {
	int counter = 0, i, j;
	for (i = 0; i < 8; i++) {
		for (j = 0; j < 8; j++) {
			if (chessboard[i][j] == 1) counter++;
			if (chessboard[i][j] == 2) counter--;
			//cout << *(pointer + i * 8 + j) << ' ';
		}		
	}	
	return counter;
}

int Max_Min_Test(int chessboard[][8], int Alpha, int Beta, int color) {
	int x, i, j, nextcolor, Alpha2 = Alpha, Beta2 = Beta;  //子节点AlphaBeta继承；
	int chessboard2[8][8];

	if (Alpha2 > Beta2) {
		if (color == 1) return Beta2;
		if (color == 2) return Alpha2;
	}
	
	if (color == 1) nextcolor = 2;
	else nextcolor = 1;
    
	if ((ifThereExistsLegal(chessboard, color) == false)) {
		if (ifThereExistsLegal(chessboard, nextcolor) == false) {    //叶节点返回自身结果
			
			return Referee(chessboard);
		}
		else {
			color = nextcolor;
			if (color == 1) nextcolor = 2; else nextcolor = 1;
		}
	}

	CopyChessboard(chessboard, chessboard2);

	
	for (i = 0; i < 8; i++) for (j = 0; j < 8; j++) {
		if (ifLegal(i, j, chessboard2,color) == true) {
			Reverse(i, j, chessboard2, color); 
			//cout << i << ' ' << j << ' ' << color<< ' '<< Alpha2<<' ' <<Beta2<<endl ; 
			//printCurrent(chessboard2);
			if (color == 1) //如果颜色为1，为求极大值点，更改其Alpha
			{
				//stepnum++;
				x = Max_Min_Test(chessboard2, Alpha2, Beta2, nextcolor);
				//stepnum--;
				if (Alpha2 < x) {
					Alpha2 = x;

					//laststeps[stepnum+1] = i * 8 + j;
				}
			}

			if (color == 2)
			{
				//stepnum++;
				x = Max_Min_Test(chessboard2, Alpha2, Beta2, nextcolor);
				//stepnum--;
				if (Beta2 > x) {
					Beta2 = x;

					//	laststeps[stepnum+1] = i * 8 + j;
				}
			}
			
			CopyChessboard(chessboard, chessboard2);
		}
	}


	if (color == 1) {  return Alpha2; }
	if (color == 2) {   return Beta2; }
}

int Referee_Feasi(int chessboard[][8], int color){
	int counter = 0;
	for (int i = 0; i < 8; i++) 
		for (int j = 0; j < 8; j++) {
			if (ifLegal(i, j, chessboard, color) == true) counter++;
		}
	return counter;
}


int main() {
	int x, y, currentColor, i, j, k, stoper = 0;
	bool f;
	int chessboard[8][8] = { 0 };

	int*pointerChessboard = &chessboard[0][0];

	srand((unsigned)time(NULL));

	mapDefault(chessboard);
	

	f = true;
	currentColor = 1;
	srand((unsigned)time(NULL));
	x = rand() % 8;
	y = rand() % 8;
	//盲走，留下最后颜色、棋盘
	for (i = 0; i < 52; i++)
	{
		while ((f) && (ifLegal(x, y, chessboard, currentColor) == false))
		{
			x = rand() % 8;
			y = rand() % 8;
		}

		//cout << x << ' ' << y << endl;

		if (ifLegal(x, y, chessboard, currentColor)) {
			Reverse(x, y, chessboard, currentColor);
			stoper = 0;
		}
		else {
			stoper++;
		}
		if (currentColor == 1) currentColor = 2;
		else currentColor = 1;

		if (stoper > 1) {
			Referee(chessboard);
			break;
		}

		f = ifThereExistsLegal(chessboard, currentColor);
		/*f = false;
		for (j = 0; j < 8; j++) {
			for (k = 0; k < 8; k++) {
				if (ifLegal(j, k, pointerChessboard, currentColor)) {
					f = true; break;
				}
			}
			if (f) break;
		}*/

		x = rand() % 8;
		y = rand() % 8;
	}


	//cout << Referee(pointerChessboard)<< endl;
	printCurrent(chessboard);
	cout << Referee_Feasi(chessboard, currentColor);
	//cout <<endl<< Referee(chessboard);
	cout << currentColor<<endl;
	cout << Max_Min_Test(chessboard, -64, 64, currentColor) << endl;
	//printCurrent(pointerChessboard);
   //for (i = 0; i < 10; i++) cout << laststeps[i] << ' ';
	
	

	//黑棋（1），白棋（2）， referee 返回值 设为 黑棋数减白棋数，
	//黑棋需求为极大。
	getchar();
	return 0;
}

