#pragma once
#pragma once
#include "camera.h";
#include "functionaldraw.h"
#include "player.h"

extern Camera camera;


struct Water {
public:
	//设置平台
	//length width 平台长度与宽度
	//position 平台左上角坐标
	//areaposx 区块标号
	//flag 是否绘制左右两边
	Water(int lengh, int width, POINT position, int areaposx = 0, bool flag = false) :lengh(lengh), width(width) {
		pos.x = position.x * 64;
		pos.y = position.y * 64;
		areax = areaposx;
		drawpos.x = pos.x + areax * areawidth;
		drawpos.y = pos.y;
		draw_side = false;
	};
	~Water() {};
	void OnDraw() {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < lengh; j++) {
				if (i == 0) {
					if (j == 0 && draw_side) putimage_alpha(drawpos.x - camera.GetPos().x + j * 64, pos.y - camera.GetPos().y + i * 64, &grass_top_left);
					else if (j == lengh - 1 && draw_side)putimage_alpha(drawpos.x - camera.GetPos().x + j * 64, pos.y - camera.GetPos().y + i * 64, &grass_top_right);
					else putimage_alpha(drawpos.x - camera.GetPos().x + j * 64, pos.y - camera.GetPos().y + i * 64, &grass_top);
				}
				else {
					if (j == 0 && draw_side) putimage_alpha(drawpos.x - camera.GetPos().x + j * 64, pos.y - camera.GetPos().y + i * 64, &dirt_left);
					else if (j == lengh - 1 && draw_side)putimage_alpha(drawpos.x - camera.GetPos().x + j * 64, pos.y - camera.GetPos().y + i * 64, &dirt_right);
					else putimage_alpha(drawpos.x - camera.GetPos().x + j * 64, pos.y - camera.GetPos().y + i * 64, &dirt);
				}
			}
		}
	}
	void ShowFrame(bool flag) {
		show_frame = flag;
	}
	Vector2 GetPos() {
		return drawpos;
	}
	int GetLengh() {
		return lengh * 64;
	}
private:
	int areax = 0, areawidth = 2048;
	int lengh = 0;
	int width = 0;
	Vector2 pos = { 0,0 }, drawpos = { 0,0 };
	bool show_frame = false;
	bool draw_side = false;
};