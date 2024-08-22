#define _CRT_SECURE_NO_DEPRECATE

#include <graphics.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <string>
#include <tchar.h>
using namespace std;
POINT point_pos = { 0,0 };

class Animation{};

class Player{
public:
	Player() {

	}
	~Player() {

	}
    void ProcessEvent(ExMessage msg, bool* running) {
        switch (msg.message) {
        case WM_LBUTTONDOWN:
            is_shooting = true; break;

        case WM_KEYDOWN:
            switch (msg.vkcode) {
            case VK_LEFT:
                move_l = true;
                break;
            case VK_RIGHT:
                move_r = true;
                break;
            case VK_SPACE://¿Õ¸ñ¼ü£¬ÌøÔ¾
                if (!is_fall)
                    is_jump = true;
                break;
            case 0x46://f¼ü£¬³å´Ì
                is_dashing = true;
                break;
            case 0x41://a¼ü£¬Ïò×óÒÆ¶¯
                move_l = true;
                break;
            case 0x44://d¼ü£¬ÏòÓÒÒÆ¶¯
                move_r = true;
                break;
            }break;


        case WM_KEYUP:
            switch (msg.vkcode) {
            case VK_LEFT:
                move_l = false;
                break;
            case VK_RIGHT:
                move_r = false;
                break;
            case 0x41://a¼ü£¬Ïò×óÒÆ¶¯
                move_l = false;
                break;
            case 0x44://d¼ü£¬ÏòÓÒÒÆ¶¯
                move_r = false;
                break;
            }break;

        }
    }
    void Move() {
        player_pos.x += x_speed;
        player_pos.y += y_speed;
    }
    void Speed() {
        if (move_l && x_speed > -speed_limit)
            x_speed -= speed_up;
        else if (move_l && x_speed < -speed_limit - speed_down)
            x_speed += speed_down;
        else if (move_l && x_speed < -speed_limit)
            x_speed = -speed_limit;
        else if (!move_l && x_speed < 0)
            x_speed += speed_down;

        if (move_r && x_speed < speed_limit)
            x_speed += speed_up;
        else if (move_r && x_speed > speed_limit + speed_down)
            x_speed -= speed_down;
        else if(move_r && x_speed > speed_limit)
            x_speed = speed_limit;
        else if (!move_r && x_speed > 0)
            x_speed -= speed_down;

        if (is_jump&&!is_fall) {
            is_jump = false;
            is_fall = true;
            y_speed = -jump_speed;
        }

        if (is_fall) {
            y_speed += fall_speed_up;
        }

        if (player_pos.y > 512) {
            player_pos.y = 512;
            is_fall = false;
            y_speed = 0;
        }

        if (is_dashing) {
            if (x_speed < 0) x_speed = -dash_speed;            
            if (x_speed > 0) x_speed = dash_speed;
            is_dashing = false;

        }
    }
    void Frame(DWORD screen_wide, DWORD screen_hight) {
        setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 4);
        rectangle((screen_wide - player_wide) / 2, (screen_hight - player_hight) / 2, (screen_wide + player_wide) / 2, (screen_hight + player_hight) / 2);
        //solidcircle(screen_wide/2, screen_hight/2, 100);
    }
    POINT player_pos = { GetSystemMetrics(SM_CXFULLSCREEN) / 2,GetSystemMetrics(SM_CYFULLSCREEN) / 2 };
private:
    bool is_shooting = false, is_dashing = false, is_fall = false, is_jump = false, move_r = false, move_l = false, double_jump = false;
	int speed_limit = 15, jump_speed = 30, fall_speed_up = 1, player_wide = 40, player_hight = 80, x_speed = 0, y_speed = 0, speed_up =  1, speed_down = 1, dash_speed = 40;
    
};

class enemy{};

class bullet{};

class Object{};

int main() {
	

	ExMessage msg;
    IMAGE background;
    DWORD real_FPS = 0, FPS = 240, time_count = GetTickCount(), index_count = 0, delta = 0, screen_wide = GetSystemMetrics(SM_CXFULLSCREEN), screen_hight = GetSystemMetrics(SM_CYFULLSCREEN);
    int load_num_x = 0, load_num_y = 0;
    bool running = true, frame_display = false;
    Player P;
    cout << screen_wide << ' ' << screen_hight;

    initgraph(screen_wide, screen_hight, EW_SHOWCONSOLE);

	BeginBatchDraw();
    loadimage(&background,_T("img/testbackground.png"));

	while (running) {
		DWORD frame_start_time = GetTickCount();

		while (peekmessage(&msg)) {
            P.ProcessEvent(msg, &running);
            switch (msg.message) {
            case WM_MOUSEMOVE:
                point_pos.x = msg.x;
                point_pos.y = msg.y; break;
            case WM_KEYDOWN:
                switch (msg.vkcode) {
                case 0x50://p¼ü£¬ÓÃÓÚÍË³ö
                    running = false;
                    break;
                case VK_OEM_3://~¼ü£¬ÏÔÊ¾Åö×²Ïä
                    if (frame_display) frame_display = false;
                    else frame_display = true;
                }break;
            }
		}
            P.Speed();
            P.Move();
            putimage(screen_wide / 2 - P.player_pos.x, screen_hight / 2 - P.player_pos.y, &background);
            if (frame_display) {
                P.Frame(screen_wide, screen_hight);
            }
            FlushBatchDraw();
            cleardevice();
        

		DWORD frame_end_time = GetTickCount();
		delta = frame_end_time - frame_start_time;
		
		if (delta < 1000 / FPS)
			Sleep(1000 / FPS - delta);
	}
}