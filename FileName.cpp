#pragma once
#define _CRT_SECURE_NO_DEPRECATE
#define PI 3.1415926535
#include "camera.h"
#include "functionaldraw.h"
#include "game_scene.h"
#include "menu_scene.h"
#include "platform.h"
#include "player.h"
#include "playerA.h"
#include "scene.h"
#include "scene_manager.h"
using namespace std;

Atlas Player_A_l;//Íæ¼ÒÍ¼¼¯
Atlas Player_B_l;
Atlas Player_C_l;
Atlas Player_D_l;
Atlas Player_E_l;
Atlas Player_A_r;
Atlas Player_B_r;
Atlas Player_C_r;
Atlas Player_D_r;
Atlas Player_E_r;
Atlas flybot_l;
Atlas flybot_r;
IMAGE flybot_attacked_l;
IMAGE flybot_attacked_r;
IMAGE background;
IMAGE background_top;
IMAGE background_bottom;
IMAGE black;
IMAGE background_menu;
IMAGE grass;
IMAGE grass_left;
IMAGE grass_right;
IMAGE grass_alone;
IMAGE dirt_left;
IMAGE dirt;
IMAGE dirt_right;
IMAGE dirt_alone;
IMAGE grass_top_left;
IMAGE grass_top_right;
IMAGE grass_top_alone;
IMAGE grass_top;
IMAGE player_life_full;
IMAGE player_life_half;
IMAGE player_life_blank;
Atlas bullet_A;
IMAGE gun;
IMAGE gun_flipepped;
Atlas num;
Atlas waterfall_top;
Atlas waterfall_mid;
Atlas waterfall_bottom;
Atlas water_top;
IMAGE water;

void LoadResource() {
    Player_A_l.LoadFromFile(_T("resource/img/character/playerA_%d.png"), 2);
    Player_B_l.LoadFromFile(_T("resource/img/character/playerB_%d.png"), 2);
    Player_C_l.LoadFromFile(_T("resource/img/character/playerC_%d.png"), 2);
    Player_D_l.LoadFromFile(_T("resource/img/character/playerD_%d.png"), 2);
    Player_E_l.LoadFromFile(_T("resource/img/character/playerE_%d.png"), 2);
    FlipAtlas(Player_A_r, Player_A_l);
    FlipAtlas(Player_B_r, Player_B_l);
    FlipAtlas(Player_C_r, Player_C_l);
    FlipAtlas(Player_D_r, Player_D_l);
    FlipAtlas(Player_E_r, Player_E_l);
    flybot_l.LoadFromFile(_T("resource/img/character/flybot_%d.png"), 4);
    loadimage(&flybot_attacked_l,_T("resource/img/character/flybot_4.png"));
    FlipAtlas(flybot_r, flybot_l);
    FlipImage(&flybot_attacked_r, &flybot_attacked_l);
    loadimage(&background, _T("resource/img/background_white.png"));
    loadimage(&background_top, _T("resource/img/background_white_top.png"));
    loadimage(&background_bottom, _T("resource/img/background_white_bottom.png"));
    loadimage(&black, _T("resource/img/black.png"));
    loadimage(&background_menu, _T("resource/img/SampleA.png"));
    loadimage(&grass_alone, _T("resource/img/block/grass_0.png"));
    loadimage(&grass_left, _T("resource/img/block/grass_1.png"));
    loadimage(&grass, _T("resource/img/block/grass_2.png"));
    loadimage(&grass_right, _T("resource/img/block/grass_3.png"));
    loadimage(&dirt_left, _T("resource/img/block/dirt_0.png"));
    loadimage(&dirt, _T("resource/img/block/dirt_1.png"));
    loadimage(&dirt_right, _T("resource/img/block/dirt_2.png"));
    loadimage(&dirt_alone, _T("resource/img/block/dirt_3.png"));
    loadimage(&grass_top_alone, _T("resource/img/block/grass_top_0.png"));
    loadimage(&grass_top_left, _T("resource/img/block/grass_top_1.png"));
    loadimage(&grass_top_right, _T("resource/img/block/grass_top_2.png"));
    loadimage(&grass_top, _T("resource/img/block/grass_top_3.png"));
    loadimage(&player_life_full, _T("resource/img/block/life_0.png"));
    loadimage(&player_life_half, _T("resource/img/block/life_1.png"));
    loadimage(&player_life_blank, _T("resource/img/block/life_2.png"));
    bullet_A.LoadFromFile(_T("resource/img/block/bullet_A_%d.png"), 5);
    loadimage(&gun, _T("resource/img/block/gun.png"));
    FlipImage(&gun_flipepped, &gun);
    num.LoadFromFile(_T("resource/img/block/num_%d.png"), 10);
    waterfall_top.LoadFromFile(_T("resource / img / block / waterfall_top_ % d.png"), 2);
    waterfall_mid.LoadFromFile(_T("resource / img / block / waterfall_mid_ % d.png"), 2);
    waterfall_bottom.LoadFromFile(_T("resource / img / block / waterfall_bottom_ % d.png"), 2);
    water_top.LoadFromFile(_T("resource/img/block/water_top_%d.png"), 2);
    loadimage(&water, _T("resource/img/block/water.png"));

}

