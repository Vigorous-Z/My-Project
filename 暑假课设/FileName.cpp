#define _CRT_SECURE_NO_DEPRECATE

#include <stdio.h>
#include <graphics.h>
#include <iostream>
#include <math.h>
#include <string.h>
#include <string>
#include <tchar.h>
#include <vector>
using namespace std;
POINT point_pos = { 0,0 };


#pragma comment(lib, "MSIMG32.LIB")//透明渲染通道，让透明图片正常渲染，我不会（头秃）
inline void putimage_alpha(int x, int y, IMAGE* img) {
    int w = img->getwidth();
    int h = img->getheight();
    AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}

class Animation{
public:
    Animation(LPCTSTR path, int num, int interval) {
        interval_ms = interval;
        TCHAR path_file[256];
        for (int i = 0; i < num; i++) {
            _stprintf_s(path_file, path, i);

            IMAGE* frame = new IMAGE();
            loadimage(frame, path_file);
            frame_list.push_back(frame);
        }
    }
    ~Animation() {
        for (size_t i = 0; i < frame_list.size(); i++) {
            delete frame_list[i];
        }
    }

    void Play(int x, int y, int delta) {
        timer += delta;
        if (timer >= interval_ms) {
            idx_frame = (idx_frame + timer / interval_ms) % frame_list.size();
            timer = 0;
        }
        putimage_alpha(x, y, frame_list[idx_frame]);
    }

    void PlayerPlay(int x, int y, int delta, bool is_move, bool is_fall) {
        timer += delta;
        if (timer >= interval_ms) {
            idx_frame = (idx_frame + timer / interval_ms) % frame_list.size();
            timer = 0;
        }
        if (!is_move || is_fall) putimage_alpha(x, y, frame_list[!is_fall]);
        else putimage_alpha(x, y, frame_list[idx_frame]);
    }

private:
    vector<IMAGE*> frame_list;
    int interval_ms = 0;
    int timer = 0;
    int idx_frame = 0;
};

class Player{
public:
	Player() {

	}
	~Player() {

	}
    void ProcessEvent(ExMessage msg) {
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
            case 0x41://a键，向左移动
                move_l = true;
                break;
            case 0x44://d键，向右移动
                move_r = true;
                break;
            case VK_SPACE://空格键，跳跃
                if ((double_jump && jump_count < 2) || !is_fall)
                    is_jump = true;
                break;
            case 0x46://f键，冲刺
                is_dashing = true;
                break;
            case 0x52://r键，切换二段跳
                if (double_jump) double_jump = false;
                else double_jump = true;
            }break;


