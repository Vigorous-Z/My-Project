#pragma once
#define PI 3.1415926535

#include "player.h"
#include "timer.h"
#include "animation.h"
#include "particle.h"
#include <math.h>
#include <iostream>

extern Camera camera;
extern Atlas Player_A_l, Player_A_r;
extern POINT screen_size;
extern Vector2 click_pos;
extern IMAGE player_life_full;
extern IMAGE player_life_half;
extern IMAGE player_life_blank;
extern IMAGE gun, gun_flipepped;

struct PlayerB :public Player {
public:
	PlayerB() {
		timer.SetTimer(100, [&]() {
			}, false);
		player_l.SetAtlas(&Player_A_l, inter_anim, 100);
		player_r.SetAtlas(&Player_A_r, inter_anim, 100);
		dash_shadow_generation.SetTimer(30, [&]() {
			Vector2 position;
			position.x = real_pos.x + player_size.x / 2;
			position.y = real_pos.y + player_size.y / 2;
			IMAGE* frame;
			if (is_move && !is_fall)
				if (!face_r)
					frame = player_l.GetImage();
				else
					frame = player_r.GetImage();
			else frame = player_static;
			Particle particle(position, frame, 100);
			particle.SetTransparent(true, 100);
			Particle_list.push_back(particle);
			}, false);
		bullet_gengration.SetTimer(shoot_cooldown, [&]() {
			bullet_list.emplace_back(pos, click_pos, speed);
			}, false);
		hurt_timer.SetTimer(1000, [&]() {
			is_hurt = false;
			}, false);
	}
	~PlayerB() {}

	void DrawPlayerUI() {
		for (int i = 0; i < life_max / 2 + life_max % 2; i++) {
			if (life >= (i + 1) * 2) putimage_alpha(10 + i * player_life_full.getwidth(), 10, &player_life_full);
			else if (life >= i * 2 + 1) putimage_alpha(10 + i * player_life_half.getwidth(), 10, &player_life_half);
			else putimage_alpha(10 + i * player_life_blank.getwidth(), 10, &player_life_blank);
		}
	}