POINT screen_size = { GetSystemMetrics(SM_CXFULLSCREEN) , GetSystemMetrics(SM_CYFULLSCREEN) };
POINT point_pos = { 0,0 };
Scene* Menu_Scene = NULL;
Scene* Game_Scene = NULL;
Player* Player_A = NULL;
vector<Platform> platform_list;
Camera camera;
Vector2 click_pos;

SceneManager Scene_Manager;

void TextSet(int state) {
    switch (state)
    {
    case 0:
        settextstyle(32, 0, _T("Èñ×ÖÌ«¿ÕÆæÓöÏñËØ¼ò-ÉÁ"));
        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
    case 1:    
    settextstyle(64, 0, _T("Èñ×ÖÌ«¿ÕÆæÓöÏñËØ¼ò-ÉÁ"));
    setbkmode(TRANSPARENT);
    settextcolor(WHITE);
    case 2:
        settextstyle(128, 0, _T("Èñ×ÖÌ«¿ÕÆæÓöÏñËØ¼ò-ÉÁ"));
        setbkmode(TRANSPARENT);
        settextcolor(WHITE);
        break;
    }
}

int main() {
    AddFontResourceEx(_T("resource/pixel_character.ttf"), FR_PRIVATE, NULL);//¼ÓÔØ×ÖÌå

    Menu_Scene = new MenuScene();
    Game_Scene = new GameScene();
    Player_A = new PlayerA();

    Scene_Manager.SetCurrentScene(Menu_Scene);
	ExMessage msg;
    int real_FPS = 0, count_FPS = 0, FPS = 1000;
    bool running = true;
    clock_t time_count = 0, delta = 0, sleep_time = 0;
    TCHAR str[256], str1[256];
    _stprintf_s(str, _T("FPS:%d"), real_FPS);
    _stprintf_s(str1, _T("Delta:%d"), delta + sleep_time);

    cout << screen_size.x << ' ' << screen_size.y<<endl;


   
    
    HWND hwnd = initgraph(screen_size.x, screen_size.y, EW_SHOWCONSOLE);
    HCURSOR hcur = (HCURSOR)LoadImage(NULL, _T("resource/cur/nomal.cur"), IMAGE_CURSOR, 0, 0, LR_LOADFROMFILE);
    SetClassLongPtr(hwnd, GCLP_HCURSOR, (long)hcur);
    LoadResource();

	BeginBatchDraw();
    TextSet(0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	while (running) {
        clock_t frame_start_time = clock();

		while (peekmessage(&msg)) {
            Scene_Manager.OnInput(msg);
            switch (msg.message) {
            case WM_MOUSEMOVE:
                point_pos.x = msg.x ;
                point_pos.y = msg.y; break;
            case WM_KEYDOWN:
                switch (msg.vkcode) {
                case 0x50://p¼ü£¬ÓÃÓÚÍË³ö
                    running = false;break;
                }break;
            }
            
		}
        click_pos.x = point_pos.x + camera.GetPos().x;
        click_pos.y = point_pos.y + camera.GetPos().y;
        _stprintf_s(str1, _T("Delta:%d"), delta + sleep_time);
        Scene_Manager.OnUpdate(delta + sleep_time);

        Scene_Manager.OnDraw();
        settextstyle(50,0,BLACK);
        outtextxy(10, 100, str);
        outtextxy(10, 200, str1);
        FlushBatchDraw();
        cleardevice();
        clock_t frame_end_time = clock();
		delta = frame_end_time - frame_start_time;
        time_count += delta;
        if (time_count > CLOCKS_PER_SEC) {
            time_count = 0;
            real_FPS = count_FPS + 1;
            count_FPS = 0;
            _stprintf_s(str, _T("FPS:%d"), real_FPS);
        }
        else count_FPS++;
        if (delta < 1000 / FPS) {
            sleep_time = 1000 / FPS - delta;
            Sleep(sleep_time);
            time_count += sleep_time;
        }
        else sleep_time = 0;
	}
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
}

