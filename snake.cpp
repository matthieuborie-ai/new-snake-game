#include <iostream>
#include <vector>
#include <conio.h>    // _kbhit, _getch   Windows only
#include <windows.h>  // Sleep, system    Windows only
#include <cstdlib>
#include <ctime>

using namespace std;

const int WIDTH = 30;
const int HEIGHT = 20;

enum Direction {
    STOP = 0,
    LEFT_DIR,
    RIGHT_DIR,
    UP_DIR,
    DOWN_DIR
};

struct Position {
    int x;
    int y;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y;
    }
};

bool gameOverFlag;
Direction dir;
vector<Position> snake;
Position food;
int scoreVal;
int speedMs = 120; // smaller is faster

void GenerateFood() {
    while (true) {
        food.x = rand() % WIDTH;
        food.y = rand() % HEIGHT;

        bool onSnake = false;
        for (const auto& part : snake) {
            if (part == food) {
                onSnake = true;
                break;
            }
        }
        if (!onSnake) {
            break;
        }
    }
}

void Setup() {
    gameOverFlag = false;
    dir = RIGHT_DIR;
    snake.clear();

    Position start;
    start.x = WIDTH / 2;
    start.y = HEIGHT / 2;
    snake.push_back(start);

    scoreVal = 0;
    srand(static_cast<unsigned int>(time(nullptr)));

    GenerateFood();
}

void Draw() {
    system("cls");

    cout << "Snake Game C plus plus realtime\n";
    cout << "W A S D to move   X to quit\n";
    cout << "Score " << scoreVal << "\n\n";

    for (int i = 0; i < WIDTH + 2; i++) {
        cout << "#";
    }
    cout << "\n";

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            if (x == 0) {
                cout << "#";
            }

            Position current{ x, y };

            if (snake[0] == current) {
                cout << "O";
            } else if (food == current) {
                cout << "F";
            } else {
                bool bodyPrinted = false;
                for (size_t i = 1; i < snake.size(); i++) {
                    if (snake[i] == current) {
                        cout << "o";
                        bodyPrinted = true;
                        break;
                    }
                }
                if (!bodyPrinted) {
                    cout << " ";
                }
            }

            if (x == WIDTH - 1) {
                cout << "#";
            }
        }
        cout << "\n";
    }

    for (int i = 0; i < WIDTH + 2; i++) {
        cout << "#";
    }
    cout << "\n";
}

void Input() {
    // Non blocking keyboard check
    while (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'a':
            case 'A':
                if (dir != RIGHT_DIR) dir = LEFT_DIR;
                break;
            case 'd':
            case 'D':
                if (dir != LEFT_DIR) dir = RIGHT_DIR;
                break;
            case 'w':
            case 'W':
                if (dir != DOWN_DIR) dir = UP_DIR;
                break;
            case 's':
            case 'S':
                if (dir != UP_DIR) dir = DOWN_DIR;
                break;
            case 'x':
            case 'X':
                gameOverFlag = true;
                break;
            default:
                break;
        }
    }
}

void Logic() {
    Position head = snake[0];

    switch (dir) {
        case LEFT_DIR:
            head.x -= 1;
            break;
        case RIGHT_DIR:
            head.x += 1;
            break;
        case UP_DIR:
            head.y -= 1;
            break;
        case DOWN_DIR:
            head.y += 1;
            break;
        default:
            break;
    }

    if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) {
        gameOverFlag = true;
        return;
    }

    for (size_t i = 1; i < snake.size(); i++) {
        if (snake[i] == head) {
            gameOverFlag = true;
            return;
        }
    }

    snake.insert(snake.begin(), head);

    if (head == food) {
        scoreVal += 10;
        if (speedMs > 40) {
            speedMs -= 2;
        }
        GenerateFood();
    } else {
        snake.pop_back();
    }
}

int main() {
    Setup();

    while (!gameOverFlag) {
        Draw();
        Input();
        Logic();
        Sleep(speedMs);
    }

    system("cls");
    cout << "Game Over\n";
    cout << "Final score " << scoreVal << "\n";
    cout << "Press any key to exit\n";
    _getch();
    return 0;
}
