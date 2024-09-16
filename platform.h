#pragma once
#include "camera.h";
#include "functionaldraw.h"
#include "player.h"

extern Camera camera;
extern IMAGE grass_alone;
extern IMAGE grass_left;
extern IMAGE grass;
extern IMAGE grass_right;
extern IMAGE dirt_left;
extern IMAGE dirt;
extern IMAGE dirt_right;
extern IMAGE dirt_alone;
extern IMAGE grass_top_left;
extern IMAGE grass_top_right;
extern IMAGE grass_top_alone;
extern IMAGE grass_top;

struct Platform {
public:
	//设置平台
	//length width 平台长度与宽度
	//position 平台左上角坐标
	//areaposx 区块标号
	//flag 是否绘制左右两边
	Platform(int lengh, int width, POINT position,int areaposx = 0, bool flag = false) :lengh(lengh),width(width){
		pos.x = position.x * 64;
		pos.y = position.y * 64;
		areax = areaposx;
		drawpos.x = pos.x + areax * areawidth;
		drawpos.y = pos.y;
		draw_side = flag;
	};
	~Platform() {};
	virtual void OnDraw(){
		if (lengh == 1) {
			for(int i = 0; i < width; i++){
				if (i == 0)putimage_alpha(drawpos.x - camera.GetPos().x, pos.y - camera.GetPos().y + i * 64, &grass_alone);
				else putimage_alpha(drawpos.x - camera.GetPos().x, pos.y - camera.GetPos().y + i * 64, &dirt_alone);
			}
		}
		else for (int i = 0; i < width; i++) {
			for (int j = 0; j < lengh; j++) {
				if (i == 0) {
					if (j == 0 && draw_side) putimage_alpha(drawpos.x - camera.GetPos().x + j * 64 , pos.y - camera.GetPos().y + i * 64, &grass_top_left);
					else if (j == lengh - 1 && draw_side)putimage_alpha(drawpos.x - camera.GetPos().x + j * 64 , pos.y - camera.GetPos().y + i * 64, &grass_top_right);
					else putimage_alpha(drawpos.x - camera.GetPos().x + j * 64 , pos.y - camera.GetPos().y + i * 64, &grass_top);
				}
				else {
					if (j == 0 && draw_side) putimage_alpha(drawpos.x - camera.GetPos().x + j * 64, pos.y - camera.GetPos().y + i * 64, &dirt_left);
					else if (j == lengh - 1 && draw_side)putimage_alpha(drawpos.x - camera.GetPos().x + j * 64, pos.y - camera.GetPos().y + i * 64, &dirt_right);
					else putimage_alpha(drawpos.x - camera.GetPos().x + j * 64 , pos.y - camera.GetPos().y + i * 64, &dirt);
				}
			}
		}
		if (show_frame) {
			setlinecolor(GREEN);
			setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 3);
			rectangle(drawpos.x - camera.GetPos().x, drawpos.y - camera.GetPos().y, drawpos.x + lengh * 64 - camera.GetPos().x, drawpos.y + 9 - camera.GetPos().y);
		}
	}
	virtual void ShowFrame(bool flag) {
		show_frame = flag;
	}
	virtual Vector2 GetPos() {
		return drawpos;
	}
	virtual int GetLengh() {
		return lengh * 64;
	}
	virtual int GetCollideType() {
		return collide_type;
	}
private:
	int areax = 0, areawidth = 2048;
	int lengh = 0;
	int width = 0;
	Vector2 pos = { 0,0 }, drawpos = { 0,0 };
	bool show_frame = false;
	bool draw_side = false;

	int collide_type = 1;
};