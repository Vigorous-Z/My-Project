#pragma once

#include "scene.h"
#include <iostream>
#include <graphics.h>
using namespace std;

#pragma comment(lib, "MSIMG32.LIB")//͸����Ⱦͨ������͸��ͼƬ������Ⱦ���Ҳ��ᣨͷͺ��



class GameScene : public Scene {
public:
	GameScene() {

	};
	~GameScene() = default;

	inline void putimage_alpha(int x, int y, IMAGE* img) {
		int w = img->getwidth();
		int h = img->getheight();
		AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	}

	void on_enter() {
		cout << "�������˵�" << endl;
	}
	void on_update(bool* running) {
		cout << "���˵���������..." << endl;
		if (is_exit) {
			on_exit();
			*running = false;
		}
	}
	void on_draw() {
		outtextxy(10, 10, _T("���˵���������"));
	}
	void on_input(const ExMessage& msg) {
		switch (msg.message) {
		case WM_LBUTTONDOWN:
			click_pos.x = msg.x;
			click_pos.y = msg.y;
			break;
		case WM_KEYDOWN:
			switch (msg.vkcode) {
			case VK_ESCAPE:
				is_exit = true;
				break;
			}
		
		}
	}
	void on_exit() {
		cout << "���˵����˳�" << endl;
	}

private:
	IMAGE img;
	POINT click_pos;
	bool is_exit = false;
};