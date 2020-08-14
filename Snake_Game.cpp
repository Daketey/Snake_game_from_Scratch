// Snakeattempt.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include<iostream>
#include<thread>
#include<Windows.h>
#include<vector>
#include<time.h>>

using namespace std;

int ScreenWidth = 120;
int ScreenHeight = 40;

int Width_of_Field = 40;
int Height_of_Field = 20;

char snake[3];

unsigned char *playField = nullptr;

bool hitObstacle(bool keyhold, int x, int y,wchar_t *screen) {
    if (keyhold) {
        if (screen[(y) * ScreenWidth + (x)] == L'#') {
            return true;
        }
        else if (screen[(y) * ScreenWidth + (x)] == L'O') {
            return true;
        }
    }
    return false;

}

bool collision(int ntetria, int x, int y) {

    for (int px = 0; px < 1; px++) {
        for (int py = 0;py < 1;py++) {

            int pi = px+py;  //Getting index into piece

            int fi = (y + py) * Width_of_Field + (x + px);  //Getting index into field

            if (x + px >= 0 && x + px < Width_of_Field) {
                if (y + py >= 0 && y + py < Height_of_Field) {
                    if (snake[ntetria][pi] == L'X' &&playField[fi] != 0) {
                        return false;
                    }
                }
            }
        }
    }
    return true;
}   

int main(){

	snake[0] = "X";                                    //Sprite for Snake Head
    snake[1] = "*";

    wchar_t* screen = new wchar_t[ScreenWidth * ScreenHeight];                // Create Screen Buffer
    for (int i = 0; i < ScreenWidth * ScreenHeight; i++)
        screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    playField = new unsigned char[Width_of_Field * Height_of_Field];    //Main playing field
    for (int i = 0; i < Width_of_Field; i++) {
        for (int j = 0; j < Height_of_Field; j++) {
            playField[j * Width_of_Field + i] = (i == 0 || j ==0 || i == Width_of_Field - 1 || j == Height_of_Field - 1) ? 1 : 0;    //Draws a square
        }
    }

    srand(time(0));
        
    bool gameOver = false;
    int currentPiece = 0;
    int current_X = Width_of_Field / 2;
    int current_Y = Height_of_Field / 2;
    int keys[4];
    bool keyHold_D = false;
    bool keyHold_A = false;
    bool keyHold_S = false;
    bool keyHold_W = false;

    int speed = 2;
    int speedCounter = 0;

    int foodPos_X = Width_of_Field / 2;
    int foodPos_Y = Height_of_Field / 4;

    int Snake_pos;
    vector<int> snakePos;

    int positions=-1;

    int colposx = current_X;
    int colposy = current_Y;

    bool foodPicked = false;

    int increment = 0;


    while (!gameOver) {

        this_thread::sleep_for(50ms);
        speedCounter++;

        for (int i = 0; i < Width_of_Field; i++) {                                        //Drawing the field
            for (int j = 0; j < Height_of_Field;j++) {
                screen[(j + 2) * ScreenWidth + (i + 2)] = L" #O"[playField[j * Width_of_Field + i]];  //Characters are written as we want them to be displayed according to index like at index=1 , value is "A "
            }
        }

        for (int k = 0; k < 4; k++) {
            keys[k] = (0x8000 & GetAsyncKeyState((unsigned char)("DASW"[k]))) != 0;        //Key input "D" for right "A" for left "S" for down "W" for up. 0x8000 gives real time state of the key
        }

        if (keys[1]) {
            if (keyHold_D == false) {
                keyHold_A = true;
                keyHold_S = false;
                keyHold_W = false;
            }
        }
        if (keyHold_A) {
            if (speedCounter >= speed && collision(currentPiece, current_X - 1, current_Y)) {
                current_X--;
                speedCounter = 0;
            }
        }

        if (keys[0]) {
            if (keyHold_A == false) {
                keyHold_D = true;
                keyHold_S = false;
                keyHold_W = false;
            }
        }
        if (keyHold_D) {
            if (speedCounter >= speed && collision(currentPiece, current_X + 1, current_Y)) {
                current_X++;
                speedCounter = 0;
            }
        }

        if (keys[3]) {
            if (keyHold_S == false) {
                keyHold_A = false;
                keyHold_D = false;
                keyHold_W = true;
            }
        }
        if (keyHold_W) {
            if (speedCounter >= speed && collision(currentPiece, current_X, current_Y - 1)) {
                current_Y--;
                speedCounter = 0;
            }
        }

        if (keys[2]) {
            if (keyHold_W == false) {
                keyHold_A = false;
                keyHold_D = false;
                keyHold_S = true;
            }
        }
        if (keyHold_S) {
            if (speedCounter >= speed && collision(currentPiece, current_X, current_Y + 1)) {
                current_Y++;
                speedCounter = 0;
            }
        }

        
        if (keyHold_D || keyHold_A || keyHold_S || keyHold_W){
        
            positions++;
            speed = 2;
            //SNAKE DRAWING LOOP

            for (int i = 0; i < 1;i++) {
                for (int j = 0;j < 1;j++) {
                    if (snake[currentPiece][i + j] == L'X') {
                        screen[(current_Y + j + 2) * ScreenWidth + (current_X + i + 2)] = 'X';
                        Snake_pos = (current_Y + j + 2) * ScreenWidth + (current_X + i + 2);
                        snakePos.push_back((current_Y + j) * Width_of_Field + (current_X + i));                      
                        if (foodPicked) {
                            playField[snakePos[positions]] = 2;
                            playField[snakePos[positions - increment]] = NULL;
                        }
                    }
                }
            }

            if (Snake_pos == (foodPos_Y + 2) * ScreenWidth + (foodPos_X + 2))
            {
                increment = 2 + increment;
                foodPos_X = (rand() % (Width_of_Field - 3)) + 2;
                foodPos_Y = (rand() % (Height_of_Field - 4)) + 3;
                foodPicked = true;
            }
        }

        //FOOD DRAWING LOOP
        for (int i = 0; i < 1;i++) {
            for (int j = 0;j < 1;j++) {
                if (snake[1][i + j] == L'*') {
                    screen[(foodPos_Y + j + 2) * ScreenWidth + (foodPos_X + i + 2)] = '*';
                }
            }
        }


        if (hitObstacle(keyHold_D, current_X + 3, current_Y + 2, screen)) {
            gameOver = true;
        }
        else if (hitObstacle(keyHold_A, current_X + 1, current_Y + 2, screen)) {
            gameOver = true;
        }
        else if (hitObstacle(keyHold_S, current_X + 2, current_Y + 3, screen)) {
            gameOver = true;
        }
        else if (hitObstacle(keyHold_W, current_X + 2, current_Y + 1, screen)) {
            gameOver = true;
        }

        WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);  //draws to console starting from postion 0,0
    }
        return 0;
    }

	

