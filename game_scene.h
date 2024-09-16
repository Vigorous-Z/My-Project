#pragma once

#include "player.h"
#include "playerA.h"
#include "scene.h"
#include "scene_manager.h"
#include "platform.h"
#include "camera.h"
#include "flybot.h"
#include "area.h"
#include <iostream>
#include <time.h>
#include <graphics.h>
using namespace std;

extern SceneManager Scene_Manager;
extern IMAGE background, background_top, background_bottom;
extern Player* Player_A;
extern vector<Platform> platform_list;
extern Camera camera;

struct GameScene : public Scene {
public:
	GameScene() {
	};
	~GameScene() = default;

	void on_enter() {
		if(!area_exist){
			for (int i = -1; i < 2; i++) {
				Area temp(i);
				area_list.push_back(temp);
				cout << "area添加成功" << endl;
			}
			area_exist = true;
		}
	}
	void on_update(clock_t delta) {
		Player_A->OnUpdate(delta);
		areax = (Player_A->GetPos().x >= 0)? Player_A->GetPos().x / 2048 : Player_A->GetPos().x / 2048 - 1;
		if (now_area_pos < areax) {
			swap(area_list[0], area_list[1]);
			swap(area_list[1], area_list[2]);
			area_list.pop_back();
			now_area_pos = areax;
			Area temp(areax + 1);
			area_list.push_back(temp);
			cout << "area添加成功" << endl;
		}
		else if (now_area_pos > areax) {
			area_list.pop_back();
			now_area_pos = areax;
			Area temp(areax - 1);
			area_list.push_back(temp);
			swap(area_list[1], area_list[2]);
			swap(area_list[0], area_list[1]);
			cout << "area添加成功" << endl;
		}
		for (Area &a : area_list) {
			a.OnUpdate(delta);
			a.ShowFrame(show_frame);
		}
		backgorund_pos.x = Player_A->GetPos().x/2 / background.getwidth();
		backgorund_pos.y = Player_A->GetPos().y/2 / background.getheight();

		Player_A->ShowFrame(show_frame);

	}
	void on_draw() {
		for (int i = -1; i < 2; i++) {
			for (int j = -1; j < 2; j++) {
				if (backgorund_pos.y + i > 0)
					putimage((j + backgorund_pos.x) * background.getwidth() - camera.GetPos().x/2, -background.getheight() / 2 + (i + backgorund_pos.y) * background.getheight() - camera.GetPos().y/2, &background_bottom);
				else if (backgorund_pos.y + i < 0)
					putimage((j + backgorund_pos.x) * background.getwidth() - camera.GetPos().x/2, -background.getheight() / 2 + (i + backgorund_pos.y) * background.getheight() - camera.GetPos().y/2, &background_top);
				else putimage((j + backgorund_pos.x) * background.getwidth() - camera.GetPos().x/2, -background.getheight() / 2 + (i + backgorund_pos.y) * background.getheight() - camera.GetPos().y/2, &background);
			}
		}
		for (Area &a : area_list) {
			a.OnDraw();
		}
		Player_A->OnDraw();
	}
	void on_input(const ExMessage& msg) {
		Player_A->OnInput(msg);
		switch (msg.message) {
		case WM_KEYDOWN:
			switch (msg.vkcode)
			{
			case VK_ESCAPE:
				Scene_Manager.SwitchTo(SceneManager::SceneType::Menu);
				break;
			case 0xC0:
				show_frame = !show_frame;
				break;
			}break;
		}
	}

	void on_exit() {
		platform_list.clear();
		cout << "局内游戏已退出" << endl;
	}

private:
	IMAGE img;
	POINT click_pos = { 0,0 }, backgorund_pos = { 0,0 };
	vector<Area> area_list;
	int now_area_pos = 0, areax = 0;
	bool is_exit = false, show_frame = false, area_exist = false;
};