	void OnUpdate(clock_t delta) {
		timer.OnUpdate(delta);
		last_y = pos.y + player_static_size.y / 2;
		if (face_r) {//更新玩家动画
			player_r.OnUpdate(delta);
		}
		else {
			player_l.OnUpdate(delta);
		}
		is_move = is_move_r || is_move_l;//更新移动状态
		if (is_move_r + is_move_l == 1) face_r = is_move_r;

		if (!face_r)
			player_static = player_l.GetImage(is_fall);
		else
			player_static = player_r.GetImage(is_fall);
		player_size.x = fabs(player_static->getwidth() * cos(rad)) + fabs(player_static->getheight() * sin(rad));
		player_size.y = fabs(player_static->getwidth() * sin(rad)) + fabs(player_static->getheight() * cos(rad));
		player_static_size.x = player_static->getwidth();
		player_static_size.y = player_static->getheight();



		if (is_move) {//更新玩家速度和倾斜角
			speed.x = fabs(speed.x) >= speed_limit ? speed.x : speed.x + (1 - 2 * (!face_r)) * speed_up * delta;
			rad += (!is_fall) * ((1 - 2 * (face_r)) * run_rad - rad) * 0.005 * delta;
		}
		else {
			speed.x = fabs(speed.x) < speed_down * delta ? 0 : speed.x - (1 - 2 * (speed.x < 0)) * speed_down * delta * (1 - (fabs(speed.x) <= speed_limit && is_fall) * 0.8);
			rad = (!is_fall) * rad * pow(0.99, delta);
		}
		if (is_fall) {
			rad += ((2 * face_r - 1) * run_rad / 2 - rad) * 0.004 * delta;
		}
		if (speed.x / fabs(speed.x) != face_r - !face_r && is_move) {//反向时速度修正
			speed.x = fabs(speed.x) < speed_down ? 0 : speed.x - (1 - 2 * (speed.x < 0)) * speed_down * delta;
		}
		if (fabs(speed.x) > speed_limit)//超速处理
			speed.x = speed.x < 0 ? speed.x + min(-speed.x, speed_down * delta) : speed.x - min(speed.x, speed_down * delta);

		if (is_jump) {//跳跃检测
			is_jump = false;
			speed.y = -jump_speed;
			is_fall = true;
			dash_count = dash_cooldown;
			can_dash = true;
		}
		if (!is_fall)//跳跃条件判断
			can_jump = true;


		speed.y = min(60, speed.y + gravity * delta);


		if (fabs(speed.x) > speed_limit) dash_shadow_generation.Pause(false);
		else dash_shadow_generation.Pause(true);

		dash_shadow_generation.OnUpdate(delta);
		bullet_gengration.OnUpdate(delta);
		hurt_timer.OnUpdate(delta);

		if (!can_dash && !is_fall) {//冲刺条件判断
			dash_count += delta;
			if (dash_count >= dash_cooldown) {
				dash_count = 0;
				can_dash = true;
			}
		}

		if (is_dash) {//冲刺检测
			speed.x = dash_speed * (2 * face_r - 1);
			is_dash = false;
		}
		if (fabs(speed.x) > speed_limit && speed.y > 0)
		{
			speed.y = 0;
		}
		if (speed.y > 50)camera.Shake(true);
		else camera.Shake(false);

		pos.x += speed.x * (1 - is_shoot * 0.3) * delta;
		pos.y += speed.y * delta;//更新坐标
		real_pos.x = pos.x - player_size.x / 2;
		real_pos.y = pos.y - player_size.y / 2;

		if (pos.y > 10000) {
			pos.y = -10000;
			life--;
		}
		camera.Shake(is_shoot);
		camera.UpDate(pos.x - screen_size.x / 2.0 + player_size.x / 2.0, pos.y - screen_size.y / 2.0 + player_size.y / 2.0);//更新摄像机


		if (is_shoot)bullet_gengration.Pause(false);
		else bullet_gengration.Pause(true);

		Particle_list.erase(remove_if(
			Particle_list.begin(), Particle_list.end(),
			[](Particle& particle) {
				return !particle.CheckValid();
			}),
			Particle_list.end());
		for (Particle& particle : Particle_list) {
			particle.OnUpdate(delta);
		}

		bullet_list.erase(remove_if(
			bullet_list.begin(), bullet_list.end(),
			[](Bullet_A& b) {
				return b.CheckDelete();
			}),
			bullet_list.end());
		for (Bullet_A& b : bullet_list) {
			b.OnUpdate(delta);
		}

		if (atan2(click_pos.x - pos.x, click_pos.y - pos.y) < 0)
			gun_rotated = RotateImageAlpha(&gun_flipepped, atan2(click_pos.x - pos.x, click_pos.y - pos.y) - PI * 3 / 2);
		else gun_rotated = RotateImageAlpha(&gun, atan2(click_pos.x - pos.x, click_pos.y - pos.y) - PI / 2);
	};
	void OnDraw() {
		for (Particle& particle : Particle_list) {
			particle.OnDraw();
		}
		DrawPlayerUI();

		if (is_move && !is_fall)
			if (!face_r)
				player_l.OnDraw(real_pos.x - camera.GetPos().x, real_pos.y - camera.GetPos().y, 1 - is_hurt * 0.5, rad);
			else
				player_r.OnDraw(real_pos.x - camera.GetPos().x, real_pos.y - camera.GetPos().y, 1 - is_hurt * 0.5, rad);
		else DrawImage(real_pos.x - camera.GetPos().x, real_pos.y - camera.GetPos().y, player_static, 1 - is_hurt * 0.5, rad);

		for (Bullet_A b : bullet_list) {
			b.OnDraw();
		}
		DrawImage(pos.x - camera.GetPos().x - gun_rotated.getwidth() / 2, pos.y - camera.GetPos().y - gun_rotated.getheight() / 2 + 20, &gun_rotated);

		if (show_frame) {
			setlinestyle(PS_SOLID | PS_ENDCAP_SQUARE, 3);
			setlinecolor(BLUE);
			rectangle(real_pos.x - camera.GetPos().x, real_pos.y - camera.GetPos().y, real_pos.x - camera.GetPos().x + player_size.x, real_pos.y - camera.GetPos().y + player_size.y);
			circle(pos.x - camera.GetPos().x, pos.y - camera.GetPos().y, 5);
			for (Bullet_A& b : bullet_list) {
				if (b.CheckValid())rectangle(b.GetPos().x - camera.GetPos().x - 3, b.GetPos().y - camera.GetPos().y - 3, b.GetPos().x - camera.GetPos().x + 3, b.GetPos().y - camera.GetPos().y + 3);
			}
		}
	}
	void OnInput(const ExMessage& msg) {
		switch (msg.message) {
		case WM_KEYDOWN:
			switch (msg.vkcode) {
			case 0x41:
				face_r = false;
				is_move_l = true;
				break;
			case 0x44:
				face_r = true;
				is_move_r = true;
				break;
			case 0x46:
				if (can_dash) {
					is_dash = true;
					can_dash = false;
				}
				break;
			case VK_SPACE:
				if (can_jump) {
					is_jump = true;
					can_jump = false;
				}
				break;
			}break;
		case WM_KEYUP:
			switch (msg.vkcode) {
			case 0x41:
				is_move_l = false;
				break;
			case 0x44:
				is_move_r = false;
				break;
			}break;
		case WM_LBUTTONDOWN:
			is_shoot = true;
			bullet_gengration.Shot();
			break;
		case WM_LBUTTONUP:
			is_shoot = false;
			break;
		}
	}
	void OnExit() {
		speed.x = 0, speed.y = 0;
	}

