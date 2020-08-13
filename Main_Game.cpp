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

wstring snake[2];

unsigned char *playField = nullptr;

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

	snake[0] = L"X";                                    //Sprite for Snake Head
    snake[1] = L"*";

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
    bool keyHold_0 = false;
    bool keyHold_1 = false;
    bool keyHold_2 = false;
    bool keyHold_3 = false;

    int speed = 2;
    int speedCounter = 0;

    int foodPos_X = Width_of_Field / 3;
    int foodPos_Y = Height_of_Field / 4;

    int Snake_pos;
    vector<int> snakePos;

    int positions=-1;

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
            keyHold_1 = true;
            keyHold_0 = false;
            keyHold_2 = false;
            keyHold_3 = false;
        }
        if (keyHold_1) {
            if (speedCounter >= speed && collision(currentPiece, current_X - 1, current_Y)) {
                current_X--;
                speedCounter = 0;
            }
        }

        if (keys[0]) {
            keyHold_1 = false;
            keyHold_0 = true;
            keyHold_2 = false;
            keyHold_3 = false;
        }
        if (keyHold_0) {
            if (speedCounter >= speed && collision(currentPiece, current_X + 1, current_Y)) {
                current_X++;
                speedCounter = 0;
            }
        }

        if (keys[3]) {
            keyHold_1 = false;
            keyHold_0 = false;
            keyHold_2 = false;
            keyHold_3 = true;
        }
        if (keyHold_3) {
            if (speedCounter >= speed && collision(currentPiece, current_X, current_Y - 1)) {
                current_Y--;
                speedCounter = 0;
            }
        }

        if (keys[2]) {
            keyHold_1 = false;
            keyHold_0 = false;
            keyHold_2 = true;
            keyHold_3 = false;
        }
        if (keyHold_2) {
            if (speedCounter >= speed && collision(currentPiece, current_X, current_Y + 1)) {
                current_Y++;
                speedCounter = 0;
            }
        }

        if (keyHold_0 || keyHold_1 || keyHold_2 || keyHold_3){
        
            positions++;

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
                foodPos_X = (rand() % (Width_of_Field - 2)) + 1;
                foodPos_Y = (rand() % (Height_of_Field - 2)) + 1;
                foodPicked = true;
            }
        }
            
        //FOOD POS
        for (int i = 0; i < 1;i++) {
            for (int j = 0;j < 1;j++) {
                if (snake[1][i + j] == L'*') {
                    screen[(foodPos_Y + j + 2) * ScreenWidth + (foodPos_X + i + 2)] = '*';
                }
            }
        }

        WriteConsoleOutputCharacter(hConsole, screen, ScreenWidth * ScreenHeight, { 0,0 }, &dwBytesWritten);  //draws to console starting from postion 0,0
    }
        return 0;
    }

	

