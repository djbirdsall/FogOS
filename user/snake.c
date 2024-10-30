#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define WIDTH 20
#define HEIGHT 10
#define SNAKE_MAX_LENGTH 20

uint32 rand() {
    uint32 x = uptime();
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    return x;
}

void move_cursor() {
    printf("\033[%d;1H", HEIGHT + 3);
}

void clear_screen() {
    printf("\033[H\033[J");
}

void draw_border() {
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");
    for (int i = 0; i < HEIGHT; i++) {
        printf("#");
        for (int j = 0; j < WIDTH; j++) printf(" ");
        printf("#\n");
    }
    for (int i = 0; i < WIDTH + 2; i++) printf("#");
    printf("\n");
}

void draw_snake(Snake *snake) {
    for (int i = 0; i < snake->length; i++) {
        printf("\033[%d;%dH@", snake->body[i].y + 2, snake->body[i].x + 2);
    }
}

void draw_food(Point *food) {
    printf("\033[%d;%dH*", food->y + 2, food->x + 2);
}

void update_snake(Snake *snake) {
    for (int i = snake->length - 1; i > 0; i--) {
        snake->body[i] = snake->body[i - 1];
    }
    snake->body[0].x += snake->direction.x;
    snake->body[0].y += snake->direction.y;
}

int check_collision(Snake *snake) {
    Point head = snake->body[0];
    if (head.x < 0 || head.x >= WIDTH || head.y < 0 || head.y >= HEIGHT) return 1;
    for (int i = 1; i < snake->length; i++) {
        if (head.x == snake->body[i].x && head.y == snake->body[i].y) return 1;
    }
    return 0;
}

void place_food(Point *food, Snake *snake) {
    int valid;
    do {
        valid = 1;
        food->x = rand() % WIDTH;
        food->y = rand() % HEIGHT;
        for (int i = 0; i < snake->length; i++) {
            if (food->x == snake->body[i].x && food->y == snake->body[i].y) {
                valid = 0;
                break;
            }
        }
    } while (!valid);
}

void game_over(Snake *snake) {
    printf("\033[%d;%dHGame Over!\n", HEIGHT / 2 + 2, WIDTH / 2 - 4);
    printf("\033[%d;%dHYou scored %d!\n", HEIGHT / 2 + 3, WIDTH / 2 - 4, snake->length-1);
    move_cursor();
    return;
}

int main() {
    Snake snake;
    Point food;
    snake.length = 1;
    snake.body[0].x = WIDTH / 2;
    snake.body[0].y = HEIGHT / 2;
    snake.direction.x = 1;
    snake.direction.y = 0;

    place_food(&food, &snake);

    clear_screen();
    draw_border();
    draw_snake(&snake);
    draw_food(&food);

    while (1) {
        char input[3]; // Buffer to store escape sequences
        int n = keys(0, input, 3); // Read up to 3 characters
        if (n > 0) {
            if (n == 3 && input[0] == '\033' && input[1] == '[') {
                switch (input[2]) {
                    case 'A': if (snake.direction.y == 0) { snake.direction.x = 0; snake.direction.y = -1; } break; // Up
                    case 'B': if (snake.direction.y == 0) { snake.direction.x = 0; snake.direction.y = 1; } break;  // Down
                    case 'C': if (snake.direction.x == 0) { snake.direction.x = 1; snake.direction.y = 0; } break; // Right
                    case 'D': if (snake.direction.x == 0) { snake.direction.x = -1; snake.direction.y = 0; } break; // Left
                }
            }
        }
        if (check_collision(&snake)){
            game_over(&snake);
            return 0;
        }
        if (snake.body[0].x == food.x && snake.body[0].y == food.y) {
            if (snake.length < SNAKE_MAX_LENGTH) snake.length++;
            place_food(&food, &snake);
        }
        update_snake(&snake);
        clear_screen();
        draw_border();
        draw_snake(&snake);
        draw_food(&food);
        move_cursor();
        sleep(3);
    }
    return 0;
}

