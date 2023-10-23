#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>

#define MAX_INPUT_SIZE 1024
#define MAX_ARGS 100
#define HEIGHT 20
#define WIDTH 20

int gameover, score;
int x, y, fruitX, fruitY;
int tailX[MAX_ARGS], tailY[MAX_ARGS], nTail;
enum eDirection { STOP = 0, LEFT, RIGHT, UP, DOWN };
enum eDirection dir;

struct termios original_termios;

void setup() {
    gameover = 0;
    dir = STOP;
    x = HEIGHT / 2;
    y = WIDTH / 2;
    fruitX = rand() % HEIGHT;
    fruitY = rand() % WIDTH;
    score = 0;
}

void draw() {
    system("clear");

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (j == 0 || j == WIDTH - 1)
                printf("#");
            else if (i == x && j == y)
                printf("O");
            else if (i == fruitX && j == fruitY)
                printf("F");
            else {
                int isprint = 0;
                for (int k = 0; k < nTail; k++) {
                    if (tailX[k] == i && tailY[k] == j) {
                        printf("o");
                        isprint = 1;
                    }
                }
                if (isprint == 0)
                    printf(" ");
            }
        }
        printf("\n");
    }

    printf("Score: %d\n", score);
}

void input() {
    char buf = 0;
    struct termios old = {0};

    if (tcgetattr(0, &original_termios) < 0)
        perror("tcsetattr()");
    old = original_termios;
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
        perror("tcsetattr ICANON");

    if (read(0, &buf, 1) < 0)
        perror("read()");

    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
        perror("tcsetattr ~ICANON");
    switch (buf) {
    case 'a':
        dir = LEFT;
        break;
    case 'd':
        dir = RIGHT;
        break;
    case 'w':
        dir = UP;
        break;
    case 's':
        dir = DOWN;
        break;
    case 'x':
        gameover = 1;
        break;
    }
}

void logic() {
    int prevX = tailX[0];
    int prevY = tailY[0];
    int prev2X, prev2Y;
    tailX[0] = x;
    tailY[0] = y;

    for (int i = 1; i < nTail; i++) {
        prev2X = tailX[i];
        prev2Y = tailY[i];
        tailX[i] = prevX;
        tailY[i] = prevY;
        prevX = prev2X;
        prevY = prev2Y;
    }

    switch (dir) {
    case LEFT:
        y--;
        break;
    case RIGHT:
        y++;
        break;
    case UP:
        x--;
        break;
    case DOWN:
        x++;
        break;
    default:
        break;
    }

    if (x >= HEIGHT || x < 0 || y >= WIDTH || y < 0)
        gameover = 1;

    for (int i = 0; i < nTail; i++)
        if (tailX[i] == x && tailY[i] == y)
            gameover = 1;

    if (x == fruitX && y == fruitY) {
        score += 10;
        fruitX = rand() % HEIGHT;
        fruitY = rand() % WIDTH;
        nTail++;
    }
}

int main() {

    printf("Use WASD to move! Have fun!");
    
    setup();

    while (!gameover) {
        draw();
        input();
        logic();
        usleep(100000); // Sleep for 100 milliseconds (0.1 seconds)
    }

    return 0;
}
