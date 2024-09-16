#pragma once
#include "playerA.h"
#include "floattext.h"
#include <graphics.h>
#include <tchar.h>

extern Camera cmaera;
extern PlayerA player_A;
extern Atlas flybot_r,flybot_l;
extern IMAGE flybot_attacked_l, flybot_attacked_r;

struct Flybot : public Enemy {
public:
	Flybot() {};
	Flybot(Vector2 spawnpos, Vector2 worldpos = {0,0}) {
		pos = spawnpos.operator+(worldpos);
		size.x = flybot_l.GetImage(0)->getwidth();
		size.y = flybot_l.GetImage(0)->getheight();
		anim_l.SetAtlas(&flybot_l, 100, true);
		anim_r.SetAtlas(&flybot_r, 100, true);
		attack_count.SetTimer(1000, [&]() {
			resume = true;
			}, false);

		count = rand()%1000;
	}
	~Flybot() {}
	void OnUpdate(clock_t delta, Vector2 player_pos) {
		attack_count.OnUpdate(delta);
		count = (count + delta) % 1000;
		distance = player_pos.operator-(pos);
		if (distance.Length() <= 640&&resume) {
			distance = distance.Normalize();
			direction.x = direction.x / 2 + distance.x / 2;
			direction.y = direction.y / 2 + distance.y / 2;
			speed_up_count = min(speed_up_time, speed_up_count + delta);
			if (direction.x > 0) face_r = true;
			else if (direction.x < 0) face_r = false;
		}
		else {
			direction = direction.Normalize();
			speed_up_count = max(0, speed_up_count - delta*2);
			pos.y += sin((float)count / (float)1000 * delta);
		}
		now_speed = (resume ? 1 : -1) * speed * (1 - hurt * 0.5) * (speed_up_count / (float)speed_up_time);
		pos.x += now_speed * direction.x * delta;
		pos.y += now_speed * direction.y * (1 - hurt * 0.5) * (speed_up_count / (float)speed_up_time) * delta;

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
	void OnDraw() {
		if (hurt) {
			if (face_r) putimage_alpha(pos.x - size.x / 2 - camera.GetPos().x, pos.y - size.y / 2 - camera.GetPos().y,&flybot_attacked_r);
			else putimage_alpha(pos.x - size.x / 2 - camera.GetPos().x, pos.y - size.y / 2 - camera.GetPos().y,&flybot_attacked_l);
		}
		else {
			if (face_r) anim_r.OnDraw(pos.x - size.x / 2 - camera.GetPos().x, pos.y - size.y / 2 - camera.GetPos().y);
			else anim_l.OnDraw(pos.x - size.x / 2 - camera.GetPos().x, pos.y - size.y / 2 - camera.GetPos().y);
		}

		if (show_frame) {
			setlinecolor(RED);
			setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 3);
			rectangle(pos.x - size.x / 2 - camera.GetPos().x, pos.y - size.y / 2 - camera.GetPos().y, pos.x + size.x / 2 - camera.GetPos().x, pos.y + size.y / 2 - camera.GetPos().y);
			circle(pos.x - camera.GetPos().x, pos.y - camera.GetPos().y, 5);
		}
		
	}
	void Hurt(int damage) {
		if (!hurt && damage > 0) {
			attacked_damage = damage;
			life -= damage;
			hurt = true;
		}
		else attacked_damage = 0;
	}
	void ShowFrame(bool flag) {
		show_frame = flag;
	}
	bool CheckAlive() {
		return dead;
	}
	int CheckHurt() {
		return attacked_damage;
	}
	Vector2 GetPos() {
		return pos;
	}
	int GetWidth() {
		return size.x;
	}
	int GetHeight() {
		return size.y;
	}
	void Attack(bool right) {
		direction.x = right ? 1 : -1;
		direction.y = -1;
		direction.Normalize();
		resume = false;
	}

private:
	Vector2 pos, direction, size, distance;
	float speed = 2.5, now_speed;
	Animation anim_r,anim_l;
	bool face_r = false, dead = false, hurt = false, show_frame = false,resume = true;
	int life = 50, seek_area = 640, attacked_damage = 0;
	TCHAR str[256];
	clock_t hurt_time = 10, hurt_count = 0, count = 0, speed_up_count = 0, speed_up_time = 2000;
	Timer attack_count;
};