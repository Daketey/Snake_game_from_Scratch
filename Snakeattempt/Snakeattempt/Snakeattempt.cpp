// Snakeattempt.cpp : This file contains the 'main' function. Program execution begins and ends there.

#include<iostream>
#include<thread>
#include<Windows.h>
#include<vector>
#include<cstdlib>
#include<stdlib.h>

using namespace std;

int nScreenWidth = 120;
int nScreenHeight = 40;

int widthoffield = 40;
int heightoffield = 20;

wstring snake[2];

unsigned char *playfield = nullptr;

bool collision(int ntetria, int rotation, int x, int y) {

    for (int px = 0; px < 1; px++) {
        for (int py = 0;py < 1;py++) {

            int pi = px+py;  //Getting index into piece

            int fi = (y + py) * widthoffield + (x + px);  //Getting index into field

            if (x + px >= 0 && x + px < widthoffield) {
                if (y + py >= 0 && y + py < heightoffield) {
                    if (snake[ntetria][pi] == L'X' &&playfield[fi] != 0) {
                        return false;
                    }
                    if (snake[1][pi] == L'X' && playfield[fi] != 0) {
                        return true;
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

    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];                // Create Screen Buffer
    for (int i = 0; i < nScreenWidth * nScreenHeight; i++)
        screen[i] = L' ';
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWritten = 0;

    playfield = new unsigned char[widthoffield * heightoffield];    //Main playing field
    for (int i = 0; i < widthoffield; i++) {
        for (int j = 0; j < heightoffield; j++) {
            playfield[j * widthoffield + i] = (i == 0 || j ==0 || i == widthoffield - 1 || j == heightoffield - 1) ? 1 : 0;    //Draws a square
        }
    }
        
    bool gameover = false;
    int currentpiece = 0;
    int currentrotation = 0;
    int currentx = widthoffield / 2;
    int currenty = heightoffield / 2;
    int keys[4];
    bool keyhold = false;
    bool keyhold1 = false;
    bool keyhold2 = false;
    bool keyhold3 = false;

    int speed = 3;
    int speedcounter = 0;

    int foodposx = rand()%widthoffield;
    int foodposy = rand()%heightoffield;

    int Snake_pos[40 * 20];
    int snakepos[40 * 20];

    int positions=0;

    bool foodpicked = false;

    int increment = 0;
    int clear=0;


    for (int i = 0;i < 40*20;i++)
    {
        Snake_pos[i] = L' ';
        snakepos[i] = 0;

    }

    while (!gameover) {

        this_thread::sleep_for(50ms);
        speedcounter++;

        for (int k = 0; k < 4; k++) {
            keys[k] = (0x8000 & GetAsyncKeyState((unsigned char)("DASW"[k]))) != 0;        //Key input in hexdecimal form,, \x27 is right key , \x25 is left key , \x28 is down key and we have the R key. 0x8000 gives real time state of the key
        }

        for (int i = 0; i < widthoffield; i++) {                                        //Drawing the field
            for (int j = 0; j < heightoffield;j++) {
                screen[(j + 2) * nScreenWidth + (i + 2)] = L" #O"[playfield[j * widthoffield + i]];  //Characters are written as we want them to be displayed according to index like at index=1 , value is "A "
            }
        }

        if (keys[1]) {
            keyhold1 = true;
            keyhold = false;
            keyhold2 = false;
            keyhold3 = false;
        }
        if (keyhold1) {
            if (speedcounter >= speed && collision(currentpiece, currentrotation, currentx - 1, currenty)) {
                currentx--;
                speedcounter = 0;
            }
        }

        if (keys[0]) {
            keyhold1 = false;
            keyhold = true;
            keyhold2 = false;
            keyhold3 = false;
        }
        if (keyhold) {
            if (speedcounter >= speed && collision(currentpiece, currentrotation, currentx + 1, currenty)) {
                currentx++;
                speedcounter = 0;
            }
        }

        if (keys[3]) {
            keyhold1 = false;
            keyhold = false;
            keyhold2 = false;
            keyhold3 = true;
        }
        if (keyhold3) {
            if (speedcounter >= speed && collision(currentpiece, currentrotation, currentx, currenty - 1)) {
                currenty--;
                speedcounter = 0;
            }
        }

        if (keys[2]) {
            keyhold1 = false;
            keyhold = false;
            keyhold2 = true;
            keyhold3 = false;
        }
        if (keyhold2) {
            if (speedcounter >= speed && collision(currentpiece, currentrotation, currentx, currenty + 1)) {
                currenty++;
                speedcounter = 0;
            }
        }


        if (keyhold|| keyhold1|| keyhold2|| keyhold3)
        {
            positions++;

        }

        //SNAKE DRAWING LOOP

        for (int i = 0; i < 1;i++) {
            for (int j = 0;j < 1;j++) {
                if (snake[currentpiece][i + j] == L'X') {
                    screen[(currenty + j + 2) * nScreenWidth + (currentx + i + 2)] = 'X';
                    Snake_pos[0] = (currenty + j + 2) * nScreenWidth + (currentx + i + 2);
                    if (foodpicked) {
                        snakepos[positions] = (currenty + j) * widthoffield + (currentx + i);
                        playfield[snakepos[positions]] = 2;
                        clear = positions;
                        playfield[snakepos[clear - increment]] = 0;
                    }
                }
            }
        }

       
        screen[(foodposy+2) * nScreenWidth + (foodposx+2)] ='*';
        

        if (Snake_pos[0] == (foodposy+2) * nScreenWidth + (foodposx + 2))
        {
            increment = 3 + increment;
            foodposx = (rand() % ((widthoffield - 1))) + 1;
            foodposy = (rand() % (heightoffield -1))+2;
            foodpicked = true;
        }
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);  //draws to console starting from postion 0,0
    }
        return 0;
    }

	

