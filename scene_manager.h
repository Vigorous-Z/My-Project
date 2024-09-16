#pragma once
#include "scene.h"
#include "functionaldraw.h"
extern Scene* Menu_Scene;
extern Scene* Game_Scene;
extern IMAGE black;

struct SceneManager {
public:
	enum struct SceneType {
		Menu,
		Game,
		Select,
		Setting
	};

	SceneManager() = default;
	~SceneManager() = default;

	void SetCurrentScene(Scene* scene) {
		current_scene = scene;
		current_scene->on_enter();
	}
	void SwitchTo(SceneType type) {
		is_switch = true;
		flag = type;
	}
	void OnUpdate(clock_t delta) {
		if (is_switch) {
			count += delta;
			if (!is_finished_0)
				if (count > switch_time)
				{
					is_finished_0 = true;
					current_scene->on_exit();
					switch (flag) {
					case SceneType::Menu:
						current_scene = Menu_Scene;
						break;
					case SceneType::Game:
						current_scene = Game_Scene;
						break;
					case SceneType::Select:
						break;
					default:
						break;
					}
					current_scene->on_enter();
				}
			if (!is_finished_1)
				if (count > 2 * switch_time)
					is_finished_1 = true;
			if (is_finished_0 && is_finished_1)
			{
				is_switch = false;
				is_finished_0 = false;
				is_finished_1 = false;
				count = 0;
			}
		}
		
		current_scene->on_update(delta);
	}
	void OnDraw() {
		current_scene->on_draw();
		if (is_switch && !is_finished_0)
		{
			DrawAlpha(0, 0, &black, (float)count / (float)switch_time);
		}
		else if (is_switch && !is_finished_1) {
			DrawAlpha(0, 0, &black, 2 - (float)count / (float)switch_time);
		}
	}
	void OnInput(ExMessage msg) {
		current_scene->on_input(msg);
	}

private:
	Scene* current_scene;
	bool is_switch = false, is_finished_0 = false, is_finished_1 = false;
	clock_t switch_time = 500, count = 0;
	SceneType flag;
};