#include<bits/stdc++.h> // to include all the function
#include<conio.h> // to check if a key is pressed or not
#include<windows.h> // to draw the poins(co-ordinate)

using namespace std;

#define MAX_LENGTH 1000 // max size of our snake will be 1000

// Directions
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

void initScreen(){
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  //gives to handle control of our console windows
	CONSOLE_SCREEN_BUFFER_INFO csbi;  //storees information of buffering
	GetConsoleScreenBufferInfo(hConsole, &csbi);
	consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1; // +1 for minimum height 1
	consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1; // +1 for minimum height 1
}
	
struct Point{
	int xCoord;
	int yCoord;
	Point(){
	}
	Point(int x, int y){
		xCoord = x;
		yCoord = y;
	}
};

// all about snake  {body, direcction , size}
class Snake{
	int length;
	char direction;

public:
	Point body[MAX_LENGTH];  // make it public to show on console;
	Snake(int x, int y){ // storing the co-ordintes of bod of our snake
		length = 1;
		body[0] = Point(x,y);
		direction = DIR_RIGHT;
	}
	
	int getLength(){
		return length;
	}
	
	void changeDirection(char newDirection){
		if(newDirection == DIR_UP && direction != DIR_DOWN){
			direction = newDirection; 
		}
		else if(newDirection == DIR_DOWN && direction != DIR_UP){
			direction = newDirection; 
		}
		else if(newDirection == DIR_LEFT && direction != DIR_RIGHT){
			direction = newDirection; 
		}
		else if(newDirection == DIR_RIGHT && direction != DIR_LEFT){
			direction = newDirection; 
		}
	}
	
	bool move(Point food){ 
	    // for movement of snake body
		for( int i = length-1; i>0; i--){
			body[i].xCoord  = body[i-1].xCoord % consoleWidth;
			body[i].yCoord  = body[i-1].yCoord % consoleHeight;
		}
		
		// for position of head
		switch(direction){
			int val;
			case DIR_UP:
				val = body[0].yCoord;
				body[0].yCoord = val-1;
				break;
			case DIR_DOWN:
				val = body[0].yCoord;
				body[0].yCoord = val+1;
				break;
			case DIR_RIGHT:
				val = body[0].xCoord;
				body[0].xCoord = val+1;
				break;
			case DIR_LEFT:
				val = body[0].xCoord;
				body[0].xCoord = val-1;
				break;	
		}
		
		 // Handle wrap-around at the screen boundaries
		    if (body[0].xCoord < 0) {
		        body[0].xCoord = consoleWidth - 1;
		    } else if (body[0].xCoord >= consoleWidth) {
		        body[0].xCoord = 0;
		    }
		
		    if (body[0].yCoord < 0) {
		        body[0].yCoord = consoleHeight - 1;
		    } else if (body[0].yCoord >= consoleHeight) {
		        body[0].yCoord = 0;
		    }
		
		//snake bites itself
		for(int i = 1 ; i < length; i++){
			if(body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord){
				return false;
			}
		}
		
		//snake eats food
		if(food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord){
			body[length] = Point(body[length-1].xCoord, body[length-1].yCoord);
			length++;
		}
		
		return true;
	}
	
};

class Board{
	Snake  *snake;  // use * for dynamic memory allocation  , define in heap {-> used}
	const char SNAKE_BODY = 'O';
	Point food; // stack memory allocatin {. used}
	const char FOOD = '*';
	int score;
public:
	Board(){
		spawnFood();
		snake  = new Snake(10,10);
		score = 0;
	}
	
	~Board(){ // destructor to delete snake
		delete snake;
	}
	
	int getScore(){
		return score;
	}
	
	void spawnFood(){
		int x = rand() % consoleWidth; // rand is used to generate random number 
		int y = rand() % consoleHeight; 
		food = Point(x,y);
	}
	
	void gotoxy(int x, int y){	
	// availiable in dispaly header file
		COORD coord;
		coord.X = x;
		coord.Y = y;
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); 
	}
	
	
	void draw(){
		system("cls");    //to clear our console window
		for(int i = 0 ; i < snake->getLength() ; i++){
			gotoxy(snake->body[i].xCoord, snake->body[i].yCoord); // using -> because snake is dynamic memory allocation
			cout << SNAKE_BODY;
		}
		gotoxy(food.xCoord, food.yCoord); // using dot operator because food is stack memory allocation
		cout << FOOD;
		
		// to remove pointer from the screen
    	gotoxy(consoleWidth-1, consoleHeight-1);
	}
	
	bool update(){
		bool isAlive = snake->move(food);
		if(isAlive == false){
			return false;
		}
		
		if(food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord){
			score++;
			spawnFood();
		}
		
		return true;
	}
	
	// to check which key is being pressed
	void getInput(){
		
		if(kbhit()){
			int key = getch();
			if(key == 'w' || key == 'W' || key == '8'){
				snake->changeDirection(DIR_UP);
			}
			else if(key == 'a' || key == 'A' || key == '4'){
				snake->changeDirection(DIR_LEFT);
			}
			else if(key == 's' || key == 'S' || key == '2'){
				snake->changeDirection(DIR_DOWN);
			}
			else if(key == 'd' || key == 'D' || key == '6'){
				snake->changeDirection(DIR_RIGHT);
			}
		}
	}
};

int main(){
	
	initScreen(); // for console
	
	Board *board = new Board(); // for creation of our board
	
	while(board->update()){
		board->getInput();
		board->draw();
		Sleep(250);
	}
	
	cout << "GAME OVER" << endl;
	return 0;
}
