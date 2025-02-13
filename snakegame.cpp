#include "snakegame.h"

Snake snake;
P_Food food;
int speed = 100;
int speedfactor = 1;
int liveflag = 1;
int message_x = Game_Width + 5 * Grid_Size;
int message_y = 5 * Grid_Size;
int score = 0;
TCHAR losemessage[] = _T("��ʧ����");
void InitGame()
{
    srand(time(NULL)); // ֻ����Ϸ��ʼ��ʱ����һ�ξ���
    initgraph(Screen_Width, Screen_Height);
    InitSnake();
    food = (P_Food)malloc(sizeof(Food)); // ����ʳ���ڴ�
    if (!food) {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    GenerateFood();

    setbkmode(TRANSPARENT);

    while (liveflag) {
        KeyboardControl();
        BeginBatchDraw();
        cleardevice();
        DrawWall();
        DrawFood();
        DrawSnake(&snake);
        MoveSnake(&snake);
        EndBatchDraw();
        Sleep(speed/speedfactor);
    }
    _getch();
}



void DrawWall()
{
    for (int i = 0; i < Game_Width; i += Grid_Size)
    {
        rectangle(i, 0, i + Grid_Size, Grid_Size);
    }
    for (int i = 0; i < Game_Height; i += Grid_Size)
    {
        rectangle(0, i, Grid_Size, i + Grid_Size);
    }
    for (int i = 0; i <= Game_Width - Grid_Size; i += Grid_Size)
    {
        rectangle(i, Game_Height - Grid_Size, i + Grid_Size, Game_Height);
    }
    for (int i = 0; i <= Game_Height - Grid_Size; i += Grid_Size)
    {
        rectangle(Game_Width - Grid_Size, i, Game_Width, i + Grid_Size);
    }
}

void InitSnake()
{
    snake.head = (P_SnakeNode)malloc(sizeof(SnakeNode));
    if (!snake.head) {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }

    snake.head->next = NULL;
    snake.tail = snake.head;

    // ��ʼ����ͷ��λ��
    snake.head->x = 100;
    snake.head->y = 100;
    snake.direction = 3; // ��ʼ��������

    // ��ʼ���ߵ����岿��
    for (int i = 0; i < 2; i++)
    {
        P_SnakeNode newNode = (P_SnakeNode)malloc(sizeof(SnakeNode));
        if (!newNode) {
            printf("�ڴ����ʧ�ܣ�\n");
            exit(1);
        }

        newNode->next = NULL;
        newNode->x = snake.head->x - (i + 1) * Grid_Size; // ���岿����ͷ�������
        newNode->y = snake.head->y;

        snake.tail->next = newNode;
        snake.tail = newNode;
    }
}

void Grow(P_Snake snake)
{
    P_SnakeNode newNode = (P_SnakeNode)malloc(sizeof(SnakeNode));
    if (!newNode) {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }

    newNode->next = NULL;
    newNode->x = snake->tail->x;
    newNode->y = snake->tail->y;
    switch (snake->direction) {
    case 0: newNode->y -= Grid_Size; break;  // ����
    case 1: newNode->y += Grid_Size; break;  // ����
    case 2: newNode->x -= Grid_Size; break;  // ����
    case 3: newNode->x += Grid_Size; break;  // ����
    }
    

    snake->tail->next = newNode;
    snake->tail = newNode;
}

void DrawSnake(P_Snake snakes)
{
    P_SnakeNode current = snakes->head;

    setfillcolor(GREEN); // ���������ɫΪ��ɫ
    while (current)
    {
        if (current == snakes->head) {
            setfillcolor(RED);  // ����ͷ��ɺ�ɫ
        }
        else {
            setfillcolor(GREEN);
        }

        solidrectangle(current->x, current->y, current->x + Grid_Size, current->y + Grid_Size);
        current = current->next;
    }
}


void GenerateFood()
{
    int valid = 0;
    while (!valid) {
        food->x = (rand() % (Game_Width / Grid_Size)) * Grid_Size;
        food->y = (rand() % (Game_Height / Grid_Size)) * Grid_Size;

        // ����Ƿ�������������
        P_SnakeNode temp = snake.head;
        valid = 1; 
        while (temp) {
            if (temp->x == food->x && temp->y == food->y) {
                valid = 0;
                break;
            }
            temp = temp->next;
        }

        // ����Ƿ�������ǽ��
        if (food->x == 0 || food->y == 0 ||
            food->x == Game_Width - Grid_Size || food->y == Game_Height - Grid_Size) {
            valid = 0;
        }
    }
    printf("��ʳ������: (%d, %d)\n", food->x, food->y);
}


void DrawFood()
{
    setfillcolor(YELLOW); // �趨ʳ����ɫΪ��ɫ
    solidrectangle(food->x, food->y, food->x + Grid_Size, food->y + Grid_Size);
}





void MoveSnake(P_Snake snake) {
    // 1. ��������ͷ
    P_SnakeNode newHead = (P_SnakeNode)malloc(sizeof(SnakeNode));
    if (!newHead) {
        printf("�ڴ����ʧ�ܣ�\n");
        exit(1);
    }
    newHead->x = snake->head->x;
    newHead->y = snake->head->y;

    // 2. ��������ͷ��λ��
    switch (snake->direction) {
    case 0: newHead->y -= Grid_Size; break;  // ����
    case 1: newHead->y += Grid_Size; break;  // ����
    case 2: newHead->x -= Grid_Size; break;  // ����
    case 3: newHead->x += Grid_Size; break;  // ����
    }

    // 3. ��������ͷ
    newHead->next = snake->head;
    snake->head = newHead;
    if (snake->head->x == 0 || snake->head->y == 0
        || snake->head->x == Game_Width - Grid_Size || snake->head->y == Game_Height - Grid_Size)
    {

        liveflag = 0;
        printf("��ʧ����!!!");
        outtextxy(message_x,message_y,losemessage);
    }
    if (CheakEatSelf(snake))
    {
        liveflag = 0;
        printf("��ʧ����!!!");
        outtextxy(message_x, message_y, losemessage);
    }
    // 4. �ж��Ƿ�Ե�ʳ��
    if (newHead->x == food->x && newHead->y == food->y) {
        /*score += 10;*/
        GenerateFood(); // ��������ʳ��
        printf("�Ե�ʳ��!\n");  // ������Ϣ
    }
    else {
        // û�гԵ�ʳ��,ɾ��β��
        P_SnakeNode temp = snake->head;
        while (temp->next && temp->next != snake->tail) {
            temp = temp->next;
        }

        free(snake->tail); // �ͷž�β��
        temp->next = NULL;
        snake->tail = temp; // ����β��
    }
    printf("��ͷ����: (%d, %d)\n", snake->head->x, snake->head->y);
}

int CheakEatSelf(P_Snake snake)
{
    P_SnakeNode current = snake->head->next;
    while (current)
    {
        if (current->x == snake->head->x && current->y == snake->head->y)
        {
            printf("ҧ�����Լ�!\n");
            return 1;
        }
        current = current->next;
    }
    return 0;
}






void PrintSnake()
{
    P_SnakeNode current = snake.head;
    while (current)
    {
        printf("(%d, %d) -> ", current->x, current->y);
        current = current->next;
    }
    printf("NULL\n");
}

void KeyboardControl() {
    if (GetAsyncKeyState('W') & 0x8000) {
        if (snake.direction != 1)
            snake.direction = 0;
    } // ��

    if (GetAsyncKeyState('S') & 0x8000) {
        if (snake.direction != 0)
            snake.direction = 1;
    } // ��

    if (GetAsyncKeyState('A') & 0x8000) {
        if (snake.direction != 3)
            snake.direction = 2;
    } // ��

    if (GetAsyncKeyState('D') & 0x8000) {
        if (snake.direction != 2)
            snake.direction = 3;
    } // ��

    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        if (snake.direction != 1)
            snake.direction = 0;
    } // ��

    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        if (snake.direction != 0)
            snake.direction = 1;
    } // ��

    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        if (snake.direction != 3)
            snake.direction = 2;
    } // ��

    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        if (snake.direction != 2)
            snake.direction = 3;
    } // ��
}

