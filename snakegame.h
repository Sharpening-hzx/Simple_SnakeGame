#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#include <conio.h>
#include <easyx.h>
#include <graphics.h>
#include <time.h>

#define Screen_Width 1000
#define Screen_Height 750

#define Game_Width 750
#define Game_Height 750
#define Grid_Size 10



typedef struct snakenode
{
    int x;
    int y;
    struct snakenode* next;
} SnakeNode, * P_SnakeNode;

typedef struct snake
{
    int direction;
    P_SnakeNode head;
    P_SnakeNode tail;

}Snake,*P_Snake;

typedef struct foodnode
{
    int x;
    int y;
}Food,*P_Food;

void InitGame();
void DrawWall();
void InitSnake();
void PrintSnake();
void KeyboardControl();
void Grow(P_Snake snake);
void DrawSnake(P_Snake snakes);
void GenerateFood();
void DrawFood();
void MoveSnake(P_Snake snake);

int CheakEatSelf(P_Snake snake);
