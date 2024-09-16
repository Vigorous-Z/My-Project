#pragma once
#include "camera.h"
#include "functionaldraw.h"
#include "animation.h"
#include <math.h>
#include <time.h>

extern Camera camera;
extern Atlas bullet_A;

struct Bullet_A {
public:

	Bullet_A() {}
	Bullet_A(Vector2 playerpos, Vector2 click_pos, Vector2 player_speed):player_speed(player_speed) {
		pos = playerpos;
		pos.y += 35;
		click_pos.x += -deviation + rand() % (2 * deviation);
		click_pos.y += -deviation + rand() % (2 * deviation);
		life += -deviation + rand() % (2 * deviation);
		direction = click_pos.operator-(pos).Normalize();
		bullet = bullet_A.GetImage(0);
		breakanim.SetAtlas(&bullet_A,50,false);
	}
	~Bullet_A() {}

	void OnUpdate(clock_t delta) {
		count += delta;
		now_size += (1 - now_size)/5;
		if (count >= life&&valid) valid = false;
		else {
			if (!valid && breakanim.CheckFinished()) can_delete = true;
			speed = start_speed * max(0, (1 - (float)count * 1.2 / (float)life));
			
			
		}
		if(valid)
		{
			pos.x += speed * direction.x * delta;
			pos.y += speed * direction.y * delta;
		}
		if (!valid) {
				breakanim.OnUpdate(delta);
				if (breakanim.CheckFinished()) can_delete = true;
			}

	}

	void OnDraw() {
		if (valid) DrawImage(pos.x - camera.GetPos().x - bullet->getwidth() / 2, pos.y - camera.GetPos().y - bullet->getheight() / 2, bullet,1,0,BLACK, now_size);
		else breakanim.OnDraw(pos.x - camera.GetPos().x - bullet->getwidth() / 2, pos.y - camera.GetPos().y - bullet->getheight() / 2);

	}

	Vector2 GetPos() {
		return pos;
		
	}

	int GetDamage() {
		return damage;
	}
	void Break() {
		
		valid = false;
	}
	bool CheckValid() {
		return valid;
	}
	bool CheckDelete() {
		return can_delete;
	}
	bool CheckCollide(Vector2 enemypos,int w,int h) {
		if (pos.x > enemypos.x - w / 2 && pos.x<enemypos.x + w / 2 && pos.y>enemypos.y - h / 2 && pos.y < enemypos.y + h / 2) {
			valid = false;
			return true;
		}
		else return false;
	}
private:
	Vector2 size;
	Vector2 pos = { 0,0 }, direction, player_speed;
	float speed = 1, start_speed = 2, now_size = 0;
	clock_t life = 500, count = 0;
	bool valid = true, can_delete = false;
	IMAGE* bullet;
	int deviation = 75, damage = 5;
	Animation breakanim;
};