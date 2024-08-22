#pragma once

#include <graphics.h>

class Scene {
public:
	Scene() = default;
	~Scene() = default;

	virtual void on_enter(){}
	virtual void on_update(bool* running) {}
	virtual void on_draw() {}
	virtual void on_input(const ExMessage& msg) {}
	virtual void on_exit() {}

private:
};