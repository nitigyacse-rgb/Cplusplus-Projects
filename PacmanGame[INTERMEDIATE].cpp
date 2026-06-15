#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define WIDTH 20
#define HEIGHT 10

int gameOver, score;
int pacX, pacY;
int ghostX, ghostY;

char maze[HEIGHT][WIDTH + 1] = {
    "###################",
    "#........#........#",
    "#.####...#...####.#",
    "#.................#",
    "#.###.#####.###...#",
    "#........#........#",
    "#.####...#...####.#",
    "#.................#",
    "#........#........#",
    "###################"};

// Setup
void setup()
{
    gameOver = 0;
    score = 0;
    pacX = 1;
    pacY = 1;
    ghostX = WIDTH - 2;
    ghostY = HEIGHT - 2;
}

// Clear screen buffer (NO flicker, NO overlap)
void clearScreen()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);

    COORD topLeft = {0, 0};
    DWORD written;

    CONSOLE_SCREEN_BUFFER_INFO screen;
    GetConsoleScreenBufferInfo(hOut, &screen);

    FillConsoleOutputCharacter(
        hOut, ' ',
        screen.dwSize.X * screen.dwSize.Y,
        topLeft, &written);

    SetConsoleCursorPosition(hOut, topLeft);
}

// Draw
void draw()
{
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD coord = {0, 0};
    SetConsoleCursorPosition(hOut, coord);

    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if (i == pacY && j == pacX)
                printf("C");
            else if (i == ghostY && j == ghostX)
                printf("G");
            else
                printf("%c", maze[i][j]);
        }
        printf("\n");
    }

    printf("\nScore: %d\n", score);
    printf("Controls: W A S D | X to Exit\n");
}
// Input (smooth real-time)
void input()
{
    int newX = pacX;
    int newY = pacY;

    if (GetAsyncKeyState('A') & 0x8000)
        newX--;
    if (GetAsyncKeyState('D') & 0x8000)
        newX++;
    if (GetAsyncKeyState('W') & 0x8000)
        newY--;
    if (GetAsyncKeyState('S') & 0x8000)
        newY++;
    if (GetAsyncKeyState('X') & 0x8000)
        gameOver = 1;

    if (newX >= 0 && newX < WIDTH &&
        newY >= 0 && newY < HEIGHT &&
        maze[newY][newX] != '#')
    {

        pacX = newX;
        pacY = newY;
    }
}

// Logic
void logic()
{
    // Eat food
    if (maze[pacY][pacX] == '.')
    {
        score += 10;
        maze[pacY][pacX] = ' ';
    }

    // Ghost chase
    int newGX = ghostX;
    int newGY = ghostY;

    if (pacX < ghostX)
        newGX--;
    else if (pacX > ghostX)
        newGX++;
    else if (pacY < ghostY)
        newGY--;
    else if (pacY > ghostY)
        newGY++;

    if (newGX >= 0 && newGX < WIDTH &&
        newGY >= 0 && newGY < HEIGHT &&
        maze[newGY][newGX] != '#')
    {

        ghostX = newGX;
        ghostY = newGY;
    }

    // Collision
    if (pacX == ghostX && pacY == ghostY)
    {
        gameOver = 1;
    }
}

int main()
{
    setup();

    // Hide cursor
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);

    // Clear once at start (removes PowerShell junk)
    system("cls");

    while (!gameOver)
    {
        draw();
        input();
        logic();
        Sleep(100);
    }

    clearScreen();
    printf("Game Over! Final Score: %d\n", score);

    return 0;
}