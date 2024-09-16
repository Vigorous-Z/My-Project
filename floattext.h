#pragma once
#include <graphics.h>
#include <time.h>
#include "camera.h"
#include "atlas.h"
#include "functionaldraw.h"

extern Camera camera;
extern Atlas num;

struct FloatNum {
public:
	FloatNum(int number, Vector2 spawnpos) {
		display_num = number;
		pos = spawnpos;
		direction = { (float)(rand() % 100) - 50,(float)(rand() % 100) - 50 };
		direction = direction.Normalize();
		size = num.GetImage(0)->getwidth();
		speed *= (rand() % 5 / (float)10 + 0.5 );
		now_speed = speed;
	}
	~FloatNum() {}
	void OnUpdate(clock_t delta) {
		count += delta;
		pos.x += now_speed * direction.x;
		pos.y += now_speed * direction.y;
		now_speed -= speed * speed_down;
		if (now_speed < 0) now_speed = 0;
		if (count >= life) {
			AA = 0;
			valid = false;
		}
		else AA = 1 - count / (float)life;
	}
	void OnDraw() {
		int temp = display_num;
		for (int i = 0;temp != 0; i++) {
			DrawImage(pos.x + (temp < 10 ? 0 : log10(temp)) * size - camera.GetPos().x, pos.y - camera.GetPos().y, num.GetImage(temp % 10), AA);
			temp = temp / 10;
		}
	}
	bool CheckValid() {
		return valid;
	}

private:
	Vector2 direction = { 0,0 }, pos;
	int display_num = 0, size = 0;
	clock_t life = 1000, count = 0;
	float AA = 1;
	float speed = 10, now_speed = 10, speed_down = 0.03;
	bool valid = true;
};