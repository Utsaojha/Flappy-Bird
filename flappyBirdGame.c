#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#include <time.h>

#define xSize       32
#define ySize       16
#define pipeCount   3
#define qKey        0x51
#define GREEN       "\e[32m"
#define YELLOW      "\e[33m"
#define NC          "\e[0m"

typedef struct
{
    int x;
    int y;
} PIX;

PIX ball;
PIX pipes[3];

int score = 0;

void Draw();
void Pipes();
void HitTest();

void Draw()
{
    system("cls");
    char buff[5000];
    strcpy(buff, "\e[17A");

    for (int y = 0; y <= ySize; y++)
    {
        for (int x = 0; x <= xSize; x++)
        {
            if (
                y == 0 ||
                y == ySize ||
                x == 0 ||
                x == xSize
            )
            {
                strcat(buff, NC "[]");
                continue;
            }

            for (int i = 0; i < pipeCount; i++)
            {
                if (
                    pipes[i].x == x &&
                    (
                        pipes[i].y <= y - 5 ||
                        pipes[i].y >= y + 5
                    )
                )
                {
                    strcat(buff, GREEN "[]");
                    goto bottom;
                }
            }

            if
            (
                ball.y == y &&
                ball.x == x
            )
            {
                strcat(buff, YELLOW "o");
            }
            else if
            (
                ball.y == y - 1 &&
                ball.x == x
            )
            {
                strcat(buff, YELLOW "()");
            }
            else
            {
                strcat(buff, NC "  ");
            }

            bottom:;
        }

        strcat(buff, "\n");
    }

    printf("%s\nScore: %d\n", buff, score);
}

void Pipes()
{
    for (int i = 0; i < pipeCount; i++)
    {
        if (pipes[i].x == -1)
        {
            (i == 0) ? (pipes[i].x = pipes[2].x + 15) : (pipes[i].x = pipes[i - 1].x + 15);
            pipes[i].y = (rand() % 7) + 5;
        }
    }
}

void HitTest()
{
    if (ball.y == 15)
    {
        exit(0);
    }

    for (int i = 0; i < pipeCount; i++)
    {
        if (
            (ball.x - 1 < pipes[i].x + 1) &&
            (ball.x + 1 > pipes[i].x - 1) &&
            (
                (ball.y < pipes[i].y - 2) ||
                (ball.y > pipes[i].y + 2)
            )
        )
        {
            exit(0);
        }
    }

    // Increase score if the ball successfully passes through a set of pipes
    for (int i = 0; i < pipeCount; i++)
    {
        if (ball.x == pipes[i].x)
        {
            score++;
        }
    }
}

int main()
{
    srand(time(NULL));
    system("title \"Flappy Ball\"");

    ball.x = 10;
    ball.y = 10;

    for (int i = 0; i < pipeCount; i++)
    {
        pipes[i].x = 25 + 15 * i;
        pipes[i].y = (rand() % 7) + 5;
    }

    int frame = 0;

    printf("Press SPACE to jump and Q to quit.\n");

    for (int i = 0; i <= ySize; i++)
    {
        printf("\n");
    }

    Draw();

    system("pause>nul");

    while (1)
    {
        if (_kbhit() && _getch() == 32)
        {
            ball.y -= 2;
        }

        if (_kbhit() && _getch() == qKey)
        {
            break;
        }

        if (frame == 2)
        {
            ball.y++;
            for (int i = 0; i < 3; i++)
            {
                pipes[i].x--;
            }
            frame = 0;
        }

        HitTest();
        Draw();
        Pipes();

        frame++;
        Sleep(100);
    }

    return 0;
}
