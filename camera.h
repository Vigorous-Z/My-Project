#pragma once
#include "vector2.h"
#include <math.h>
#include <time.h>
#include<stdlib.h>

struct Camera {
public:
	Camera() {}
	~Camera() {}

	Vector2 GetPos() {
		return pos;
	}
	void Reset() {
		pos.x = 0;
		pos.y = 0;
	}
	//更新摄像机
	//newx,newy 摄像机新的坐标
	void UpDate(float newx, float newy) {
		real_pos.x = real_pos.x*(1-follow) + newx * follow;
		real_pos.y = ((((float)-800) < (real_pos.y * (1 - follow) + newy * follow)) ? ((float)-800) : (real_pos.y * (1 - follow) + newy * follow));
		pos = real_pos;
		if (is_shake) {
			pos.x += -shake_strength + rand() % (2 * shake_strength);
			pos.y += -shake_strength + rand() % (2 * shake_strength);
		}
	}
	void Shake(bool flag) {
		is_shake = flag;
	}
private:
	Vector2 real_pos = { 2048,0 }, pos = {2048,0};
	float follow = 0.2;
	int shake_strength = 3;
	bool is_shake = false;
};