#pragma once
#include "camera.h"
#include "bullet_a.h"
#include "platform.h"
#include "enemy.h"
#include <graphics.h>
#include <time.h>
#include <vector>


struct Player {
public:
	Player() {}
	~Player() {}
	
	virtual void OnUpdate(clock_t delta) {

	}
	virtual void OnDraw() {

	}
	virtual void OnInput(const ExMessage& msg) {

	}
	virtual Vector2 GetPos() {
		return { 0, 0 };
	}
	virtual IMAGE* GetIndex() {
		return NULL;
	}
	virtual int BulletCollide(Vector2 enemypos, int w, int h) {
		return 0;
	}
	virtual void ShowFrame(bool flag) {

	}
	virtual void PlatformTopCollide(vector<Platform> platform_list) {
	
	}
	virtual void Hurt(vector<Enemy*> enemy_list) {

	}
private:

};
