#pragma once
#include "atlas.h"
#include "functionaldraw.h"
#include <graphics.h>


struct Animation {
public:
	Animation() {}
	~Animation() {}

	void Reset() {
		count = 0;
		idx_frame = 0;
	}
	//设置当前图集
	// new_atlas 图集，
	// ms 帧间隔， 
	// loop 是否循环
	void SetAtlas(Atlas* new_atlas, int ms = 45, bool loop = true) {
		Reset();
		interval = ms;
		atlas = new_atlas;
		is_loop = loop;
	}
	int GetIndex() {
		return idx_frame;
	}
	IMAGE* GetImage(int idx = 100) {
		if(idx > atlas->GetSize())
			return atlas->GetImage(idx_frame);
		else return atlas->GetImage(idx);
	}
	bool CheckFinished() {
		if (is_loop) return false;
		return (idx_frame == atlas->GetSize() - 1);
	}

	void OnUpdate(clock_t delta) {
		count += delta;
		if (count >= interval) {
			idx_frame += count/interval;
			count = 0;
			if (idx_frame >= atlas->GetSize()) {
				idx_frame = is_loop ? 0 : atlas->GetSize() - 1;
			}
		}
	}	
// 绘制当前图集，支撑透明信息、覆盖透明度、旋转图片
// x,y 绘制位置的左上角坐标
// AA 覆盖透明度，0为完全透明
// rad 旋转弧度,为正时逆时针旋转
// bkcolor 背景填充颜色
	void OnDraw(int x, int y, double AA = 1, double rad = 0, COLORREF bkcolor = BLACK) {
		DrawImage(x, y, atlas->GetImage(idx_frame), AA, rad, bkcolor);
	}

private:
	clock_t count = 0;
	clock_t interval = 100;
	int idx_frame = 0;
	bool is_loop = true;
	Atlas* atlas = NULL;
};