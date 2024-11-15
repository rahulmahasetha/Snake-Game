#include<bits/stdc++.h>
#include<conio.h>
#include<windows.h>

using namespace std;

#define MAX_LENGTH 1000

// Directions
const char DIR_UP = 'U';
const char DIR_DOWN = 'D';
const char DIR_LEFT = 'L';
const char DIR_RIGHT = 'R';

int consoleWidth, consoleHeight;

void initScreen() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    consoleHeight = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
    consoleWidth = csbi.srWindow.Right - csbi.srWindow.Left + 1;
}

struct Point {
    int xCoord, yCoord;
    Point(int x = 0, int y = 0) : xCoord(x), yCoord(y) {}
};

class Snake {
    int length;
    char direction;
public:
    Point body[MAX_LENGTH];
    Snake(int x, int y) : length(1), direction(DIR_RIGHT) {
        body[0] = Point(x, y);
    }

    int getLength() { return length; }

    void changeDirection(char newDirection) {
        if (newDirection == DIR_UP && direction != DIR_DOWN)
            direction = newDirection;
        else if (newDirection == DIR_DOWN && direction != DIR_UP)
            direction = newDirection;
        else if (newDirection == DIR_LEFT && direction != DIR_RIGHT)
            direction = newDirection;
        else if (newDirection == DIR_RIGHT && direction != DIR_LEFT)
            direction = newDirection;
    }

    bool move(Point food) {
        for (int i = length - 1; i > 0; i--)
            body[i] = body[i - 1];

        switch (direction) {
            case DIR_UP: body[0].yCoord--; break;
            case DIR_DOWN: body[0].yCoord++; break;
            case DIR_RIGHT: body[0].xCoord++; break;
            case DIR_LEFT: body[0].xCoord--; break;
        }

        // Check if snake bites itself
        for (int i = 1; i < length; i++) {
            if (body[0].xCoord == body[i].xCoord && body[0].yCoord == body[i].yCoord)
                return false;
        }

        // Snake eats food
        if (food.xCoord == body[0].xCoord && food.yCoord == body[0].yCoord) {
            if (length < MAX_LENGTH) {
                body[length] = body[length - 1];
                length++;
            }
        }
        return true;
    }
};

class Board {
    Snake* snake;
    Point food;
    int score;
    const char SNAKE_BODY = 'O';
    const char FOOD = 'o';

public:
    Board() : score(0) {
        initScreen();
        spawnFood();
        snake = new Snake(10, 10);
    }

    ~Board() { delete snake; }

    int getScore() { return score; }

    void spawnFood() {
        int x = rand() % consoleWidth;
        int y = rand() % consoleHeight;
        food = Point(x, y);
    }

    void displayCurrentScore() {
        gotoxy(consoleWidth / 2, 0);
        cout << "Current Score: " << score;
    }

    void gotoxy(int x, int y) {
        COORD coord = {short(x), short(y)};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    }

    void draw() {
        system("cls");
        for (int i = 0; i < snake->getLength(); i++) {
            gotoxy(snake->body[i].xCoord, snake->body[i].yCoord);
            cout << SNAKE_BODY;
        }
        gotoxy(food.xCoord, food.yCoord);
        cout << FOOD;
        displayCurrentScore();
    }

    bool update() {
        if (!snake->move(food)) return false;

        if (food.xCoord == snake->body[0].xCoord && food.yCoord == snake->body[0].yCoord) {
            score++;
            spawnFood();
        }
        return true;
    }

    void getInput() {
        if (_kbhit()) {
            int key = _getch();
            if (key == 'w' || key == 'W') snake->changeDirection(DIR_UP);
            else if (key == 'a' || key == 'A') snake->changeDirection(DIR_LEFT);
            else if (key == 's' || key == 'S') snake->changeDirection(DIR_DOWN);
            else if (key == 'd' || key == 'D') snake->changeDirection(DIR_RIGHT);
        }
    }
};

int main() {
    srand(static_cast<unsigned>(time(0)));
    Board* board = new Board();
    while (board->update()) {
        board->getInput();
        board->draw();
        Sleep(100);
    }
    cout << "Game over\nFinal score: " << board->getScore() << endl;
    delete board;
    return 0;
}
