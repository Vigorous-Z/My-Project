#pragma once

#include "player.h"
#include "playerA.h"

extern Camera camera;

struct Particle {
public:
	//设置粒子
	//pos 粒子位置
	//atlas 粒子图集
	//life 粒子持续时间
	Particle(const Vector2& pos, Atlas* atlas, clock_t life):pos(pos),atlas(atlas),life(life) {
		interval = max(1, life / atlas->GetSize());
		valid = true;
		is_img = false;
	}
	//设置粒子
	//pos 粒子位置
	//atlas 粒子图片
	//life 粒子持续时间
	Particle(const Vector2& pos, IMAGE* image, clock_t life) :pos(pos), image(image), life(life) {
		valid = true;
		is_img = true;
	}
	~Particle() {}
	//设置粒子透明化
	//flag 是否透明化
	//time 透明化时间
	//start 开始时透明度
	//end 结束时透明度
	void SetTransparent(bool flag = false, clock_t time = 0, float start = 1, float end = 0) {
		is_transparent = flag;
		transparent_time = time;
		startAA = start;
		endAA = end;
	}
	bool CheckValid() {
		return valid;
	}
	void OnUpdate(clock_t delta) {
		count += delta;
		if (is_transparent) {
			nowAA = (1-(float)count / life * (startAA - endAA)) + endAA;
			if (nowAA < 0)nowAA = 0;
			else if (nowAA > 1)nowAA = 1;
		}
		if (!is_img) {
			idx_frame = (int)count / (int)interval;
		}
		if (count >= life) valid = false;
	}
	void OnDraw() {
		if (is_img) {
			DrawAlpha(pos.x - image->getwidth() / 2 - camera.GetPos().x, pos.y - image->getheight() / 2 - camera.GetPos().y, image, (is_transparent)*nowAA + !is_transparent);
		}
	}
private:
	clock_t count = 0;
	clock_t life = 0;
	clock_t interval = 1;
	clock_t transparent_time = 0;
	float startAA = 1, endAA = 0, nowAA = 1;
	int idx_frame = 0;
	Vector2 pos = { 0,0 };
	bool valid = false;
	bool is_transparent = false;
	bool is_img = false;
	Atlas* atlas = NULL;
	IMAGE* image = NULL;
};