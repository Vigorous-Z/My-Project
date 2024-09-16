#pragma once
#include "camera.h"
#include "timer.h"
#include "functionaldraw.h"

extern Camera camera;

struct Enemy{
public:
	Enemy() {}
	Enemy(Vector2 spawnpos, Vector2 worldpos) {}
	~Enemy() {}
	virtual void OnUpdate(clock_t delta, Vector2 player_pos) {
		attack_count.OnUpdate(delta);
		count = (count + delta);
		if (resume)
			distance = player_pos.operator-(pos);
		if (distance.Length() <= 640) {
			if (resume) {
				distance = distance.Normalize();
				direction.x = direction.x / 2 + distance.x / 2;
				direction.y = direction.y / 2 + distance.y / 2;
			}
			speed_up_count = min(speed_up_time, speed_up_count + delta);
			if (direction.x > 0) face_r = true;
			else if (direction.x < 0) face_r = false;
		}
		else {
			direction = direction.Normalize();
			speed_up_count = max(0, speed_up_count - delta);
			pos.y += sin((float)count / (float)3000 * delta);
		}
		pos.x += speed * direction.x * (1 - hurt * 0.5) * (speed_up_count / (float)speed_up_time) * delta;
		pos.y += speed * direction.y * (1 - hurt * 0.5) * (speed_up_count / (float)speed_up_time) * delta;

		if (face_r) anim_r.OnUpdate(delta);
		else anim_l.OnUpdate(delta);
		if (life <= 0)dead = true;
		if (hurt) {
			hurt_count += delta;
			if (hurt_count > hurt_time) {
				hurt_count = 0;
				hurt = false;
			}
		}
	}
	virtual void OnDraw() {

	}
	virtual void Hurt(int damage) {
		if (!hurt && damage > 0) {
			attacked_damage = damage;
			life -= damage;
			hurt = true;
		}
	}
	virtual void ShowFrame(bool flag) {
		show_frame = flag;
	}
	virtual bool CheckAlive() {
		return dead;
	}
	virtual Vector2 GetPos() {
		return pos;
	}
	virtual int GetWidth() {
		return size.x;
	}
	virtual int GetHeight() {
		return size.y;
	}
	virtual int CheckHurt() {
		return attacked_damage;
	}
	virtual void Attack(bool right) {

	}

private:
	Vector2 pos, direction, size, distance;
	float speed = 2.5;
	Animation anim_r, anim_l;
	bool face_r = false, dead = false, hurt = false, show_frame = false, resume = true;
	int life = 50, seek_area = 640, attacked_damage = 0;
	TCHAR str[256];
	clock_t hurt_time = 10, hurt_count = 0, count = 0, speed_up_count = 0, speed_up_time = 2000;
	Timer attack_count;
};