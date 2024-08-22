#pragma once

#include "scene.h"
#include <iostream>
#include <graphics.h>
using namespace std;

#pragma comment(lib, "MSIMG32.LIB")//透明渲染通道，让透明图片正常渲染，我不会（头秃）
inline void putimage_alpha(int x, int y, IMAGE* img) {
	int w = img->getwidth();
	int h = img->getheight();
	AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
}


class MenuScene : public Scene {
public:
	MenuScene() {
		
	};
	~MenuScene() = default;

	inline void putimage_alpha(int x, int y, IMAGE* img) {
		int w = img->getwidth();
		int h = img->getheight();
		AlphaBlend(GetImageHDC(NULL), x, y, w, h, GetImageHDC(img), 0, 0, w, h, { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA });
	}


	void on_enter() {
		cout << "进入主菜单" << endl;
	}
	void on_update(bool* running) {
		cout << "主菜单正在运行..." << endl;
		if (is_exit) {
			on_exit();
			*running = false;
		}
	}
	void on_draw() {
		outtextxy(10, 10, _T("主菜单绘制内容"));
		loadimage(&img, _T("img/Start Menu.png"));
		loadimage(&background, _T("img/start_background.png"));
		putimage_alpha(0, 0, &img);
		putimage_alpha(0, -1024, &background);
	}
	void on_input(const ExMessage& msg) {
		switch (msg.message) {
		case WM_LBUTTONDOWN:
			click_pos.x = msg.x; 
			click_pos.y = msg.y; 
			break;
		}
		if (click_pos.x > 95 && click_pos.x < 272 && click_pos.y >863 && click_pos.y < 928)
			is_exit = true;
	}
	void on_exit() {
		cout << "主菜单已退出" << endl;
	}

private:
	IMAGE img, background;
	POINT click_pos;
	bool is_exit = false;
};