#pragma once

#include "timer.h"
#include "scene.h"
#include "scene_manager.h"
#include <iostream>
#include <graphics.h>
using namespace std;

extern POINT screen_size;
extern SceneManager Scene_Manager;
extern void TextSet(int);
extern IMAGE background_menu;

struct MenuScene : public Scene {
public:
	MenuScene() {
		
	};
	~MenuScene() = default;

	void on_enter() {
		cout << "进入主菜单" << endl;
		timer.SetTimer((clock_t)100, [&]() {
			}, false);
	}
	void on_update(clock_t delta) {
		timer.OnUpdate(delta);
		count += delta/200.0;
		
		text_size = cos(count);
	}
	void on_input(const ExMessage& msg) {
		switch (msg.message) {
		case WM_KEYDOWN:
			if(msg.vkcode == VK_RETURN)
				Scene_Manager.SwitchTo(SceneManager::SceneType::Game);
		}
	}
	void on_draw() {
		putimage(0, 0, &background_menu);
		settextstyle(96 + 4*text_size, 0, _T("锐字太空奇遇像素简-闪"));
		outtextxy(screen_size.x/2 -400 - 16 * text_size, screen_size.y/2 + 320, _T("按下回车键开始游戏"));
	}
	void on_exit() {
		count = 0;
		cout << "主菜单已退出" << endl;
	}

private:
	float count = 0;
	Timer timer;
	POINT click_pos = { 0,0 };
	bool is_exit = false;
	float text_size;
};