	POINT GetPlayerStaticSize() {
		return player_static_size;
	}
	Vector2 GetPos() {
		return pos;
	}
	IMAGE* GetIndex() {
		return face_r ? player_r.GetImage() : player_l.GetImage();
	}
	int BulletCollide(Vector2 enemypos, int w, int h) {
		int bullets = 0;
		for (Bullet_A& b : bullet_list) {
			if (b.CheckValid() && b.CheckCollide(enemypos, w, h)) {
				bullets++;
			}
		}
		if (bullets > 0)
			return bullet_list[0].GetDamage();
		else return 0;
	}
	void PlatformCollide(vector<Platform> platform_list) {
		if (speed.y >= 0) {//碰撞检测
			for (Platform& p : platform_list) {
				if (pos.x + player_static_size.x / 2 > p.GetPos().x && pos.x - player_static_size.x / 2 < p.GetPos().x + p.GetLengh() && last_y <= p.GetPos().y && pos.y + player_static_size.y / 2 > p.GetPos().y) {
					speed.y = 0;
					pos.y = p.GetPos().y - player_static_size.y / 2;
					is_fall = false;
				}
			}
		}
	}
	void Hurt(vector<Enemy*> enemy_list) {
		if (!is_hurt) {
			hurt_timer.RestartPause(true);
			for (Enemy*& e : enemy_list) {
				if (e->GetPos().x > pos.x - player_size.x / 2 && e->GetPos().x < pos.x + player_size.x / 2 && e->GetPos().y > pos.y - player_size.y / 2 && e->GetPos().y < pos.y + player_size.y / 2)
				{
					life--;
					if (e->GetPos().x > pos.x) {
						speed.x = -1;
					}
					else {
						speed.x = 1;
					}
					speed.y = -1;
					is_hurt = true;
					is_fall = true;
					break;
				}
			}
		}
		else {
			hurt_timer.Pause(false);
		}
	}
	void ShowFrame(bool flag) {
		show_frame = flag;
	}
private:
	Timer timer;
	POINT player_size = { 0,0 }, player_static_size = { 0,0 };
	Vector2 pos = { 1024,-100 }, speed = { 0,0 }, real_pos = { 0,-100 };
	Animation player_r, player_l;
	IMAGE* player_static;
	IMAGE gun_rotated;
	clock_t inter_anim = 100, dash_count = 0, dash_cooldown = 300, shoot_cooldown = 20;
	float rad = 0;
	float last_y = 0;
	float speed_up = 0.003, speed_down = 0.004, speed_limit = 0.75, jump_speed = 1.5, gravity = 0.005, dash_speed = 2;
	float run_rad = PI / 12;
	int life = 6, life_max = 6;
	bool is_move = false, is_move_l = false, is_move_r = false;
	bool face_r = true;
	bool is_fall = false;
	bool is_land = false;
	bool show_frame;
	bool is_hurt = false;
	bool is_dead = false, dead_count = 0;
	bool is_jump = false, can_jump = false;
	bool is_dash = false, can_dash = false;
	bool is_shoot = false;
	vector<Particle> Particle_list;
	vector<Bullet_A> bullet_list;
	Timer dash_shadow_generation;//冲刺影子
	Timer bullet_gengration;
	Timer hurt_timer;
};