        case WM_KEYUP:
            switch (msg.vkcode) {
            case VK_LEFT:
                move_l = false;
                break;
            case VK_RIGHT:
                move_r = false;
                break;
            case 0x41://a键，向左移动
                move_l = false;
                break;
            case 0x44://d键，向右移动
                move_r = false;
                break;
            }break;

        }
    }
    void Move(clock_t delta) {
        player_pos.x += x_speed * delta;
        player_pos.y += y_speed * delta;
    }
    void Speed(clock_t delta) {
        if (move_l) {
            if (x_speed > -speed_limit) x_speed = max(x_speed - speed_up, -speed_limit);
            else if (x_speed < -speed_limit) x_speed = min(x_speed + over_speed_down, -speed_limit);
        }
        if (move_r) {
            if (x_speed < speed_limit) x_speed = min(x_speed + speed_up, speed_limit);
            else if (x_speed > speed_limit) x_speed = max(x_speed - over_speed_down, speed_limit);
        }
        if(!move_l&&!move_r) {
            if (x_speed < 0) x_speed = min(0, x_speed + speed_down);
            else if (x_speed > 0) x_speed = max(0, x_speed - speed_down);
        }//基本x轴速度计算

        if (is_fall) {
            y_speed += fall_speed_up * delta;
        }
        if (player_pos.y > 0) {
            player_pos.y = 0;
            is_fall = false;
            y_speed = 0;
            jump_count = 0;
        }//暂时平台检测，记得删

        dash_time += delta;
        if (is_dashing && dash_time >= dash_cooldown) {
            if (x_speed < 0) x_speed = -dash_speed;
            if (x_speed > 0) x_speed = dash_speed;
            is_dashing = false;
            y_speed = 0;
            dash_time = 0;
        }
        else is_dashing = false;//冲刺条件判断

        if (fabs(x_speed) > speed_limit && y_speed > 0) {
            y_speed = 0;
        }
    }
    void Jump() {
            is_jump = false;
            is_fall = true;
            y_speed = -jump_speed;
            jump_count++;
    }
    void Frame(DWORD screen_wide, DWORD screen_hight) {
        setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 3);
        if (double_jump) setlinecolor(BLUE);
        else setlinecolor(WHITE);
        rectangle((screen_wide - player_wide) / 2, (screen_hight - player_height) / 2, (screen_wide + player_wide) / 2, (screen_hight + player_height) / 2);
    }
    void Animation(LPCTSTR path, int num, int interval) {
        interval_ms = interval;
        TCHAR path_file[256];
        for (int i = 0; i < num; i++) {
            _stprintf_s(path_file, path, i);

            IMAGE* frame = new IMAGE();
            loadimage(frame, path_file);
            frame_list.push_back(frame);
        }
    }
    void PlayerPlay(int x, int y, int delta) {
        timer += delta;
        if (timer >= interval_ms) {
            idx_frame = (idx_frame + timer / interval_ms) % frame_list.size();
            timer = 0;
        }
        if (x_speed == 0 || is_fall) putimage_alpha(x, y, frame_list[!is_fall]);
        else putimage_alpha(x, y, frame_list[idx_frame]);
    }
    void DeleteAnimation() {
        for (size_t i = 0; i < frame_list.size(); i++) {
            delete frame_list[i];
        }
    }
    bool GetState() {
        return double_jump;
    }
    POINT GetPos() {
        return player_pos;
    }
    float GetXSpeed() {
        return x_speed;
    }
    int GetWide() {
        return player_wide;
    }
    int GetHeight() {
        return player_height;
    }
    bool IsFall() {
        return is_fall;
    }

    POINT player_pos = { 0, 0};
    bool is_jump = false;
private:
    double move_rad = 45, now_rad = 0;
    clock_t dash_cooldown = 1000, dash_time = 0;
    bool is_shooting = false, is_dashing = false, is_fall = false, move_r = false, move_l = false, double_jump = false;
	int  player_wide = 80, player_height = 96, jump_count = 0;
    float  speed_limit = 1,x_speed = 0, y_speed = 0, speed_up = 0.1, fall_speed_up = 0.005, jump_speed = 2, speed_down = 0.05, over_speed_down = 0.15, dash_speed = 6;

    vector<IMAGE*> frame_list;
    int interval_ms = 0;
    int timer = 0;
    int idx_frame = 0;
};

class Enemy{
public:
    Enemy() {}
    ~Enemy(){}
private:

};

class Jumpcloud {//跳跃特效对象
public:
    Jumpcloud(){}
    ~Jumpcloud(){}
    void Creat(POINT pos) {

    }
    void Frame(POINT pos) {

    }
private:

};

class Floattext {//悬浮文字对象
public:
    Floattext(POINT pos, POINT player_pos, bool state) {
        textpos.x = pos.x/2 + player_pos.x;
        textpos.y = pos.y/2 + player_pos.y;
        jump_state = state;
    }
    ~Floattext(){}

    void Draw(POINT player_pos) {
        if(jump_state)
            outtextxy(textpos.x - player_pos.x, textpos.y - player_pos.y, _T("二段跳启动！"));
        else outtextxy(textpos.x - player_pos.x, textpos.y - player_pos.y, _T("二段跳关闭！"));
    }

    void Timer(){
        
    }
private:
    POINT textpos = { 0, 0 };
    bool jump_state = false;
    clock_t timer = 0;
};

class Bullet{};

class Object{};

void DrawPlayerScore(DWORD score) {
    static TCHAR text[64];
    _stprintf_s(text, _T("Delta: %lu"), score);

    settextstyle(32, 0, _T("Consolas"));
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    outtextxy(10, 10, text);
}
void DrawPlayerPos(POINT pos) {
    static TCHAR text1[64];
    static TCHAR text2[64];
    _stprintf_s(text1, _T("Pos_x: %d"), pos.x/64);
    _stprintf_s(text2, _T("Pos_y: %d"), -pos.y/64);

    outtextxy(10, 30, text1);
    outtextxy(10, 50, text2);
}
void DrawPlayerXSpeed(float speed) {
    static TCHAR text[64];
    _stprintf_s(text, _T("X_Speed: %f"),speed);

    outtextxy(10, 70, text);
}
void DrawFPS(DWORD fps) {
    static TCHAR text[64];
    _stprintf_s(text, _T("FPS: %lu"), fps);

    outtextxy(10, 90, text);
}


