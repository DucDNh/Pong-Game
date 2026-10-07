#include <raylib.h>
#include <iostream>

using namespace std;

Color green = {15, 199, 2, 255}, 
        yellow = {249, 255, 0, 255};

class Ball{
public : 
    float x, y; 
    int speed_x, speed_y;
    int radius;

    void Draw() {
        DrawCircle(x, y, radius, yellow);
    }

    void Update(int& cpu_s, int& player_s) {
        x += speed_x;
        y += speed_y;

        if (y + radius >= GetScreenHeight() || y - radius <= 0) {
            speed_y *= -1;
        } 
        if (x + radius >= GetScreenWidth() || x - radius <= 0) {
            speed_x *= -1;
            if (x + radius >= GetScreenWidth()) {
                cpu_s++;
            } else {
                player_s++;
            }
            Reset();
        }
    }

    void Reset() {
        x = GetScreenWidth() / 2;
        y = GetScreenHeight() / 2;

        int speed_choices[2] = {-1, 1};
        speed_x *= speed_choices[GetRandomValue(0, 1)];
        speed_y *= speed_choices[GetRandomValue(0, 1)];
    }
};

class Paddle {
public :
    float x, y;
    float width, height;
    int speed;

    void Draw() {
        DrawRectangle(x, y, width, height, WHITE);
    }

    void Update() {
        if (IsKeyDown(KEY_UP) && y - speed >= 0) {
            y -= speed;
        } 
        else if (IsKeyDown(KEY_DOWN) && y + speed + height <= GetScreenHeight()) {
            y += speed;
        }
    }
};

class CpuPaddle : public Paddle {
    public : 

    void Update(int ball_y) {
        if (y + height / 2 <= ball_y && y + height + speed <= GetScreenHeight()) {
            y += speed;
        }
        else if (y + height / 2 > ball_y && y - speed >= 0) {
            y -= speed;
        }
    }
};

int main() {
    const int screen_width = 1280;
    const int screen_height = 800;

    Ball ball;
    ball.radius = 20;
    ball.x = screen_width/2;
    ball.y = screen_height/2;
    ball.speed_x = 7;
    ball.speed_y = 7;

    Paddle player;
    player.width = 25;
    player.height = 120;
    player.x = screen_width - player.width - 10;
    player.y = screen_height/2 - player.height/2;
    player.speed = 6;

    CpuPaddle cpu;
    cpu.height = 120;
    cpu.width = 25;
    cpu.x = 10;
    cpu.y = screen_height / 2 - cpu.height / 2;
    cpu.speed = 6;

    int player_score = 0, 
        cpu_score = 0;

    InitWindow(screen_width, screen_height, "My Pong Game~");
    SetTargetFPS(60);

    while(WindowShouldClose() == false) {
        // Update
        ball.Update(cpu_score, player_score);
        player.Update();
        cpu.Update(ball.y);

        // Check for collision
        if (CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, 
            Rectangle{player.x, player.y, player.width, player.height})
            || CheckCollisionCircleRec(Vector2{ball.x, ball.y}, ball.radius, 
            Rectangle{cpu.x, cpu.y, cpu.width, cpu.height})) {
                ball.speed_x *= -1;
            }

        // Drawing
        BeginDrawing();
        ClearBackground(green);

        DrawLine(screen_width/2, 0, screen_width/2, screen_height, WHITE);

        ball.Draw();
        player.Draw();
        cpu.Draw();
        DrawText(TextFormat("%i", cpu_score), screen_width/4 -20, 20, 80, WHITE);
        DrawText(TextFormat("%i", player_score), 3* screen_width/4 -20, 20, 80, WHITE);

        EndDrawing();
    }


    CloseWindow();
    return 0;
}