Animation player_anim_l(_T("img/playerleft_%d.png"), 2, 60);
Animation player_anim_r(_T("img/playerright_%d.png"), 2, 60);
void PlayerDraw(int delta, float x_speed, POINT screen_size, int wide, int height, bool is_fall) {
    static bool facing_left = false;
    if (x_speed < 0) facing_left = true;
    else if (x_speed > 0) facing_left = false;
    if (facing_left)
        player_anim_l.PlayerPlay((screen_size.x-wide) / 2, (screen_size.y-height) / 2, delta, x_speed != 0, is_fall);
    else player_anim_r.PlayerPlay((screen_size.x-wide) / 2, (screen_size.y-height) / 2, delta, x_speed != 0, is_fall);
}


int main() {
	
    TCHAR path_file[256];
    vector<Floattext*> text_list;
	ExMessage msg;
    IMAGE background;
    DWORD real_FPS = 0, count_FPS = 0, FPS = 120,  index_count = 0;
    POINT screen_size = { GetSystemMetrics(SM_CXFULLSCREEN) , GetSystemMetrics(SM_CYFULLSCREEN) };
    int load_num_x = 0, load_num_y = 0;
    bool running = true, frame_display = false, switch_test = false, jump_state = false;
    clock_t time_count = 0, delta = 0, sleep_time = 0;
    Player P;
    cout << screen_size.x << ' ' << screen_size.y;

    initgraph(screen_size.x, screen_size.y, EW_SHOWCONSOLE);

	BeginBatchDraw();
    loadimage(&background,_T("img/testbackground.png"));
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	while (running) {
        clock_t frame_start_time = clock();

        load_num_x = P.player_pos.x / background.getwidth() - (P.player_pos.x < 0);
        load_num_y = P.player_pos.y / background.getheight() - (P.player_pos.y < 0);

		while (peekmessage(&msg)) {
            P.ProcessEvent(msg);
            switch (msg.message) {
            case WM_MOUSEMOVE:
                point_pos.x = msg.x;
                point_pos.y = msg.y; break;
            case WM_KEYDOWN:
                switch (msg.vkcode) {
                case 0x50://p键，用于退出
                    running = false;
                    break;
                case VK_OEM_3://~键，显示碰撞箱
                    if (frame_display) frame_display = false;
                    else frame_display = true;
                }break;
            }
		}
        if (jump_state != P.GetState()) {
            jump_state = P.GetState();
            text_list.push_back(new Floattext(screen_size, P.GetPos(), P.GetState()));
        }
        P.Speed(delta + sleep_time);
        P.Move(delta + sleep_time);
        if (P.is_jump) {
            P.Jump();
        }
        for (int i = -1; i < 2; i++) {
            for (int j = -1; j < 2; j++)
                putimage(screen_size.x / 2 - P.player_pos.x + (load_num_x + i) * background.getwidth(), screen_size.y / 2 - P.player_pos.y + (load_num_y + j) * background.getheight(), &background);
        }
            
        
        for (Floattext* text : text_list) {
            text->Draw(P.player_pos);
        }
        
        if (frame_display) {
            P.Frame(screen_size.x, screen_size.y);
        }
        PlayerDraw(delta + sleep_time, P.GetXSpeed(), screen_size, P.GetWide(), P.GetHeight(), P.IsFall());
        if (frame_display) {
            DrawPlayerScore(delta);
            DrawPlayerPos(P.GetPos());
            DrawPlayerXSpeed(P.GetXSpeed());
            DrawFPS(real_FPS);
        }
        FlushBatchDraw();
        cleardevice();
        clock_t frame_end_time = clock();
		delta = frame_end_time - frame_start_time;
        time_count += delta;
        if (time_count > CLOCKS_PER_SEC) {
            time_count = 0;
            real_FPS = count_FPS + 1;
            count_FPS = 0;
        }
        else count_FPS++;
        if (delta < 1000 / FPS) {
            sleep_time = 1000 / FPS - delta;
            Sleep(sleep_time);
            time_count += sleep_time;
        }
	}
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
}